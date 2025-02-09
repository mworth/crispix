/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999  Sam Lantinga

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
 "@(#) $Id: SDL_sysmouse.c,v 1.1 2003/09/16 04:36:54 gmaynard Exp $";
#endif

#include <stdlib.h>
#include <windows.h>

#include "SDL_error.h"
#include "SDL_mouse.h"
#include "SDL_sysmouse_c.h"
#include "SDL_events_c.h"
#include "SDL_cursor_c.h"
#include "SDL_lowvideo.h"

#ifdef _WIN32_WCE
#define USE_STATIC_CURSOR
#endif

HCURSOR	SDL_hcursor = NULL;		/* Exported for SDL_eventloop.c */

/* The implementation dependent data for the window manager cursor */
/* For some reason when creating a windows cursor, the ands and xors memory
   is not copied, so we need to keep track of it and free it when we are done
   with the cursor.  If we free the memory prematurely, the app crashes. :-}
*/
struct WMcursor {
	HCURSOR curs;
#ifndef USE_STATIC_CURSOR
	Uint8 *ands;
	Uint8 *xors;
#endif
};

/* Convert bits to padded bytes */
#define PAD_BITS(bits)	((bits+7)/8)

#ifdef CURSOR_DEBUG
static void PrintBITMAP(FILE *out, char *bits, int w, int h)
{
	int i;
	unsigned char ch;

	while ( h-- > 0 ) {
		for ( i=0; i<w; ++i ) {
			if ( (i%8) == 0 )
				ch = *bits++;
			if ( ch&0x80 )
				fprintf(out, "X");
			else
				fprintf(out, " ");
			ch <<= 1;
		}
		fprintf(out, "\n");
	}
}
#endif

#ifndef USE_STATIC_CURSOR
/* Local functions to convert the SDL cursor mask into Windows format */
static void memnot(Uint8 *dst, Uint8 *src, int len)
{
	while ( len-- > 0 )
		*dst++ = ~*src++;
}
static void memxor(Uint8 *dst, Uint8 *src1, Uint8 *src2, int len)
{
	while ( len-- > 0 )
		*dst++ = (*src1++)^(*src2++);
}
#endif /* !USE_STATIC_CURSOR */

void WIN_FreeWMCursor(_THIS, WMcursor *cursor)
{
#ifndef USE_STATIC_CURSOR
	if ( cursor->curs == GetCursor() )
		SetCursor(NULL);
	if ( cursor->curs != NULL )
		DestroyCursor(cursor->curs);
	if ( cursor->ands != NULL )
		free(cursor->ands);
	if ( cursor->xors != NULL )
		free(cursor->xors);
#endif /* !USE_STATIC_CURSOR */
	free(cursor);
}

