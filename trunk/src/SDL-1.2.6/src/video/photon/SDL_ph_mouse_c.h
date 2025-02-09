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

#ifndef __SDL_PH_MOUSE_H__
#define __SDL_PH_MOUSE_H__

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_ph_mouse_c.h,v 1.1 2003/09/16 04:36:52 gmaynard Exp $";
#endif /* SAVE_RCSID */

#include "SDL_ph_video.h"

/* Functions to be exported */
extern void ph_FreeWMCursor(_THIS, WMcursor *cursor);
extern WMcursor *ph_CreateWMCursor(_THIS,
		Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);
extern PhCursorDef_t ph_GetWMPhCursor(WMcursor *cursor);
extern int ph_ShowWMCursor(_THIS, WMcursor *cursor);
extern void ph_WarpWMCursor(_THIS, Uint16 x, Uint16 y);
extern void ph_CheckMouseMode(_THIS);
extern void ph_UpdateMouse(_THIS);

#endif /* __SDL_PH_MOUSE_H__ */
