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
	using XBIOS functions (MacSound compatible driver)

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
#include "SDL_mintaudio_mcsn.h"

/*--- Defines ---*/

#define MINT_AUDIO_DRIVER_NAME "mint_mcsn"

/* Debug print info */
#define DEBUG_NAME "audio:mcsn: "
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
static cookie_mcsn_t *cookie_mcsn;

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

	/* Check if we have 16 bits audio */
	if ((cookie_snd & SND_16BIT)==0) {
		DEBUG_PRINT((DEBUG_NAME "no 16 bits sound\n"));
	    return(0);
	}

	/* Cookie MCSN present ? */
	if (Getcookie(C_McSn, (long *) &cookie_mcsn) != C_FOUND) {
		DEBUG_PRINT((DEBUG_NAME "no MCSN audio\n"));
		return(0);
	}

	/* Check if interrupt at end of replay */
	if (cookie_mcsn->pint == 0) {
		DEBUG_PRINT((DEBUG_NAME "no interrupt at end of replay\n"));
		return(0);
	}

	/* Check if audio is lockable */
	if (Locksnd()!=1) {
		DEBUG_PRINT((DEBUG_NAME "audio locked by other application\n"));
		return(0);
	}

	Unlocksnd();

	DEBUG_PRINT((DEBUG_NAME "MCSN audio available!\n"));
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

AudioBootStrap MINTAUDIO_MCSN_bootstrap = {
	MINT_AUDIO_DRIVER_NAME, "MiNT MCSN audio driver",
	Audio_Available, Audio_CreateDevice
};

static void Mint_LockAudio(_THIS)
{
	/* Stop replay */
	Buffoper(0);
}

static void Mint_UnlockAudio(_THIS)
{
	/* Restart replay */
	Buffoper(SB_PLA_ENA|SB_PLA_RPT);
}

static void Mint_CloseAudio(_THIS)
{
	/* Stop replay */
	Buffoper(0);

	/* Uninstall interrupt */
	Jdisint(MFP_DMASOUND);

	/* Wait if currently playing sound */
	while (SDL_MintAudio_mutex != 0) {
	}

	/* Clear buffers */
	if (SDL_MintAudio_audiobuf[0]) {
		Mfree(SDL_MintAudio_audiobuf[0]);
		SDL_MintAudio_audiobuf[0] = SDL_MintAudio_audiobuf[1] = NULL;
	}

	/* Unlock sound system */
	Unlocksnd();
}

