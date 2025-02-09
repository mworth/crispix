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
 "@(#) $Id: SDL_dx5yuv_c.h,v 1.1 2003/09/16 04:36:54 gmaynard Exp $";
#endif

/* This is the DirectDraw implementation of YUV video overlays */

#include "SDL_video.h"
#include "SDL_lowvideo.h"
#include "SDL_dx5video.h"

extern SDL_Overlay *DX5_CreateYUVOverlay(_THIS, int width, int height, Uint32 format, SDL_Surface *display);

extern int DX5_LockYUVOverlay(_THIS, SDL_Overlay *overlay);

extern void DX5_UnlockYUVOverlay(_THIS, SDL_Overlay *overlay);

extern int DX5_DisplayYUVOverlay(_THIS, SDL_Overlay *overlay, SDL_Rect *dstrect);

extern void DX5_FreeYUVOverlay(_THIS, SDL_Overlay *overlay);
