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
 "@(#) $Id: SDL_dx5video.h,v 1.1 2003/09/16 04:36:54 gmaynard Exp $";
#endif

#ifndef _SDL_dx5video_h
#define _SDL_dx5video_h

#include "directx.h"

/* Private display data */
struct SDL_PrivateVideoData {
    LPDIRECTDRAW2 ddraw2;
    LPDIRECTDRAWSURFACE3 SDL_primary;
    LPDIRECTDRAWCLIPPER SDL_clipper;
    LPDIRECTDRAWPALETTE SDL_palette;
    PALETTEENTRY SDL_colors[256];
    int colorchange_expected;

#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
    int SDL_nummodes[NUM_MODELISTS];
    SDL_Rect **SDL_modelist[NUM_MODELISTS];
    int SDL_modeindex[NUM_MODELISTS];
};
/* Old variable names */
#define ddraw2			(this->hidden->ddraw2)
#define SDL_primary		(this->hidden->SDL_primary)
#define SDL_clipper		(this->hidden->SDL_clipper)
#define SDL_palette		(this->hidden->SDL_palette)
#define SDL_colors		(this->hidden->SDL_colors)
#define colorchange_expected	(this->hidden->colorchange_expected)
#define SDL_nummodes		(this->hidden->SDL_nummodes)
#define SDL_modelist		(this->hidden->SDL_modelist)
#define SDL_modeindex		(this->hidden->SDL_modeindex)

/* DirectX function pointers for video and events */
extern HRESULT (WINAPI *DDrawCreate)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );
extern HRESULT (WINAPI *DInputCreate)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUT *ppDI, LPUNKNOWN punkOuter);

/* DirectDraw error reporting function */
extern void SetDDerror(const char *function, int code);

#endif /* _SDL_dx5video_h */