static int Mint_CheckAudio(_THIS, SDL_AudioSpec *spec)
{
	int i;
	unsigned long masterclock, masterprediv;

	DEBUG_PRINT((DEBUG_NAME "asked: %d bits, ",spec->format & 0x00ff));
	DEBUG_PRINT(("signed=%d, ", ((spec->format & 0x8000)!=0)));
	DEBUG_PRINT(("big endian=%d, ", ((spec->format & 0x1000)!=0)));
	DEBUG_PRINT(("channels=%d, ", spec->channels));
	DEBUG_PRINT(("freq=%d\n", spec->freq));

	/* Check formats available */
	MINTAUDIO_nfreq=4;
	MINTAUDIO_sfreq=0;
	switch(cookie_mcsn->play) {
		case MCSN_ST:
			spec->channels=1;
			spec->format=8; /* FIXME: is it signed or unsigned ? */
			MINTAUDIO_nfreq=1;
			MINTAUDIO_hardfreq[0]=12500;
			break;
		case MCSN_TT:	/* Also STE, Mega STE */
			spec->format=AUDIO_S8;
			masterclock=MASTERCLOCK_STE;
			masterprediv=MASTERPREDIV_STE;
			if ((cookie_mch>>16)==MCH_TT) {
				masterclock=MASTERCLOCK_TT;
				masterprediv=MASTERPREDIV_TT;
			}
			for (i=MINTAUDIO_sfreq;i<MINTAUDIO_nfreq;i++) {
				MINTAUDIO_hardfreq[i]=masterclock/(masterprediv*(1<<i));
				DEBUG_PRINT((DEBUG_NAME "calc:freq(%d)=%lu\n", i, MINTAUDIO_hardfreq[i]));
			}
			break;
		case MCSN_FALCON:	/* Also Mac */
			MINTAUDIO_nfreq=12;
			MINTAUDIO_sfreq=1;
			masterclock=MASTERCLOCK_FALCON1;
			if (cookie_mcsn->res1 != 0) {
				masterclock=cookie_mcsn->res1;
			}
			for (i=MINTAUDIO_sfreq;i<MINTAUDIO_nfreq;i++) {
				MINTAUDIO_hardfreq[i]=masterclock/(MASTERPREDIV_FALCON*(i+1));
				DEBUG_PRINT((DEBUG_NAME "calc:freq(%d)=%lu\n", i, MINTAUDIO_hardfreq[i]));
			}
			spec->format |= 0x8000;	/* Audio is always signed */
			if ((spec->format & 0x00ff)==16) {
				spec->format |= 0x1000;	/* Audio is always big endian */
				spec->channels=2;	/* 16 bits always stereo */
			}
			break;
	}

	MINTAUDIO_numfreq=SDL_MintAudio_SearchFrequency(this, (cookie_mch>>16)==MCH_F30, spec->freq);
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
	int channels_mode;
	void *buffer;

	/* Stop currently playing sound */
	Buffoper(0);

	/* Set replay tracks */
	Settracks(0,0);
	Setmontracks(0);

	/* Select replay format */
	channels_mode=STEREO16;
	switch (spec->format & 0xff) {
		case 8:
			if (spec->channels==2) {
				channels_mode=STEREO8;
			} else {
				channels_mode=MONO8;
			}
			break;
	}
	if (Setmode(channels_mode)<0) {
		DEBUG_PRINT((DEBUG_NAME "Setmode() failed\n"));
	}

	switch(cookie_mcsn->play) {
		case MCSN_TT:
			Devconnect(DMAPLAY, DAC, CLK25M, CLKOLD, 1);
			Soundcmd(SETPRESCALE, 3-MINTAUDIO_numfreq);
			DEBUG_PRINT((DEBUG_NAME "STE/TT prescaler selected\n"));
			break;
		case MCSN_FALCON:
			if (cookie_mcsn->res1 != 0) {
				Devconnect(DMAPLAY, DAC, CLKEXT, MINTAUDIO_numfreq, 1);
				DEBUG_PRINT((DEBUG_NAME "External clock selected, prescaler %d\n", MINTAUDIO_numfreq));
			} else {
				Devconnect(DMAPLAY, DAC, CLK25M, MINTAUDIO_numfreq, 1);
				DEBUG_PRINT((DEBUG_NAME "25.175 MHz clock selected, prescaler %d\n", MINTAUDIO_numfreq));
			}
			break;
	}

	/* Set buffer */
	buffer = SDL_MintAudio_audiobuf[SDL_MintAudio_numbuf];
	if (Setbuffer(0, buffer, buffer + spec->size)<0) {
		DEBUG_PRINT((DEBUG_NAME "Setbuffer() failed\n"));
	}
	
	/* Install interrupt */
	Jdisint(MFP_DMASOUND);
	Xbtimer(XB_TIMERA, 8, 1, SDL_MintAudio_XbiosInterrupt);
	Jenabint(MFP_DMASOUND);

	if (Setinterrupt(SI_TIMERA, SI_PLAY)<0) {
		DEBUG_PRINT((DEBUG_NAME "Setinterrupt() failed\n"));
	}

	/* Go */
	Buffoper(SB_PLA_ENA|SB_PLA_RPT);
	DEBUG_PRINT((DEBUG_NAME "hardware initialized\n"));
}

static int Mint_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	/* Lock sound system */
	if (Locksnd()!=1) {
   	    SDL_SetError("Mint_OpenAudio: Audio system already in use");
        return(-1);
	}

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
