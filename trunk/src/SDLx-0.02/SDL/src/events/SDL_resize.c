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
 "@(#) $Id: SDL_resize.c,v 1.1 2003/08/06 04:26:18 chrisdanford Exp $";
#endif

/* Resize event handling code for SDL */

#include "SDL_events.h"
#include "SDL_events_c.h"
#include "SDL_sysvideo.h"


/* Keep the last resize event so we don't post duplicates */
static struct {
	int w;
	int h;
} last_resize;

/* This is global for SDL_eventloop.c */
int SDL_PrivateResize(int w, int h)
{
	int posted;
	SDL_Event events[32];

	/* See if this event would change the video surface */
	if ( !w || !h ||
	     ((last_resize.w == w) && (last_resize.h == h)) ) {
		return(0);
	}
        last_resize.w = w;
        last_resize.h = h;
	if ( ! SDL_VideoSurface ||
	     ((w == SDL_VideoSurface->w) && (h == SDL_VideoSurface->h)) ) {
		return(0);
	}

	/* Pull out all old resize events */
	SDL_PeepEvents(events, sizeof(events)/sizeof(events[0]),
	                    SDL_GETEVENT, SDL_VIDEORESIZEMASK);

	/* Post the event, if desired */
	posted = 0;
	if ( SDL_ProcessEvents[SDL_VIDEORESIZE] == SDL_ENABLE ) {
		SDL_Event event;
		event.type = SDL_VIDEORESIZE;
		event.resize.w = w;
		event.resize.h = h;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
	return(posted);
}
