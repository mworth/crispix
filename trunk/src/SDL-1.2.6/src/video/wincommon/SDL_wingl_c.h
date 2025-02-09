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
 "@(#) $Id: SDL_wingl_c.h,v 1.1 2003/09/16 04:36:54 gmaynard Exp $";
#endif

/* WGL implementation of SDL OpenGL support */

#include "SDL_sysvideo.h"


struct SDL_PrivateGLData {
    int gl_active; /* to stop switching drivers while we have a valid context */

#ifdef HAVE_OPENGL
    PIXELFORMATDESCRIPTOR GL_pfd;
    HDC GL_hdc;
    HGLRC GL_hrc;
    int pixel_format;
    int wgl_arb_pixel_format;

    void * (WINAPI *wglGetProcAddress)(const char *proc);

    HGLRC (WINAPI *wglCreateContext)(HDC hdc);

    BOOL (WINAPI *wglDeleteContext)(HGLRC hglrc);

    BOOL (WINAPI *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
   
    BOOL (WINAPI *wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList,
                                           const FLOAT *pfAttribFList,
                                           UINT nMaxFormats, int *piFormats,
                                           UINT *nNumFormats);
    BOOL (WINAPI *wglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat,
                                                int iLayerPlane,
                                                UINT nAttributes, 
                                                const int *piAttributes,
                                                int *piValues);
#endif /* HAVE_OPENGL */
};

/* Old variable names */
#define gl_active	(this->gl_data->gl_active)
#define GL_pfd		(this->gl_data->GL_pfd)
#define GL_hdc		(this->gl_data->GL_hdc)
#define GL_hrc		(this->gl_data->GL_hrc)
#define pixel_format	(this->gl_data->pixel_format)

/* OpenGL functions */
extern int WIN_GL_SetupWindow(_THIS);
extern void WIN_GL_ShutDown(_THIS);
#ifdef HAVE_OPENGL
extern int WIN_GL_MakeCurrent(_THIS);
extern int WIN_GL_GetAttribute(_THIS, SDL_GLattr attrib, int* value);
extern void WIN_GL_SwapBuffers(_THIS);
extern void WIN_GL_UnloadLibrary(_THIS);
extern int WIN_GL_LoadLibrary(_THIS, const char* path);
extern void *WIN_GL_GetProcAddress(_THIS, const char* proc);
#endif

#ifdef HAVE_OPENGL

#ifndef WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C
#endif

#ifndef WGL_ARB_multisample
#define WGL_SAMPLE_BUFFERS_ARB         0x2041
#define WGL_SAMPLES_ARB                0x2042
#endif

#endif
