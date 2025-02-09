#ifndef RAGEINPUTDEVICE_H
#define RAGEINPUTDEVICE_H 1

// #include "SDL_keysym.h"
#include "RageTimer.h"

/* Only raw, unshifted keys go in this table; this doesn't include internationalized
 * keyboards, only keys that we might actually want to test for programmatically.  Any
 * other keys are mapped to KEY_OTHER_0 and up.  (If we want to support real international
 * input, stick a wchar_t in DeviceInput.)  */
 
enum RageKeySym
{
	KEY_SPACE	= 32,
	KEY_EXCL	= 33,
	KEY_QUOTE	= 34,
	KEY_HASH	= 35,
	KEY_DOLLAR	= 36,
	KEY_PERCENT = 37,
	KEY_AMPER	= 38,
	KEY_SQUOTE	= 39,
	KEY_LPAREN	= 40,
	KEY_RPAREN	= 41,
	KEY_ASTERISK= 42,
	KEY_PLUS	= 43,
	KEY_COMMA	= 44,
	KEY_HYPHEN	= 45,
	KEY_PERIOD	= 46,
	KEY_SLASH	= 47,
	KEY_C0		= 48,
	KEY_C1		= 49,
	KEY_C2		= 50,
	KEY_C3		= 51,
	KEY_C4		= 52,
	KEY_C5		= 53,
	KEY_C6		= 54,
	KEY_C7		= 55,
	KEY_C8		= 56,
	KEY_C9		= 57,
	KEY_COLON	= 58,
	KEY_SEMICOLON = 59,
	KEY_LANGLE	= 60,
	KEY_EQUAL	= 61,
	KEY_RANGLE	= 62,
	KEY_QUESTION= 63,
	KEY_AT		= 64,
	KEY_CA		= 65,
	KEY_CB		= 66,
	KEY_CC		= 67,
	KEY_CD		= 68,
	KEY_CE		= 69,
	KEY_CF		= 70,
	KEY_CG		= 71,
	KEY_CH		= 72,
	KEY_CI		= 73,
	KEY_CJ		= 74,
	KEY_CK		= 75,
	KEY_CL		= 76,
	KEY_CM		= 77,
	KEY_CN		= 78,
	KEY_CO		= 79,
	KEY_CP		= 80,
	KEY_CQ		= 81,
	KEY_CR		= 82,
	KEY_CS		= 83,
	KEY_CT		= 84,
	KEY_CU		= 85,
	KEY_CV		= 86,
	KEY_CW		= 87,
	KEY_CX		= 88,
	KEY_CY		= 89,
	KEY_CZ		= 90,
	KEY_LBRACKET= 91,
	KEY_BACKSLASH= 92,
	KEY_RBRACKET= 93,
	KEY_CARAT	= 94,
	KEY_UNDERSCORE= 95,
	KEY_ACCENT	= 96,
	KEY_Ca		= 97,
	KEY_Cb		= 98,
	KEY_Cc		= 99,
	KEY_Cd		= 100,
	KEY_Ce		= 101,
	KEY_Cf		= 102,
	KEY_Cg		= 103,
	KEY_Ch		= 104,
	KEY_Ci		= 105,
	KEY_Cj		= 106,
	KEY_Ck		= 107,
	KEY_Cl		= 108,
	KEY_Cm		= 109,
	KEY_Cn		= 110,
	KEY_Co		= 111,
	KEY_Cp		= 112,
	KEY_Cq		= 113,
	KEY_Cr		= 114,
	KEY_Cs		= 115,
	KEY_Ct		= 116,
	KEY_Cu		= 117,
	KEY_Cv		= 118,
	KEY_Cw		= 119,
	KEY_Cx		= 120,
	KEY_Cy		= 121,
	KEY_Cz		= 122,
	KEY_LBRACE	= 123,
	KEY_PIPE	= 124,
	KEY_RBRACE	= 125,
	KEY_DEL		= 127,

	KEY_BACK,
	KEY_TAB,
	KEY_ENTER,
	KEY_PAUSE,
	KEY_ESC,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,

	KEY_LCTRL,
	KEY_RCTRL,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LMETA,
	KEY_RMETA,
	KEY_LSUPER,
	KEY_RSUPER,
	KEY_MENU,

	KEY_NUMLOCK,
	KEY_SCRLLOCK,
	KEY_CAPSLOCK,
	KEY_PRTSC,

	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_INSERT,
	KEY_HOME,
	KEY_END,
	KEY_PGUP,
	KEY_PGDN,

	KEY_KP_C0,
	KEY_KP_C1,
	KEY_KP_C2,
	KEY_KP_C3,
	KEY_KP_C4,
	KEY_KP_C5,
	KEY_KP_C6,
	KEY_KP_C7,
	KEY_KP_C8,
	KEY_KP_C9,
	KEY_KP_SLASH,
	KEY_KP_ASTERISK,
	KEY_KP_HYPHEN,
	KEY_KP_PLUS,
	KEY_KP_PERIOD,
	KEY_KP_EQUAL,
	KEY_KP_ENTER,

	KEY_OTHER_0,
	/* ... */
	KEY_LAST_OTHER=512,

	NUM_KEYS,
	KEY_INVALID
};

