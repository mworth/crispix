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

/*
    SDL_systhread.cpp
    Epoc thread management routines for SDL

    Epoc version by Markus Mertama  (w@iki.fi)
*/


//#include <stdlib.h>
//#include <stdio.h>


extern "C" {
#undef NULL
#include "SDL_error.h"
#include "SDL_thread.h"
#include "SDL_systhread.h"
    };

#include <e32std.h>


static int object_count;

int RunThread(TAny* data)
{
	SDL_RunThread(data);
	return(0);
}


TInt NewThread(const TDesC& aName, TAny* aPtr1, TAny* aPtr2)
    {
    return ((RThread*)(aPtr1))->Create(aName,
            RunThread,
            KDefaultStackSize,
            NULL,
            aPtr2);
    }

int CreateUnique(TInt (*aFunc)(const TDesC& aName, TAny*, TAny*), TAny* aPtr1, TAny* aPtr2)
    {
    TBuf<16> name;
    TInt status = KErrNone;
    do
        {
        object_count++;
        name.Format(_L("SDL_%x"), object_count);
        status = aFunc(name, aPtr1, aPtr2);
        }
        while(status == KErrAlreadyExists);
    return status;
    }


int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{
    RThread rthread;
   
    TInt status = CreateUnique(NewThread, &rthread, args);
    if (status != KErrNone) 
    {
        delete(((RThread*)(thread->handle)));
        thread->handle = NULL;
		SDL_SetError("Not enough resources to create thread");
		return(-1);
	}
	rthread.Resume();
    thread->handle = rthread.Handle();
	return(0);
}

void SDL_SYS_SetupThread(void)
{
	return;
}

Uint32 SDL_ThreadID(void)
{
    RThread current;
    TThreadId id = current.Id();
	return id;
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
    RUndertaker taker;
    taker.Create();
    TRequestStatus status;
    taker.Logon(status, thread->handle);
    User::WaitForRequest(status);
    taker.Close();
}

/* WARNING: This function is really a last resort.
 * Threads should be signaled and then exit by themselves.
 * TerminateThread() doesn't perform stack and DLL cleanup.
 */
void SDL_SYS_KillThread(SDL_Thread *thread)
{
    RThread rthread;
    rthread.SetHandle(thread->handle);
	rthread.Kill(0);
	rthread.Close();
}
