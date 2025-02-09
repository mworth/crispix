/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/*
	MiNT audio driver
	using XBIOS functions (STFA driver)

	Patrice Mandin
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Mint includes */
#include <mint/osbind.h>
#include <mint/falcon.h>
#include <mint/cookie.h>

#include "SDL_endian.h"
#include "SDL_audio.h"
#include "SDL_audio_c.h"
#include "SDL_audiomem.h"
#include "SDL_sysaudio.h"

#include "SDL_atarimxalloc_c.h"

#include "SDL_mintaudio.h"
#include "SDL_mintaudio_stfa.h"

/*--- Defines ---*/

#define MINT_AUDIO_DRIVER_NAME "mint_stfa"

/* Debug print info */
#define DEBUG_NAME "audio:stfa: "
#if 0
#define DEBUG_PRINT(what) \
	{ \
		printf what; \
	}
#else
#define DEBUG_PRINT(what)
#endif

/*--- Static variables ---*/

static unsigned long cookie_snd, cookie_mch;
static cookie_stfa_t *cookie_stfa;

static const int freqs[16]={
	4995,	6269,	7493,	8192,
	9830,	10971,	12538,	14985,
	16384,	19819,	21943,	24576,
	30720,	32336,	43885,	49152
};

/*--- Audio driver functions ---*/

static void Mint_CloseAudio(_THIS);
static int Mint_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void Mint_LockAudio(_THIS);
static void Mint_UnlockAudio(_THIS);

/* To check/init hardware audio */
static int Mint_CheckAudio(_THIS, SDL_AudioSpec *spec);
static void Mint_InitAudio(_THIS, SDL_AudioSpec *spec);

/*--- Audio driver bootstrap functions ---*/

static int Audio_Available(void)
{
	const char *envr = getenv("SDL_AUDIODRIVER");

	/* Check if user asked a different audio driver */
	if ((envr) && (strcmp(envr, MINT_AUDIO_DRIVER_NAME)!=0)) {
		DEBUG_PRINT((DEBUG_NAME "user asked a different audio driver\n"));
		return(0);
	}

	/* Cookie _MCH present ? if not, assume ST machine */
	if (Getcookie(C__MCH, &cookie_mch) == C_NOTFOUND) {
		cookie_mch = MCH_ST;
	}

	/* Cookie _SND present ? if not, assume ST machine */
	if (Getcookie(C__SND, &cookie_snd) == C_NOTFOUND) {
		cookie_snd = SND_PSG;
	}

	/* Cookie STFA present ? */
	if (Getcookie(C_STFA, (long *) &cookie_stfa) != C_FOUND) {
		DEBUG_PRINT((DEBUG_NAME "no STFA audio\n"));
		return(0);
	}

	SDL_MintAudio_stfa = cookie_stfa;

	DEBUG_PRINT((DEBUG_NAME "STFA audio available!\n"));
	return(1);
}

static void Audio_DeleteDevice(SDL_AudioDevice *device)
{
    free(device->hidden);
    free(device);
}

static SDL_AudioDevice *Audio_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	/* Initialize all variables that we clean on shutdown */
	this = (SDL_AudioDevice *)malloc(sizeof(SDL_AudioDevice));
    if ( this ) {
        memset(this, 0, (sizeof *this));
        this->hidden = (struct SDL_PrivateAudioData *)
                malloc((sizeof *this->hidden));
    }
    if ( (this == NULL) || (this->hidden == NULL) ) {
        SDL_OutOfMemory();
        if ( this ) {
            free(this);
        }
        return(0);
    }
    memset(this->hidden, 0, (sizeof *this->hidden));

    /* Set the function pointers */
    this->OpenAudio   = Mint_OpenAudio;
    this->CloseAudio  = Mint_CloseAudio;
    this->LockAudio   = Mint_LockAudio;
    this->UnlockAudio = Mint_UnlockAudio;
    this->free        = Audio_DeleteDevice;

    return this;
}

AudioBootStrap MINTAUDIO_STFA_bootstrap = {
	MINT_AUDIO_DRIVER_NAME, "MiNT STFA audio driver",
	Audio_Available, Audio_CreateDevice
};

static void Mint_LockAudio(_THIS)
{
	void *oldpile;

	/* Stop replay */
	oldpile=(void *)Super(0);
	cookie_stfa->sound_enable=STFA_PLAY_DISABLE;
	Super(oldpile);
}

static void Mint_UnlockAudio(_THIS)
{
	void *oldpile;

	/* Restart replay */
	oldpile=(void *)Super(0);
	cookie_stfa->sound_enable=STFA_PLAY_ENABLE|STFA_PLAY_REPEAT;
	Super(oldpile);
}

static void Mint_CloseAudio(_THIS)
{
	void *oldpile;

	/* Stop replay */
	oldpile=(void *)Super(0);
	cookie_stfa->sound_enable=STFA_PLAY_DISABLE;
	Super(oldpile);

	/* Wait if currently playing sound */
	while (SDL_MintAudio_mutex != 0) {
	}

	/* Clear buffers */
	if (SDL_MintAudio_audiobuf[0]) {
		Mfree(SDL_MintAudio_audiobuf[0]);
		SDL_MintAudio_audiobuf[0] = SDL_MintAudio_audiobuf[1] = NULL;
	}
}

