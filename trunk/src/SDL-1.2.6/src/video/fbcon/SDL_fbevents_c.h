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
 "@(#) $Id: SDL_fbevents_c.h,v 1.1 2003/09/16 04:36:51 gmaynard Exp $";
#endif

#include "SDL_fbvideo.h"

/* Variables and functions exported by SDL_sysevents.c to other parts 
   of the native video subsystem (SDL_sysvideo.c)
*/
extern int FB_OpenKeyboard(_THIS);
extern void FB_CloseKeyboard(_THIS);
extern int FB_OpenMouse(_THIS);
extern void FB_CloseMouse(_THIS);
extern int FB_EnterGraphicsMode(_THIS);
extern int FB_InGraphicsMode(_THIS);
extern void FB_LeaveGraphicsMode(_THIS);

extern void FB_InitOSKeymap(_THIS);
extern void FB_PumpEvents(_THIS);
