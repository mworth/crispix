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


#ifndef __SDL_PH_YUV_H__
#define __SDL_PH_YUV_H__

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_phyuv_c.h,v 1.1 2003/09/16 04:36:52 gmaynard Exp $";
#endif /* SAVE_RCSID */

/* This is the photon implementation of YUV video overlays */

#include "SDL_video.h"
#include "SDL_ph_video.h"

struct private_yuvhwdata
{
    FRAMEDATA* CurrentFrameData;
    FRAMEDATA* FrameData0;
    FRAMEDATA* FrameData1;
    PgScalerProps_t   props;
    PgScalerCaps_t    caps;
    PgVideoChannel_t* channel;
    PhArea_t CurrentViewPort;
    PhPoint_t CurrentWindowPos;
    long format;
    int scaler_on;
    int current;
    long YStride;
    long VStride;
    long UStride;
    int ischromakey;
    long chromakey;
    int forcedredraw;
    unsigned long State;
    long flags;
    int locked;
};

extern SDL_Overlay* ph_CreateYUVOverlay(_THIS, int width, int height, Uint32 format, SDL_Surface* display);
extern int ph_LockYUVOverlay(_THIS, SDL_Overlay* overlay);
extern void ph_UnlockYUVOverlay(_THIS, SDL_Overlay* overlay);
extern int ph_DisplayYUVOverlay(_THIS, SDL_Overlay* overlay, SDL_Rect* dstrect);
extern void ph_FreeYUVOverlay(_THIS, SDL_Overlay* overlay);

#endif /* __SDL_PH_YUV_H__ */
