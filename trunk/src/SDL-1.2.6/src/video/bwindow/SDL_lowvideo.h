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
 "@(#) $Id: SDL_lowvideo.h,v 1.1 2003/09/16 04:36:50 gmaynard Exp $";
#endif

#ifndef _SDL_lowvideo_h
#define _SDL_lowvideo_h

#include "SDL_mouse.h"
#include "SDL_sysvideo.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *_this

/* Private display data */
struct SDL_PrivateVideoData {
	/* The main window */
	SDL_BWin *SDL_Win;

	/* The fullscreen mode list */
	display_mode saved_mode;
#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
	int SDL_nummodes[NUM_MODELISTS];
	SDL_Rect **SDL_modelist[NUM_MODELISTS];

	/* A completely clear cursor */
	WMcursor *BlankCursor;

	/* Mouse state variables */
	uint32 last_buttons;
	BPoint last_point;

	/* Keyboard state variables */
	int key_flip;
	struct key_info keyinfo[2];
};
/* Old variable names */
#define SDL_Win		(_this->hidden->SDL_Win)
#define saved_mode	(_this->hidden->saved_mode)
#define SDL_nummodes	(_this->hidden->SDL_nummodes)
#define SDL_modelist	(_this->hidden->SDL_modelist)
#define SDL_BlankCursor	(_this->hidden->BlankCursor)
#define last_buttons	(_this->hidden->last_buttons)
#define last_point	(_this->hidden->last_point)
#define key_flip	(_this->hidden->key_flip)
#define keyinfo		(_this->hidden->keyinfo)

#endif /* _SDL_lowvideo_h */
