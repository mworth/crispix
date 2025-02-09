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
 "@(#) $Id: SDL_sysjoystick.c,v 1.1 2003/08/06 04:26:19 chrisdanford Exp $";
#endif

/* This is the system specific header for the SDL joystick API */

#include <xtl.h>
#include <math.h>
#include <stdio.h>		/* For the definition of NULL */
 
#include "SDL_error.h"
#include "SDL_joystick.h"
#include "SDL_sysjoystick.h"
#include "SDL_joystick_c.h"

/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */

#define XBINPUT_DEADZONE 0.24f
#define AXIS_MIN	-32768  /* minimum value for axis coordinate */
#define AXIS_MAX	32767   /* maximum value for axis coordinate */
#define MAX_AXES	4		/* each joystick can have up to 6 axes */
#define MAX_BUTTONS	12		/* and 12 buttons                      */
#define	MAX_HATS	2


extern BOOL                     g_bDevicesInitialized;
typedef struct GamePad
{
    // The following members are inherited from XINPUT_GAMEPAD:
    WORD    wButtons;
    BYTE    bAnalogButtons[8];
    SHORT   sThumbLX;
    SHORT   sThumbLY;
    SHORT   sThumbRX;
    SHORT   sThumbRY;

    // Thumb stick values converted to range [-1,+1]
    FLOAT      fX1;
    FLOAT      fY1;
    FLOAT      fX2;
    FLOAT      fY2;
    
    // State of buttons tracked since last poll
    WORD       wLastButtons;
    BOOL       bLastAnalogButtons[8];
    WORD       wPressedButtons;
    BOOL       bPressedAnalogButtons[8];

    // Rumble properties
    XINPUT_RUMBLE   Rumble;
    XINPUT_FEEDBACK Feedback;

    // Device properties
    XINPUT_CAPABILITIES caps;
    HANDLE     hDevice;

    // Flags for whether game pad was just inserted or removed
    BOOL       bInserted;
    BOOL       bRemoved;
} XBGAMEPAD;

// Global instance of XInput polling parameters
XINPUT_POLLING_PARAMETERS g_PollingParameters = 
{
    TRUE,
    TRUE,
    0,
    8,
    8,
    0,
};

// Global instance of input states
XINPUT_STATE g_InputStates[4];

// Global instance of custom gamepad devices
XBGAMEPAD g_Gamepads[4];

float (xfabsf)(float x)
{
	if (x == 0)
		return x;
	else
		return (x < 0.0F ? -x : x);
}

VOID XBInput_RefreshDeviceList( XBGAMEPAD* pGamepads );

/* The private structure used to keep track of a joystick */
struct joystick_hwdata
{
	XBGAMEPAD	pGamepads[4];

	/* joystick ID */
	UINT	id;

	/* values used to translate device-specific coordinates into
	   SDL-standard ranges */
	struct _transaxis {
		int offset;
		float scale;
	} transaxis[6];
};


