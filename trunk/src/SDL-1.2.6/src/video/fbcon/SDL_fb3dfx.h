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
 "@(#) $Id: SDL_fb3dfx.h,v 1.1 2003/09/16 04:36:51 gmaynard Exp $";
#endif

/* 3Dfx hardware acceleration for the SDL framebuffer console driver */

#include "SDL_fbvideo.h"

/* Set up the driver for 3Dfx acceleration */
extern void FB_3DfxAccel(_THIS, __u32 card);
