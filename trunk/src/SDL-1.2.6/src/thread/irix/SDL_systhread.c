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
 "@(#) $Id: SDL_systhread.c,v 1.1 2003/09/16 04:36:47 gmaynard Exp $";
#endif

/* IRIX thread management routines for SDL */

#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "SDL_error.h"
#include "SDL_thread.h"
#include "SDL_systhread.h"


static int sig_list[] = {
	SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM, SIGTERM, SIGCLD, SIGWINCH,
	SIGVTALRM, SIGPROF, 0
};


int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{
	/* Create the thread and go! */
	if ( sproc(SDL_RunThread, PR_SALL, args) < 0 ) {
		SDL_SetError("Not enough resources to create thread");
		return(-1);
	}
	return(0);
}

void SDL_SYS_SetupThread(void)
{
	int i;
	sigset_t mask;

	/* Mask asynchronous signals for this thread */
	sigemptyset(&mask);
	for ( i=0; sig_list[i]; ++i ) {
		sigaddset(&mask, sig_list[i]);
	}
	sigprocmask(SIG_BLOCK, &mask, NULL);
}

/* WARNING:  This may not work for systems with 64-bit pid_t */
Uint32 SDL_ThreadID(void)
{
	return((Uint32)getpid());
}

/* WARNING:  This may not work for systems with 64-bit pid_t */
void SDL_WaitThread(SDL_Thread *thread, int *status)
{
	errno = 0;
	while ( errno != ECHILD ) {
		waitpid(thread->handle, NULL, 0);
	}
}

/* WARNING:  This may not work for systems with 64-bit pid_t */
void SDL_KillThread(SDL_Thread *thread)
{
	kill(thread->handle, SIGKILL);
}

