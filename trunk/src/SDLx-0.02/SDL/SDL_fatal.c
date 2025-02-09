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
 "@(#) $Id: SDL_fatal.c,v 1.1 2003/08/06 04:26:18 chrisdanford Exp $";
#endif

/* General fatal signal handling code for SDL */

#ifdef NO_SIGNAL_H

/* No signals on this platform, nothing to do.. */

void SDL_InstallParachute(void)
{
	return;
}

void SDL_UninstallParachute(void)
{
	return;
}

#else

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "SDL.h"
#include "SDL_fatal.h"

#ifdef __CYGWIN__
#define DISABLE_STDIO
#endif

/* This installs some signal handlers for the more common fatal signals,
   so that if the programmer is lazy, the app doesn't die so horribly if
   the program crashes.
*/

static void print_msg(const char *text)
{
#ifndef DISABLE_STDIO
	fprintf(stderr, "%s", text);
#endif
}

static void SDL_Parachute(int sig)
{
	signal(sig, SIG_DFL);
	print_msg("Fatal signal: ");
	switch (sig) {
		case SIGSEGV:
			print_msg("Segmentation Fault");
			break;
#ifdef SIGBUS
#if SIGBUS != SIGSEGV
		case SIGBUS:
			print_msg("Bus Error");
			break;
#endif
#endif /* SIGBUS */
#ifdef SIGFPE
		case SIGFPE:
			print_msg("Floating Point Exception");
			break;
#endif /* SIGFPE */
#ifdef SIGQUIT
		case SIGQUIT:
			print_msg("Keyboard Quit");
			break;
#endif /* SIGQUIT */
#ifdef SIGPIPE
		case SIGPIPE:
			print_msg("Broken Pipe");
			break;
#endif /* SIGPIPE */
		default:
#ifndef DISABLE_STDIO
			fprintf(stderr, "# %d", sig);
#endif
			break;
	}
	print_msg(" (SDL Parachute Deployed)\n");
	SDL_Quit();
	exit(-sig);
}

static int SDL_fatal_signals[] = {
	SIGSEGV,
#ifdef SIGBUS
	SIGBUS,
#endif
#ifdef SIGFPE
	SIGFPE,
#endif
#ifdef SIGQUIT
	SIGQUIT,
#endif
#ifdef SIGPIPE
	SIGPIPE,
#endif
	0
};

void SDL_InstallParachute(void)
{
	int i;
	void (*ohandler)(int);

	/* Set a handler for any fatal signal not already handled */
	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SDL_Parachute);
		if ( ohandler != SIG_DFL ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#ifdef SIGALRM
	/* Set SIGALRM to be ignored -- necessary on Solaris */
	{
		struct sigaction action, oaction;

		/* Set SIG_IGN action */
		memset(&action, 0, (sizeof action));
		action.sa_handler = SIG_IGN;
		sigaction(SIGALRM, &action, &oaction);

		/* Reset original action if it was already being handled */
		if ( oaction.sa_handler != SIG_DFL ) {
			sigaction(SIGALRM, &oaction, NULL);
		}
	}
#endif
	return;
}

void SDL_UninstallParachute(void)
{
	int i;
	void (*ohandler)(int);

	/* Remove a handler for any fatal signal handled */
	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SIG_DFL);
		if ( ohandler != SDL_Parachute ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
}

#endif /* NO_SIGNAL_H */