WMcursor *WIN_CreateWMCursor(_THIS,
		Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y)
{
#ifdef USE_STATIC_CURSOR
	WMcursor *cursor;

	/* Allocate the cursor */
	cursor = (WMcursor *)malloc(sizeof(*cursor));
	if ( cursor ) {
		cursor->curs = LoadCursor(NULL, IDC_ARROW);
	}
	return(cursor);
#else
	WMcursor *cursor;
	int allowed_x;
	int allowed_y;
	int run, pad, i;
	Uint8 *aptr, *xptr;

	/* Check to make sure the cursor size is okay */
	allowed_x = GetSystemMetrics(SM_CXCURSOR);
	allowed_y = GetSystemMetrics(SM_CYCURSOR);
	if ( (w > allowed_x) || (h > allowed_y) ) {
		SDL_SetError("Only cursors of dimension (%dx%d) are allowed",
							allowed_x, allowed_y);
		return(NULL);
	}

	/* Allocate the cursor */
	cursor = (WMcursor *)malloc(sizeof(*cursor));
	if ( cursor == NULL ) {
		SDL_SetError("Out of memory");
		return(NULL);
	}
	cursor->curs = NULL;
	cursor->ands = NULL;
	cursor->xors = NULL;

	/* Pad out to the normal cursor size */
	run = PAD_BITS(w);
	pad = PAD_BITS(allowed_x)-run;
	aptr = cursor->ands = (Uint8 *)malloc((run+pad)*allowed_y);
	xptr = cursor->xors = (Uint8 *)malloc((run+pad)*allowed_y);
	if ( (aptr == NULL) || (xptr == NULL) ) {
		WIN_FreeWMCursor(NULL, cursor);
		SDL_OutOfMemory();
		return(NULL);
	}
	for ( i=0; i<h; ++i ) {
		memxor(xptr, data, mask, run);
		xptr += run;
		data += run;
		memnot(aptr, mask, run);
		mask += run;
		aptr += run;
		memset(xptr,  0, pad);
		xptr += pad;
		memset(aptr, ~0, pad);
		aptr += pad;
	}
	pad += run;
	for ( ; i<allowed_y; ++i ) {
		memset(xptr,  0, pad);
		xptr += pad;
		memset(aptr, ~0, pad);
		aptr += pad;
	}

	/* Create the cursor */
	cursor->curs = CreateCursor(
			(HINSTANCE)GetWindowLong(SDL_Window, GWL_HINSTANCE),
					hot_x, hot_y, allowed_x, allowed_y, 
						cursor->ands, cursor->xors);
	if ( cursor->curs == NULL ) {
		WIN_FreeWMCursor(NULL, cursor);
		SDL_SetError("Windows couldn't create the requested cursor");
		return(NULL);
	}
	return(cursor);
#endif /* USE_STATIC_CURSOR */
}

int WIN_ShowWMCursor(_THIS, WMcursor *cursor)
{
	POINT mouse_pos;

	/* The fullscreen cursor must be done in software with DirectInput */
	if ( !this->screen || DDRAW_FULLSCREEN() ) {
		return(0);
	}

	/* Set the window cursor to our cursor, if applicable */
	if ( cursor != NULL ) {
		SDL_hcursor = cursor->curs;
	} else {
		SDL_hcursor = NULL;
	}
	GetCursorPos(&mouse_pos);
	if ( PtInRect(&SDL_bounds, mouse_pos) ) {
		SetCursor(SDL_hcursor);
	}
	return(1);
}

void WIN_WarpWMCursor(_THIS, Uint16 x, Uint16 y)
{
	if ( DDRAW_FULLSCREEN() ) {
		SDL_PrivateMouseMotion(0, 0, x, y);
	} else if ( mouse_relative) {
		/*	RJR: March 28, 2000
			leave physical cursor at center of screen if
			mouse hidden and grabbed */
		SDL_PrivateMouseMotion(0, 0, x, y);
	} else {
		POINT pt;
		pt.x = x;
		pt.y = y;
		ClientToScreen(SDL_Window, &pt);
		SetCursorPos(pt.x, pt.y);
	}
}

/* Update the current mouse state and position */
void WIN_UpdateMouse(_THIS)
{
	RECT rect;
	POINT pt;

	if ( ! DDRAW_FULLSCREEN() ) {
		GetClientRect(SDL_Window, &rect);
		GetCursorPos(&pt);
		MapWindowPoints(NULL, SDL_Window, &pt, 1);
		if (PtInRect(&rect, pt) && (WindowFromPoint(pt) == SDL_Window)){
			SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
			SDL_PrivateMouseMotion(0,0, (Sint16)pt.x, (Sint16)pt.y);
		} else {
			SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
		}
	}
}

/* Check to see if we need to enter or leave mouse relative mode */
void WIN_CheckMouseMode(_THIS)
{
        /* If the mouse is hidden and input is grabbed, we use relative mode */
        if ( !(SDL_cursorstate & CURSOR_VISIBLE) &&
             (this->input_grab != SDL_GRAB_OFF) ) {
                mouse_relative = 1;
        } else {
                mouse_relative = 0;
        }
}
