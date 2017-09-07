#ifndef _INPUT_H_
#define _INPUT_H_

enum Key
{
	/* The unknown key */
	UNKNOWN = GLFW_KEY_UNKNOWN,
	/* Printable keys */
	SPACE = GLFW_KEY_SPACE,
	APOSTOPHE = GLFW_KEY_APOSTROPHE,       /* ' */
	COMMA = GLFW_KEY_COMMA,         /* , */
	MINUS = GLFW_KEY_MINUS,          /* - */
	PERIOD = GLFW_KEY_PERIOD,          /* . */
	SLASH = GLFW_KEY_SLASH,          /* / */
	NUM0 = GLFW_KEY_0,
	NUM1 = GLFW_KEY_1,
	NUM2 = GLFW_KEY_2,
	NUM3 = GLFW_KEY_3,
	NUM4 = GLFW_KEY_4,
	NUM5 = GLFW_KEY_5,
	NUM6 = GLFW_KEY_6,
	NUM7 = GLFW_KEY_7,
	NUM8 = GLFW_KEY_8,
	NUM9 = GLFW_KEY_9,
	SEMICOLON = GLFW_KEY_SEMICOLON,        /* ; */
	EQUAL = GLFW_KEY_EQUAL,               /* = */
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,
	LEFTBRACKET = GLFW_KEY_LEFT_BRACKET,   /* [ */
	BLACKSLASH = GLFW_KEY_BACKSLASH,     /* \ */
	RIGHTBRACKET = GLFW_KEY_RIGHT_BRACKET, /* ] */
	GRAVE = GLFW_KEY_GRAVE_ACCENT,        /* ` */
	WORLD1 = GLFW_KEY_WORLD_1,          /* non-US #1 */
	WORLD2 = GLFW_KEY_WORLD_2,          /* non-US #2 */

	/* Function keys */
	ESCAPE = GLFW_KEY_ESCAPE,
	ENTER = GLFW_KEY_ENTER,
	TAB = GLFW_KEY_TAB,
	BACKSPACE = GLFW_KEY_BACKSPACE,
	INSERT = GLFW_KEY_INSERT,
	DEL = GLFW_KEY_DELETE,
	RIGHTARROW = GLFW_KEY_RIGHT,
	LEFTARROW = GLFW_KEY_LEFT,
	DOWNARROW = GLFW_KEY_DOWN,
	UPARROW = GLFW_KEY_UP,
	PAGEUP = GLFW_KEY_PAGE_UP,
	PAGEDOWN = GLFW_KEY_PAGE_DOWN,
	HOME = GLFW_KEY_HOME,
	END = GLFW_KEY_END,
	CAPSLOCK = GLFW_KEY_CAPS_LOCK,
	SCROLLLOCK = GLFW_KEY_SCROLL_LOCK,
	NUMLOCK = GLFW_KEY_NUM_LOCK,
	PRINTSCREEN = GLFW_KEY_PRINT_SCREEN,
	PAUSE = GLFW_KEY_PAUSE,
	F1 = GLFW_KEY_F1,
	F2 = GLFW_KEY_F2,
	F3 = GLFW_KEY_F3,
	F4 = GLFW_KEY_F4,
	F5 = GLFW_KEY_F5,
	F6 = GLFW_KEY_F6,
	F7 = GLFW_KEY_F7,
	F8 = GLFW_KEY_F8,
	F9 = GLFW_KEY_F9,
	F10 = GLFW_KEY_F10,
	F11 = GLFW_KEY_F11,
	F12 = GLFW_KEY_F12,
	F13 = GLFW_KEY_F13,
	F14 = GLFW_KEY_F14,
	F15 = GLFW_KEY_F15,
	F16 = GLFW_KEY_F16,
	F17 = GLFW_KEY_F17,
	F18 = GLFW_KEY_F18,
	F19 = GLFW_KEY_F19,
	F20 = GLFW_KEY_F20,
	F21 = GLFW_KEY_F21,
	F22 = GLFW_KEY_F22,
	F23 = GLFW_KEY_F23,
	F24 = GLFW_KEY_F24,
	F25 = GLFW_KEY_F25,
	NUMPAD0 = GLFW_KEY_KP_0,
	NUMPAD1 = GLFW_KEY_KP_1,
	NUMPAD2 = GLFW_KEY_KP_2,
	NUMPAD3 = GLFW_KEY_KP_3,
	NUMPAD4 = GLFW_KEY_KP_4,
	NUMPAD5 = GLFW_KEY_KP_5,
	NUMPAD6 = GLFW_KEY_KP_6,
	NUMPAD7 = GLFW_KEY_KP_7,
	NUMPAD8 = GLFW_KEY_KP_8,
	NUMPAD9 = GLFW_KEY_KP_9,
	NUMPADDECIMAL = GLFW_KEY_KP_DECIMAL,
	NUMPADDIVIDE = GLFW_KEY_KP_DIVIDE,
	NUMPADMULTIPLY = GLFW_KEY_KP_MULTIPLY,
	NUMPADSUBTRACT = GLFW_KEY_KP_SUBTRACT,
	NUMPADADD = GLFW_KEY_KP_ADD,
	NUMPADENTER = GLFW_KEY_KP_ENTER,
	NUMPADEQUAL = GLFW_KEY_KP_EQUAL,
	LSHIFT = GLFW_KEY_LEFT_SHIFT,
	LCTRL = GLFW_KEY_LEFT_CONTROL,
	LALT = GLFW_KEY_LEFT_ALT,
	LSUPER = GLFW_KEY_LEFT_SUPER,
	RSHIFT = GLFW_KEY_RIGHT_SHIFT,
	RCTRL = GLFW_KEY_RIGHT_CONTROL,
	RALT = GLFW_KEY_RIGHT_ALT,
	RSUPER = GLFW_KEY_RIGHT_SUPER,
	MENU = GLFW_KEY_MENU,
	KEYLAST = GLFW_KEY_LAST
};

enum State
{
	MODSHIFT = GLFW_MOD_SHIFT,
	MODCTRL = GLFW_MOD_CONTROL,
	MODALT = GLFW_MOD_ALT,
	MODSUPER = GLFW_MOD_SUPER
};

enum Mouse
{
	BUTTON1 = GLFW_MOUSE_BUTTON_1,
	BUTTON2 = GLFW_MOUSE_BUTTON_2,
	BUTTON3 = GLFW_MOUSE_BUTTON_3,
	BUTTON4 = GLFW_MOUSE_BUTTON_4,
	BUTTON5 = GLFW_MOUSE_BUTTON_5,
	BUTTON6 = GLFW_MOUSE_BUTTON_6,
	BUTTON7 = GLFW_MOUSE_BUTTON_7,
	BUTTON8 = GLFW_MOUSE_BUTTON_8,
	MOUSELAST = GLFW_MOUSE_BUTTON_LAST,
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

#endif // _INPUT_H_