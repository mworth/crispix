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
 "@(#) $Id: SDL_fbvideo.h,v 1.1 2003/09/16 04:36:51 gmaynard Exp $";
#endif

#ifndef _SDL_fbvideo_h
#define _SDL_fbvideo_h

#include <sys/types.h>
#include <termios.h>
#include <linux/fb.h>

#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "SDL_sysvideo.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this


/* This is the structure we use to keep track of video memory */
typedef struct vidmem_bucket {
	struct vidmem_bucket *prev;
	int used;
	int dirty;
	char *base;
	unsigned int size;
	struct vidmem_bucket *next;
} vidmem_bucket;

/* Private display data */
struct SDL_PrivateVideoData {
	int console_fd;
	struct fb_var_screeninfo cache_vinfo;
	struct fb_var_screeninfo saved_vinfo;
	int saved_cmaplen;
	__u16 *saved_cmap;

	int current_vt;
	int saved_vt;
	int keyboard_fd;
	int saved_kbd_mode;
	struct termios saved_kbd_termios;

	int mouse_fd;

	char *mapped_mem;
	int mapped_memlen;
	int mapped_offset;
	char *mapped_io;
	long mapped_iolen;
	int flip_page;
	char *flip_address[2];

#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
	int SDL_nummodes[NUM_MODELISTS];
	SDL_Rect **SDL_modelist[NUM_MODELISTS];

	vidmem_bucket surfaces;
	int surfaces_memtotal;
	int surfaces_memleft;

	SDL_mutex *hw_lock;

	void (*wait_vbl)(_THIS);
	void (*wait_idle)(_THIS);
};
/* Old variable names */
#define console_fd		(this->hidden->console_fd)
#define current_vt		(this->hidden->current_vt)
#define saved_vt		(this->hidden->saved_vt)
#define keyboard_fd		(this->hidden->keyboard_fd)
#define saved_kbd_mode		(this->hidden->saved_kbd_mode)
#define saved_kbd_termios	(this->hidden->saved_kbd_termios)
#define mouse_fd		(this->hidden->mouse_fd)
#define cache_vinfo		(this->hidden->cache_vinfo)
#define saved_vinfo		(this->hidden->saved_vinfo)
#define saved_cmaplen		(this->hidden->saved_cmaplen)
#define saved_cmap		(this->hidden->saved_cmap)
#define mapped_mem		(this->hidden->mapped_mem)
#define mapped_memlen		(this->hidden->mapped_memlen)
#define mapped_offset		(this->hidden->mapped_offset)
#define mapped_io		(this->hidden->mapped_io)
#define mapped_iolen		(this->hidden->mapped_iolen)
#define flip_page		(this->hidden->flip_page)
#define flip_address		(this->hidden->flip_address)
#define SDL_nummodes		(this->hidden->SDL_nummodes)
#define SDL_modelist		(this->hidden->SDL_modelist)
#define surfaces		(this->hidden->surfaces)
#define surfaces_memtotal	(this->hidden->surfaces_memtotal)
#define surfaces_memleft	(this->hidden->surfaces_memleft)
#define hw_lock			(this->hidden->hw_lock)
#define wait_vbl		(this->hidden->wait_vbl)
#define wait_idle		(this->hidden->wait_idle)

/* Accelerator types that are supported by the driver, but are not
   necessarily in the kernel headers on the system we compile on.
*/
#ifndef FB_ACCEL_MATROX_MGAG400
#define FB_ACCEL_MATROX_MGAG400	26	/* Matrox G400			*/
#endif
#ifndef FB_ACCEL_3DFX_BANSHEE
#define FB_ACCEL_3DFX_BANSHEE	31	/* 3Dfx Banshee			*/
#endif

/* These functions are defined in SDL_fbvideo.c */
extern void FB_SavePaletteTo(_THIS, int palette_len, __u16 *area);
extern void FB_RestorePaletteFrom(_THIS, int palette_len, __u16 *area);

/* These are utility functions for working with video surfaces */

static __inline__ void FB_AddBusySurface(SDL_Surface *surface)
{
	((vidmem_bucket *)surface->hwdata)->dirty = 1;
}

static __inline__ int FB_IsSurfaceBusy(SDL_Surface *surface)
{
	return ((vidmem_bucket *)surface->hwdata)->dirty;
}

static __inline__ void FB_WaitBusySurfaces(_THIS)
{
	vidmem_bucket *bucket;

	/* Wait for graphic operations to complete */
	wait_idle(this);

	/* Clear all surface dirty bits */
	for ( bucket=&surfaces; bucket; bucket=bucket->next ) {
		bucket->dirty = 0;
	}
}

static __inline__ void FB_dst_to_xy(_THIS, SDL_Surface *dst, int *x, int *y)
{
	*x = (long)((char *)dst->pixels - mapped_mem)%this->screen->pitch;
	*y = (long)((char *)dst->pixels - mapped_mem)/this->screen->pitch;
	if ( dst == this->screen ) {
		*x += this->offset_x;
		*y += this->offset_y;
	}
}

#endif /* _SDL_fbvideo_h */
