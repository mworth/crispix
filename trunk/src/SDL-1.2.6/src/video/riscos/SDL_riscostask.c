/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001  Sam Lantinga

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
    slouken@devolution.com
*/

/*
    This file added by Alan Buckley (alan_baa@hotmail.com) to support RISCOS 
	26 March 2003

	File includes routines for:
	  Setting up as a WIMP Task
	  Reading information about the current desktop
	  Storing information before a switch to full screen
	  Restoring desktop after switching to full screen
*/

#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "swis.h"

#include "SDL_riscostask.h"

/* RISCOS variables */

static int task_handle = 0;
static int wimp_version = 0;

/* RISC OS variables to help compatability with certain programs */
int riscos_backbuffer = 0; /* Create a back buffer in system memory for full screen mode */
int riscos_closeaction = 1; /* Close icon action */
int riscos_audiobuffer = 0; /* Audio buffer size */

static int stored_mode = -1; /* -1 when in desktop, mode number or pointer when full screen */

extern int mouseInWindow; /* Mouse is in WIMP window */

/* Local function */

static int RISCOS_GetTaskName(char *task_name);

/* Uncomment next line to copy mode changes/restores to stderr */
/* #define DUMP_MODE */
#ifdef DUMP_MODE
#include "stdio.h"
static void dump_mode()
{
    fprintf(stderr, "mode %d\n", stored_mode);
    if (stored_mode < -1 || stored_mode >= 256)
    {
        int blockSize = 0;
		int *storeBlock = (int *)stored_mode;

        while(blockSize < 5 || storeBlock[blockSize] != -1)
        {
           fprintf(stderr, "   %d\n", storeBlock[blockSize++]);
        }
    }
}
#endif

/******************************************************************

 Initialise as RISCOS Wimp task

*******************************************************************/

int RISCOS_InitTask()
{
   char task_name[32];
   _kernel_swi_regs regs;
   int messages[4];

   if (RISCOS_GetTaskName(task_name) == 0) return 0;

   messages[0] = 9;       /* Palette changed */
   messages[1] = 0x400c1; /* Mode changed */
   messages[2] = 8;       /* Pre quit */
   messages[2] = 0;
   
	regs.r[0] = (unsigned int)360; /* Minimum version 3.6 */
	regs.r[1] = (unsigned int)0x4b534154;
	regs.r[2] = (unsigned int)task_name;
	regs.r[3] = (unsigned int)messages;

   if (_kernel_swi(Wimp_Initialise, &regs, &regs) == 0)
   {
	   wimp_version = regs.r[0];
	   task_handle = regs.r[1];
	   return 1;
   }

   return 0;
}

/*********************************************************************

  Close down application on exit.

**********************************************************************/

void RISCOS_ExitTask()
{
	_kernel_swi_regs regs;

    if (stored_mode == -1)
    {
       /* Ensure cursor is put back to standard pointer shape if
          we have been running in a window */
       _kernel_osbyte(106,1,0);
    }

	/* Ensure we end up back in the wimp */
	RISCOS_RestoreWimpMode();

	/* Neatly exit the task */
   	regs.r[0] = task_handle;
   	regs.r[1] = (unsigned int)0x4b534154;
   	_kernel_swi(Wimp_CloseDown, &regs, &regs);
	task_handle = 0;
}

/**************************************************************************

  Get the name of the task for the desktop.

  Param:   task_name - name of task 32 characters.

  Returns: 1 is successful, otherwise 0

  Notes:   Works by getting using OS_GetEnv to get the command line
		   used to run the program and then parsing a name from it
		   as follows.

		   1. Use name after final period if not !RunImage
		   2. If name is !RunImage then process item before the period
		      in front of !RunImage.
		   3. If directory name use that
		   4. if in form <XXX$Dir> use the XXX.

		   Finally once this value has been retrieved use it unless
		   there is a variable set up in the form SDL$<name>$TaskName
		   in which case the value of this variable will be used.

		   Now also gets other riscos configuration varibles
                SDL$<name>$BackBuffer - set to 1 to use a system memory backbuffer in fullscreen mode
						    so updates wait until a call to SDL_UpdateRects. (default 0)
						    This is required for programmes where they have assumed this is
						    always the case which is contrary to the documentation.
               SDL$<name>$CloseAction
                    0 Don't show close icon
                    1 Show close icon

               SDL$<name>$AudioBuffer - set to number of samples to buffer
                    in advance. Will default to a minimum of 1024 or twice
                    amount requested by program whichever is largest.
                    If not specified default is amount for 10 csecs.
                    Time that will be pre-buffered can be calculated as
                    sample to buffer * 1000 / freq milliseconds.
                    
***************************************************************************/

