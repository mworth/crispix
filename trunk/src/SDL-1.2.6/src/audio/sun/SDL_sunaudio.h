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
 "@(#) $Id: SDL_sunaudio.h,v 1.1 2003/09/16 04:36:41 gmaynard Exp $";
#endif

#ifndef _SDL_lowaudio_h
#define _SDL_lowaudio_h

#include "SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_AudioDevice *this

struct SDL_PrivateAudioData {
	/* The file descriptor for the audio device */
	int audio_fd;

	Uint16 audio_fmt;        /* The app audio format */
	Uint8 *mixbuf;           /* The app mixing buffer */
	int ulaw_only;           /* Flag -- does hardware only output U-law? */
	Uint8 *ulaw_buf;         /* The U-law mixing buffer */
	Sint32 written;          /* The number of samples written */
	int fragsize;            /* The audio fragment size in samples */
	int frequency;           /* The audio frequency in KHz */
};

/* Old variable names */
#define audio_fd		(this->hidden->audio_fd)
#define audio_fmt		(this->hidden->audio_fmt)
#define mixbuf			(this->hidden->mixbuf)
#define ulaw_only		(this->hidden->ulaw_only)
#define ulaw_buf		(this->hidden->ulaw_buf)
#define written			(this->hidden->written)
#define fragsize		(this->hidden->fragsize)
#define frequency		(this->hidden->frequency)

#endif /* _SDL_lowaudio_h */
