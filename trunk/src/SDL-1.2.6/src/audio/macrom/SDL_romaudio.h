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
 "@(#) $Id: SDL_romaudio.h,v 1.1 2003/09/16 04:36:40 gmaynard Exp $";
#endif

#ifndef _SDL_romaudio_h
#define _SDL_romaudio_h

#include "SDL_sysaudio.h"

/* This is Ryan's improved MacOS sound code, with locking support */
#define USE_RYANS_SOUNDCODE

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_AudioDevice *this

struct SDL_PrivateAudioData {
	/* Sound manager audio channel */
	SndChannelPtr channel;
#if defined(TARGET_API_MAC_CARBON) || defined(USE_RYANS_SOUNDCODE)
	/* FIXME: Add Ryan's static data here */
#else
	/* Double buffering variables */
	SndDoubleBufferPtr audio_buf[2];
#endif
};

/* Old variable names */
#define channel		(this->hidden->channel)
#define audio_buf	(this->hidden->audio_buf)

#endif /* _SDL_romaudio_h */