int SDL_SYS_JoystickInit(void)
{
	SDL_numjoysticks = 1;
	return(SDL_numjoysticks);
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	return("XBOX Gamepad Plugin V0.01");
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{

	DWORD i = 0;
	DWORD b = 0;
	DWORD dwDeviceMask;

	if (!g_bDevicesInitialized)
		XInitDevices(0 ,NULL);

	g_bDevicesInitialized = TRUE;

	dwDeviceMask = XGetDevices( XDEVICE_TYPE_GAMEPAD );

	joystick->hwdata = (struct joystick_hwdata *) malloc(sizeof(*joystick->hwdata));

	joystick->nbuttons = MAX_BUTTONS;
	joystick->naxes = MAX_AXES;
	joystick->nhats = MAX_HATS;
	joystick->name = "Xbox SDL Gamepad V0.02";

	for(  i=0; i < XGetPortCount(); i++ )
    {
        ZeroMemory( &g_InputStates[i], sizeof(XINPUT_STATE) );
        ZeroMemory( &joystick->hwdata->pGamepads[i], sizeof(XBGAMEPAD) );
        if( dwDeviceMask & (1<<i) ) 
        {
            // Get a handle to the device
            joystick->hwdata->pGamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, 
                                                XDEVICE_NO_SLOT, &g_PollingParameters );

            // Store capabilities of the device
            XInputGetCapabilities( joystick->hwdata->pGamepads[i].hDevice, &joystick->hwdata->pGamepads[i].caps );

            // Initialize last pressed buttons
            XInputGetState( joystick->hwdata->pGamepads[i].hDevice, &g_InputStates[i] );

            joystick->hwdata->pGamepads[i].wLastButtons = g_InputStates[i].Gamepad.wButtons;

            for( b=0; b<8; b++ )
            {
                joystick->hwdata->pGamepads[i].bLastAnalogButtons[b] =
                    // Turn the 8-bit polled value into a boolean value
                    ( g_InputStates[i].Gamepad.bAnalogButtons[b] > XINPUT_GAMEPAD_MAX_CROSSTALK );
            }
        }
    }

	return 0;
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	static int prev_buttons;
	static short nX = 0, nY = 0;
	static short nXR = 0, nYR = 0;

	DWORD i;
	DWORD b;
	FLOAT fX1;
	FLOAT fY1;
	FLOAT fX2;
	FLOAT fY2;

	int hat;

    // TCR C6-7 Controller Discovery
    // Get status about gamepad insertions and removals.
    XBInput_RefreshDeviceList( joystick->hwdata->pGamepads );
    
    // Loop through all gamepads
    for( i=0; i < XGetPortCount(); i++ )
    {
        // If we have a valid device, poll it's state and track button changes
        if( joystick->hwdata->pGamepads[i].hDevice )
        {
            // Read the input state
            XInputGetState( joystick->hwdata->pGamepads[i].hDevice, &g_InputStates[i] );

            // Copy gamepad to local structure
            memcpy( &joystick->hwdata->pGamepads[i], &g_InputStates[i].Gamepad, sizeof(XINPUT_GAMEPAD) );

            // Put Xbox device input for the gamepad into our custom format
            fX1 = (joystick->hwdata->pGamepads[i].sThumbLX+0.5f)/32767.5f;
            joystick->hwdata->pGamepads[i].fX1 = ( fX1 >= 0.0f ? 1.0f : -1.0f ) *
                               max( 0.0f, (xfabsf(fX1)-XBINPUT_DEADZONE)/(1.0f-XBINPUT_DEADZONE) );

            fY1 = (joystick->hwdata->pGamepads[i].sThumbLY+0.5f)/32767.5f;
            joystick->hwdata->pGamepads[i].fY1 = ( fY1 >= 0.0f ? 1.0f : -1.0f ) *
                               max( 0.0f, (xfabsf(fY1)-XBINPUT_DEADZONE)/(1.0f-XBINPUT_DEADZONE) );

            fX2 = (joystick->hwdata->pGamepads[i].sThumbRX+0.5f)/32767.5f;
            joystick->hwdata->pGamepads[i].fX2 = ( fX2 >= 0.0f ? 1.0f : -1.0f ) *
                               max( 0.0f, (xfabsf(fX2)-XBINPUT_DEADZONE)/(1.0f-XBINPUT_DEADZONE) );

            fY2 = (joystick->hwdata->pGamepads[i].sThumbRY+0.5f)/32767.5f;
            joystick->hwdata->pGamepads[i].fY2 = ( fY2 >= 0.0f ? 1.0f : -1.0f ) *
                               max( 0.0f, (xfabsf(fY2)-XBINPUT_DEADZONE)/(1.0f-XBINPUT_DEADZONE) );

            // Get the boolean buttons that have been pressed since the last
            // call. Each button is represented by one bit.
            joystick->hwdata->pGamepads[i].wPressedButtons = ( joystick->hwdata->pGamepads[i].wLastButtons ^ joystick->hwdata->pGamepads[i].wButtons ) & joystick->hwdata->pGamepads[i].wButtons;
            joystick->hwdata->pGamepads[i].wLastButtons    = joystick->hwdata->pGamepads[i].wButtons;

			 
			if (joystick->hwdata->pGamepads[i].wPressedButtons & XINPUT_GAMEPAD_START)
			{
				if (!joystick->buttons[8])
					SDL_PrivateJoystickButton(joystick, (Uint8)8, SDL_PRESSED);
			}
			else
			{
				if (joystick->buttons[8])
					SDL_PrivateJoystickButton(joystick, (Uint8)8, SDL_RELEASED);
			}

			if (joystick->hwdata->pGamepads[i].wPressedButtons & XINPUT_GAMEPAD_BACK)
			{
				if (!joystick->buttons[9])
					SDL_PrivateJoystickButton(joystick, (Uint8)9, SDL_PRESSED);
			}
			else
			{
				if (joystick->buttons[9])
					SDL_PrivateJoystickButton(joystick, (Uint8)9, SDL_RELEASED);
			}

			if (joystick->hwdata->pGamepads[i].wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				if (!joystick->buttons[10])
					SDL_PrivateJoystickButton(joystick, (Uint8)10, SDL_PRESSED);
			}
			else
			{
				if (joystick->buttons[10])
					SDL_PrivateJoystickButton(joystick, (Uint8)10, SDL_RELEASED);
			}

			if (joystick->hwdata->pGamepads[i].wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				if (!joystick->buttons[11])
					SDL_PrivateJoystickButton(joystick, (Uint8)11, SDL_PRESSED);
			}
			else
			{
				if (joystick->buttons[11])
					SDL_PrivateJoystickButton(joystick, (Uint8)11, SDL_RELEASED);
			}

			 

            // Get the analog buttons that have been pressed or released since
            // the last call.
            for( b=0; b<8; b++ )
            {
                // Turn the 8-bit polled value into a boolean value
                BOOL bPressed = ( joystick->hwdata->pGamepads[i].bAnalogButtons[b] > XINPUT_GAMEPAD_MAX_CROSSTALK );

                if( bPressed )
                    joystick->hwdata->pGamepads[i].bPressedAnalogButtons[b] = !joystick->hwdata->pGamepads[i].bLastAnalogButtons[b];
                else
                    joystick->hwdata->pGamepads[i].bPressedAnalogButtons[b] = FALSE;
                

				if ( bPressed  ) {
					if ( !joystick->hwdata->pGamepads[i].bLastAnalogButtons[b] ) {
						SDL_PrivateJoystickButton(joystick, (Uint8)b, SDL_PRESSED);
					}
				} else {
					if ( joystick->hwdata->pGamepads[i].bLastAnalogButtons[b] ) {
						SDL_PrivateJoystickButton(joystick, (Uint8)b, SDL_RELEASED);
					}
				}

                // Store the current state for the next time
                joystick->hwdata->pGamepads[i].bLastAnalogButtons[b] = bPressed;
            }
        }
    }

	// do the HATS baby

	hat = SDL_HAT_CENTERED;
	if (joystick->hwdata->pGamepads[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		hat|=SDL_HAT_DOWN;
	if (joystick->hwdata->pGamepads[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
		hat|=SDL_HAT_UP;
	if (joystick->hwdata->pGamepads[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		hat|=SDL_HAT_LEFT;
	if (joystick->hwdata->pGamepads[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		hat|=SDL_HAT_RIGHT;
	if ( hat != joystick->hats[0] ) {
		SDL_PrivateJoystickHat(joystick, 0, hat);
	}

	// Axis - LStick

	if ((joystick->hwdata->pGamepads[0].sThumbLX <= -10000) || 
		(joystick->hwdata->pGamepads[0].sThumbLX >= 10000))
		nX = ((Sint16)joystick->hwdata->pGamepads[0].sThumbLX);
	else
		nX = 0;

	if ( nX != joystick->axes[0] ) 
		SDL_PrivateJoystickAxis(joystick, (Uint8)0, (Sint16)nX);

	
	if ((joystick->hwdata->pGamepads[0].sThumbLY <= -10000) || 
		(joystick->hwdata->pGamepads[0].sThumbLY >= 10000))
		nY = ((Sint16)joystick->hwdata->pGamepads[0].sThumbLY);
	else
		nY = 0;

	if ( nY != joystick->axes[1] )
		SDL_PrivateJoystickAxis(joystick, (Uint8)1, (Sint16)nY); 


	// Axis - RStick

	if ((joystick->hwdata->pGamepads[0].sThumbRX <= -10000) || 
		(joystick->hwdata->pGamepads[0].sThumbRX >= 10000))
		nXR = ((Sint16)joystick->hwdata->pGamepads[0].sThumbRX);
	else
		nXR = 0;

	if ( nXR != joystick->axes[2] ) 
		SDL_PrivateJoystickAxis(joystick, (Uint8)2, (Sint16)nXR);

	
	if ((joystick->hwdata->pGamepads[0].sThumbRY <= -10000) || 
		(joystick->hwdata->pGamepads[0].sThumbRY >= 10000))
		nYR = ((Sint16)joystick->hwdata->pGamepads[0].sThumbRY);
	else
		nYR = 0;

	if ( nYR != joystick->axes[3] )
		SDL_PrivateJoystickAxis(joystick, (Uint8)3, (Sint16)nYR); 

	return;
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	if (joystick->hwdata != NULL) {
		/* free system specific hardware data */
		free(joystick->hwdata);
	}

	return;
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	return;
}


VOID XBInput_RefreshDeviceList( XBGAMEPAD* pGamepads )
{
   
    DWORD dwInsertions, dwRemovals, i, b;

    XGetDeviceChanges( XDEVICE_TYPE_GAMEPAD, &dwInsertions, &dwRemovals );

    // Loop through all gamepads
    for( i=0; i < XGetPortCount(); i++ )
    {
        // Handle removed devices.
        pGamepads[i].bRemoved = ( dwRemovals & (1<<i) ) ? TRUE : FALSE;
        if( pGamepads[i].bRemoved )
        {
            // If the controller was removed after XGetDeviceChanges but before
            // XInputOpen, the device handle will be NULL
            if( pGamepads[i].hDevice )
                XInputClose( pGamepads[i].hDevice );
            pGamepads[i].hDevice = NULL;
            pGamepads[i].Feedback.Rumble.wLeftMotorSpeed  = 0;
            pGamepads[i].Feedback.Rumble.wRightMotorSpeed = 0;
        }

        // Handle inserted devices
        pGamepads[i].bInserted = ( dwInsertions & (1<<i) ) ? TRUE : FALSE;
        if( pGamepads[i].bInserted ) 
        {
            // TCR C6-2 Device Types
            pGamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, 
                                               XDEVICE_NO_SLOT, &g_PollingParameters );

            // if the controller is removed after XGetDeviceChanges but before
            // XInputOpen, the device handle will be NULL
            if( pGamepads[i].hDevice )
            {
                XInputGetCapabilities( pGamepads[i].hDevice, &pGamepads[i].caps );

                // Initialize last pressed buttons
                XInputGetState( pGamepads[i].hDevice, &g_InputStates[i] );

                pGamepads[i].wLastButtons = g_InputStates[i].Gamepad.wButtons;

                for( b=0; b<8; b++ )
                {
                    pGamepads[i].bLastAnalogButtons[b] =
                        // Turn the 8-bit polled value into a boolean value
                        ( g_InputStates[i].Gamepad.bAnalogButtons[b] > XINPUT_GAMEPAD_MAX_CROSSTALK );
                }
            }
        }
    }
}

