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
 "@(#) $Id: SDL_gemdosevents_c.h,v 1.1 2003/09/16 04:36:50 gmaynard Exp $";
#endif

/*
 *	Atari keyboard events manager, using Gemdos
 *
 *	Patrice Mandin
 */

#ifndef _SDL_ATARI_GEMDOSEVENTS_H_
#define _SDL_ATARI_GEMDOSEVENTS_H_

#include "SDL_sysvideo.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this

extern void AtariGemdos_InitOSKeymap(_THIS);
extern void AtariGemdos_PumpEvents(_THIS);
extern void AtariGemdos_ShutdownEvents(void);

#endif /* _SDL_ATARI_GEMDOSEVENTS_H_ */