const int NUM_KEYBOARD_BUTTONS = NUM_KEYS;
const int NUM_JOYSTICKS = 6;
const int NUM_JOYSTICK_HATS = 1;
const int NUM_PUMPS = 2;
const int NUM_PARAS = 2;

enum InputDevice {
	DEVICE_KEYBOARD = 0,
	DEVICE_JOY1,
	DEVICE_JOY2,
	DEVICE_JOY3,
	DEVICE_JOY4,
	DEVICE_JOY5,
	DEVICE_JOY6,
	DEVICE_PUMP1,
	DEVICE_PUMP2,
	DEVICE_PARA,
	NUM_INPUT_DEVICES,	// leave this at the end
	DEVICE_NONE			// means this is NULL
};

/* Joystick inputs.  We try to have enough input names so any input on a reasonable
 * joystick has an obvious mapping, but keep it generic and don't try to handle odd
 * special cases.  For example, many controllers have two sticks, so the JOY_LEFT_2, etc.
 * pairs are useful for many types of sticks. */
enum JoystickButton
{
	/* Standard axis: */
	JOY_LEFT = 0, JOY_RIGHT, 
	JOY_UP, JOY_DOWN,

	/* Secondary sticks: */
	JOY_LEFT_2, JOY_RIGHT_2,
	JOY_UP_2, JOY_DOWN_2,

	JOY_Z_UP, JOY_Z_DOWN,
	JOY_ROT_UP, JOY_ROT_DOWN, JOY_ROT_LEFT, JOY_ROT_RIGHT, JOY_ROT_Z_UP, JOY_ROT_Z_DOWN,
	JOY_HAT_LEFT, JOY_HAT_RIGHT, JOY_HAT_UP, JOY_HAT_DOWN, 
	JOY_AUX_1, JOY_AUX_2, JOY_AUX_3, JOY_AUX_4,

	/* Buttons: */
	JOY_1,	JOY_2,	JOY_3,	JOY_4,	JOY_5,	JOY_6,	JOY_7,	JOY_8,	JOY_9,	JOY_10,
	JOY_11,	JOY_12,	JOY_13,	JOY_14,	JOY_15,	JOY_16,	JOY_17,	JOY_18,	JOY_19,	JOY_20,
	JOY_21,	JOY_22,	JOY_23,	JOY_24, JOY_25, JOY_26, JOY_27, JOY_28, JOY_29, JOY_30,
	JOY_31,	JOY_32,
	NUM_JOYSTICK_BUTTONS	// leave this at the end
};

enum PumpButton {
	PUMP_UL,
	PUMP_UR,
	PUMP_MID,
	PUMP_DL,
	PUMP_DR,
	PUMP_ESCAPE,

	/* These buttons are for slave pads, attached to the first pad; they don't have
	 * their own USB device, and they have no escape button. */
	PUMP_2P_UL,
	PUMP_2P_UR,
	PUMP_2P_MID,
	PUMP_2P_DL,
	PUMP_2P_DR,
	NUM_PUMP_PAD_BUTTONS	// leave this at the end
};

enum ParaButton {
	PARA_L,
	PARA_UL,
	PARA_U,
	PARA_UR,
	PARA_R,
	PARA_SELECT,
	PARA_START,
	PARA_MENU_LEFT,
	PARA_MENU_RIGHT,
	NUM_PARA_PAD_BUTTONS	// leave this at the end
};


const int NUM_DEVICE_BUTTONS[NUM_INPUT_DEVICES] =
{
	NUM_KEYBOARD_BUTTONS, // DEVICE_KEYBOARD
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY1
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY2
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY3
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY4
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY5
	NUM_JOYSTICK_BUTTONS, // DEVICE_JOY6
	NUM_PUMP_PAD_BUTTONS, // DEVICE_PUMP1
	NUM_PUMP_PAD_BUTTONS, // DEVICE_PUMP2
	NUM_PARA_PAD_BUTTONS, // DEVICE_PARA
};

const int MAX_DEVICE_BUTTONS = NUM_KEYBOARD_BUTTONS;

struct DeviceInput
{
public:
	InputDevice device;
	int button;

	/* This is usually 0 or 1.  Analog joystick inputs can set this to a percentage (0..1).
	 * This should be 0 for analog axes within the dead zone. */
	float level;

	RageTimer ts;

	DeviceInput(): device(DEVICE_NONE), button(-1), level(0), ts(RageZeroTimer) { }
	DeviceInput( InputDevice d, int b, float l=0 ): device(d), button(b), level(l), ts(RageZeroTimer) { }
	DeviceInput( InputDevice d, int b, float l, const RageTimer &t ):
		device(d), button(b), level(l), ts(t) { }

	bool operator==( const DeviceInput &other ) const
	{ 
		/* Return true if we represent the same button on the same device.  Don't
		 * compare level or ts. */
		return device == other.device  &&  button == other.button;
	};

	CString GetDescription();
	
	CString toString();
	bool fromString( const CString &s );

	bool IsValid() const { return device != DEVICE_NONE; };
	void MakeInvalid() { device = DEVICE_NONE; };

	char ToChar() const;

	bool IsJoystick() const { return DEVICE_JOY1 <= device && device < DEVICE_JOY1+NUM_JOYSTICKS; }

	static int NumButtons(InputDevice device);
};

#endif
/*
 * Copyright (c) 2001-2002 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