int RISCOS_GetTaskName(char *task_name)
{
	_kernel_swi_regs regs;

   task_name[0] = 0;

   /* Figure out a sensible task name */
   if (_kernel_swi(OS_GetEnv, &regs, &regs) == 0)
   {
	   char *command_line = (char *)regs.r[0];
	   char *buffer = malloc(strlen(command_line)+1);
	   char *env_var;
	   char *p;

	   strcpy(buffer, command_line);
	   p = strchr(buffer, ' ');
	   if (p) *p = 0;
	   p = strrchr(buffer, '.');
	   if (p == 0) p = buffer;
	   if (stricmp(p+1,"!RunImage") == 0)
	   {
		   *p = 0;
	   	   p = strrchr(buffer, '.');
		   if (p == 0) p = buffer;
	   }
	   if (*p == '.') p++;
	   if (*p == '!') p++; /* Skip "!" at beginning of application directories */

       if (*p == '<')
       {
          // Probably in the form <appname$Dir>
          char *q = strchr(p, '$');
          if (q == 0) q = strchr(p,'>'); /* Use variable name if not */
          if (q) *q = 0;
          p++; /* Move over the < */
       }

	   if (*p)
	   {
		   /* Read variables that effect the RISCOS SDL engine for this task */
		   env_var = malloc(strlen(p) + 18); /* 18 is larger than the biggest variable name */
		   if (env_var)
		   {
			   char *env_val;

			   /* See if a variable of form SDL$<dirname>$TaskName exists */

			   strcpy(env_var, "SDL$");
			   strcat(env_var, p);
			   strcat(env_var, "$TaskName");

			   env_val = getenv(env_var);
			   if (env_val) strncpy(task_name, env_val, 31);

			   strcpy(env_var, "SDL$");
			   strcat(env_var, p);
			   strcat(env_var, "$BackBuffer");

			   env_val = getenv(env_var);
			   if (env_val && strcmp(env_val,"1") == 0) riscos_backbuffer = 1;

			   strcpy(env_var, "SDL$");
			   strcat(env_var, p);
			   strcat(env_var, "$CloseAction");

			   env_val = getenv(env_var);
			   if (env_val && strcmp(env_val,"0") == 0) riscos_closeaction = 0;

			   strcpy(env_var, "SDL$");
			   strcat(env_var, p);
			   strcat(env_var, "$AudioBuffer");

			   env_val = getenv(env_var);
			   riscos_audiobuffer = atoi(env_val);

			   free(env_var);
		   }
		   
		   if (task_name[0] == 0) strncpy(task_name, p, 31);
		   task_name[31] = 0;
	   }

	   free(buffer);
   }

   if (task_name[0] == 0) strcpy(task_name, "SDL Task");

   return 1;
}

/*****************************************************************

  Store the current desktop screen mode if we are in the desktop.

******************************************************************/

void RISCOS_StoreWimpMode()
{
     _kernel_swi_regs regs;

	/* Don't store if in full screen mode */
	if (stored_mode != -1) return;

    regs.r[0] = 1;
    _kernel_swi(OS_ScreenMode, &regs, &regs);
    if (regs.r[1] >= 0 && regs.r[1] < 256) stored_mode = regs.r[1];
    else
    {
        int blockSize = 0;
        int *retBlock = (int *)regs.r[1];
		int *storeBlock;
        int j;

        while(blockSize < 5 || retBlock[blockSize] != -1) blockSize++;
        blockSize++;
        storeBlock = (int *)malloc(blockSize * sizeof(int));
        retBlock = (int *)regs.r[1];
        for ( j = 0; j < blockSize; j++)
           storeBlock[j] = retBlock[j];

		stored_mode = (int)storeBlock;
     }
#if DUMP_MODE
    fprintf(stderr, "Stored "); dump_mode();
#endif
}

/*****************************************************************

  Restore desktop screen mode if we are in full screen mode.

*****************************************************************/

void RISCOS_RestoreWimpMode()
{
    _kernel_swi_regs regs;

	/* Only need to restore if we are in full screen mode */
	if (stored_mode == -1) return;

#if DUMP_MODE
   fprintf(stderr, "Restored"); dump_mode();
#endif

    regs.r[0] = stored_mode;
    _kernel_swi(Wimp_SetMode, &regs, &regs);
    if (stored_mode < 0 || stored_mode > 256)
    {
       free((int *)stored_mode);
    }
    stored_mode = -1;

    /* Flush keyboard buffer to dump the keystrokes we've already polled */
    regs.r[0] = 21;
    regs.r[1] = 0; /* Keyboard buffer number */
    _kernel_swi(OS_Byte, &regs, &regs);

    mouseInWindow = 0;

}

/*********************************************************************

  Get version of Wimp running when task was initialised.

*********************************************************************/

int RISCOS_GetWimpVersion()
{
	return wimp_version;
}

int RISCOS_GetTaskHandle()
{
	return task_handle;
}