static int Mint_CheckAudio(_THIS, SDL_AudioSpec *spec)
{
	int i;

	DEBUG_PRINT((DEBUG_NAME "asked: %d bits, ",spec->format & 0x00ff));
	DEBUG_PRINT(("signed=%d, ", ((spec->format & 0x8000)!=0)));
	DEBUG_PRINT(("big endian=%d, ", ((spec->format & 0x1000)!=0)));
	DEBUG_PRINT(("channels=%d, ", spec->channels));
	DEBUG_PRINT(("freq=%d\n", spec->freq));

	/* Check formats available */
	MINTAUDIO_nfreq=16;
	MINTAUDIO_sfreq=0;
	for (i=MINTAUDIO_sfreq;i<MINTAUDIO_nfreq;i++) {
		MINTAUDIO_hardfreq[i]=freqs[15-i];
		DEBUG_PRINT((DEBUG_NAME "calc:freq(%d)=%lu\n", i, MINTAUDIO_hardfreq[i]));
	}

	MINTAUDIO_numfreq=SDL_MintAudio_SearchFrequency(this, 0, spec->freq);
	spec->freq=MINTAUDIO_hardfreq[MINTAUDIO_numfreq];

	DEBUG_PRINT((DEBUG_NAME "obtained: %d bits, ",spec->format & 0x00ff));
	DEBUG_PRINT(("signed=%d, ", ((spec->format & 0x8000)!=0)));
	DEBUG_PRINT(("big endian=%d, ", ((spec->format & 0x1000)!=0)));
	DEBUG_PRINT(("channels=%d, ", spec->channels));
	DEBUG_PRINT(("freq=%d\n", spec->freq));

	return 0;
}

static void Mint_InitAudio(_THIS, SDL_AudioSpec *spec)
{
	void *buffer;
	void *oldpile;

	buffer = SDL_MintAudio_audiobuf[SDL_MintAudio_numbuf];

	oldpile=(void *)Super(0);

	/* Stop replay */
	cookie_stfa->sound_enable=STFA_PLAY_DISABLE;

	/* Select replay format */
	cookie_stfa->sound_control = 15-MINTAUDIO_numfreq;
	if ((spec->format & 0xff)==8) {
		cookie_stfa->sound_control |= STFA_FORMAT_8BIT;
	} else {
		cookie_stfa->sound_control |= STFA_FORMAT_16BIT;
	}
	if (spec->channels==2) {
		cookie_stfa->sound_control |= STFA_FORMAT_STEREO;
	} else {
		cookie_stfa->sound_control |= STFA_FORMAT_MONO;
	}
	if ((spec->format & 0x8000)!=0) {
		cookie_stfa->sound_control |= STFA_FORMAT_SIGNED;
	} else {
		cookie_stfa->sound_control |= STFA_FORMAT_UNSIGNED;
	}
	if ((spec->format & 0x1000)!=0) {
		cookie_stfa->sound_control |= STFA_FORMAT_BIGENDIAN;
	} else {
		cookie_stfa->sound_control |= STFA_FORMAT_LITENDIAN;
	}

	/* Set buffer */
	cookie_stfa->sound_start = (unsigned long) buffer;
	cookie_stfa->sound_end = (unsigned long) (buffer + spec->size);

	/* Set interrupt */
	cookie_stfa->stfa_it = SDL_MintAudio_StfaInterrupt;

	/* Restart replay */
	cookie_stfa->sound_enable=STFA_PLAY_ENABLE|STFA_PLAY_REPEAT;

	Super(oldpile);

	DEBUG_PRINT((DEBUG_NAME "hardware initialized\n"));
}

static int Mint_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	SDL_MintAudio_device = this;

	/* Check audio capabilities */
	if (Mint_CheckAudio(this, spec)==-1) {
		return -1;
	}

	SDL_CalculateAudioSpec(spec);

	/* Allocate memory for audio buffers in DMA-able RAM */
	DEBUG_PRINT((DEBUG_NAME "buffer size=%d\n", spec->size));

	SDL_MintAudio_audiobuf[0] = Atari_SysMalloc(spec->size *2, MX_STRAM);
	if (SDL_MintAudio_audiobuf[0]==NULL) {
		SDL_SetError("MINT_OpenAudio: Not enough memory for audio buffer");
		return (-1);
	}
	SDL_MintAudio_audiobuf[1] = SDL_MintAudio_audiobuf[0] + spec->size ;
	SDL_MintAudio_numbuf=0;
	memset(SDL_MintAudio_audiobuf[0], spec->silence, spec->size *2);
	SDL_MintAudio_audiosize = spec->size;
	SDL_MintAudio_mutex = 0;

	DEBUG_PRINT((DEBUG_NAME "buffer 0 at 0x%08x\n", SDL_MintAudio_audiobuf[0]));
	DEBUG_PRINT((DEBUG_NAME "buffer 1 at 0x%08x\n", SDL_MintAudio_audiobuf[1]));

	/* Setup audio hardware */
	Mint_InitAudio(this, spec);

    return(1);	/* We don't use threaded audio */
}
