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
 "@(#) $Id: SDL_BeApp.h,v 1.1 2003/09/16 04:36:45 gmaynard Exp $";
#endif

/* Handle the BeApp specific portions of the application */

/* Initialize the Be Application, if it's not already started */
extern int SDL_InitBeApp(void);

/* Quit the Be Application, if there's nothing left to do */
extern void SDL_QuitBeApp(void);

/* Flag to tell whether the app is active or not */
extern int SDL_BeAppActive;
