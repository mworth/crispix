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
 "@(#) $Id: SDL_x11mouse_c.h,v 1.1 2003/09/16 04:36:54 gmaynard Exp $";
#endif

#include "SDL_x11video.h"

/* Functions to be exported */
extern void X11_FreeWMCursor(_THIS, WMcursor *cursor);
extern WMcursor *X11_CreateWMCursor(_THIS,
		Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);
extern int X11_ShowWMCursor(_THIS, WMcursor *cursor);
extern void X11_WarpWMCursor(_THIS, Uint16 x, Uint16 y);
extern void X11_CheckMouseModeNoLock(_THIS);
extern void X11_CheckMouseMode(_THIS);
