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

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_dibaudio.h,v 1.1 2003/09/16 04:36:41 gmaynard Exp $";
#endif

#ifndef _SDL_lowaudio_h
#define _SDL_lowaudio_h

#include "SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_AudioDevice *this

#define NUM_BUFFERS 2			/* -- Don't lower this! */

struct SDL_PrivateAudioData {
	HWAVEOUT sound;
	HANDLE audio_sem;
	Uint8 *mixbuf;		/* The raw allocated mixing buffer */
	WAVEHDR wavebuf[NUM_BUFFERS];	/* Wave audio fragments */
	int next_buffer;
};

/* Old variable names */
#define sound			(this->hidden->sound)
#define audio_sem 		(this->hidden->audio_sem)
#define mixbuf			(this->hidden->mixbuf)
#define wavebuf			(this->hidden->wavebuf)
#define next_buffer		(this->hidden->next_buffer)

#endif /* _SDL_lowaudio_h */
