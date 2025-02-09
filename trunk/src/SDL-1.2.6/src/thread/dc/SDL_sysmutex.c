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

    BERO
    bero@geocities.co.jp

    based on generic/SDL_sysmutex.c

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_sysmutex.c,v 1.1 2003/09/16 04:36:47 gmaynard Exp $";
#endif

/* An implementation of mutexes using semaphores */

#include <stdio.h>
#include <stdlib.h>

#include "SDL_error.h"
#include "SDL_thread.h"
#include "SDL_systhread_c.h"

#include <arch/spinlock.h>

struct SDL_mutex {
	int recursive;
	Uint32 owner;
	spinlock_t mutex;
};

/* Create a mutex */
SDL_mutex *SDL_CreateMutex(void)
{
	SDL_mutex *mutex;

	/* Allocate mutex memory */
	mutex = (SDL_mutex *)malloc(sizeof(*mutex));
	if ( mutex ) {
		spinlock_init(&mutex->mutex);
		mutex->recursive = 0;
		mutex->owner = 0;
	} else {
		SDL_OutOfMemory();
	}
	return mutex;
}

/* Free the mutex */
void SDL_DestroyMutex(SDL_mutex *mutex)
{
	if ( mutex ) {
		free(mutex);
	}
}

/* Lock the semaphore */
int SDL_mutexP(SDL_mutex *mutex)
{
#ifdef DISABLE_THREADS
	return 0;
#else
	Uint32 this_thread;

	if ( mutex == NULL ) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	this_thread = SDL_ThreadID();
	if ( mutex->owner == this_thread ) {
		++mutex->recursive;
	} else {
		/* The order of operations is important.
		   We set the locking thread id after we obtain the lock
		   so unlocks from other threads will fail.
		*/
		spinlock_lock(&mutex->mutex);
		mutex->owner = this_thread;
		mutex->recursive = 0;
	}

	return 0;
#endif /* DISABLE_THREADS */
}

/* Unlock the mutex */
int SDL_mutexV(SDL_mutex *mutex)
{
#ifdef DISABLE_THREADS
	return 0;
#else
	if ( mutex == NULL ) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	/* If we don't own the mutex, we can't unlock it */
	if ( SDL_ThreadID() != mutex->owner ) {
		SDL_SetError("mutex not owned by this thread");
		return -1;
	}

	if ( mutex->recursive ) {
		--mutex->recursive;
	} else {
		/* The order of operations is important.
		   First reset the owner so another thread doesn't lock
		   the mutex and set the ownership before we reset it,
		   then release the lock semaphore.
		 */
		mutex->owner = 0;
		spinlock_unlock(&mutex->mutex);
	}
	return 0;
#endif /* DISABLE_THREADS */
}
