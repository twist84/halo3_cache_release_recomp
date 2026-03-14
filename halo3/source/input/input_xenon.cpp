/* ---------- headers */

#define __STATIC_ARRAYS_TEMPLATES__

#include "input/input_xenon.h"

#include "cseries/constants.h"
#include "cseries/cseries_windows.h"
#include "game/game.h"
#include "input/input.h"
#include "input/input_agnostic.h"
#include "input/input_constants.h"
#include "input/input_xinput.h"
#include "interface/user_interface_controller.h"
#include "math/real_math.h"

#include "rex_macros.h"

#include <hidusage.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

/* ---------- constants */

enum
{
	k_trigger_dead_zone = 6,
	// k_thumbstick_dead_zone = 7864,
};

constexpr real k_thumbstick_dead_zone = 0.24f;

/* ---------- definitions */

struct input_globals_xenon
{
	bool initialized; // 0x0
	bool suppressed_flag; // 0x1
	bool rumble_suppressed_flag; // 0x2
	rex::be<unsigned long> last_milliseconds; // 0x4

	rex::be<unsigned short> __flags; // 0x8

	char __dataA[8]; // 0xA

	bool valid_gamepads[4]; // 0x12
	gamepad_state_be gamepad_states[4]; // 0x16

	char __data106[0x12]; // 106

	gamepad_state_be gamepad_suppressed; // 0x118
	rumble_state_be rumble_states[4]; // 0x154

	char __data164[4]; // 0x164
};
static_assert(sizeof(input_globals_xenon) == 0x168);

struct s_rawinput_state
{
	bool initialized;

	HWND window;

	bool key_down[NUMBER_OF_KEYS];
	bool mouse_buttons[NUMBER_OF_MOUSE_BUTTONS];
	long mouse_delta[NUMBER_OF_MOUSE_AXES];
};

/* ---------- prototypes */

static void input_rawinput_process(const RAWINPUT* raw);
static void input_rawinput_poll(void);
static void input_update_gamepads(long elapsed_msec);
static void input_update_gamepads_rumble(void);

static void update_threshold(unsigned char* threshold, bool down, unsigned char current_value);
static void update_trigger(unsigned char input, unsigned char* output);
static void update_thumbstick(short input_x, short input_y, point2d* output);
static void update_thumbstick(short input_x, short input_y, point2d_be* output);

/* ---------- globals */

//static input_globals_xenon input_globals;
static REX_DATA_REFERENCE_DECLARE(0x828423F0, input_globals_xenon, input_globals);

static s_rawinput_state rawinput_globals{};

REXCVAR_DEFINE_BOOL(enable_pc_joystick, false, "Blam/Runtime", "enable keyboard and mouse support");
REXCVAR_DEFINE_DOUBLE(pc_joystick_sensitivity, 0.005, "Blam/Runtime", "mouse sensitivity for pc joystick");

// real k_trigger_dead_zone_real = 0.025;
// real k_thumbstick_dead_zone_real = 0.24;

static constexpr WORD button_to_xinput_button_mask[] =
{
	0,                             // _gamepad_analog_button_left_trigger
	0,                             // _gamepad_analog_button_right_trigger
	XINPUT_GAMEPAD_DPAD_UP,        // _gamepad_binary_button_dpad_up
	XINPUT_GAMEPAD_DPAD_DOWN,      // _gamepad_binary_button_dpad_down
	XINPUT_GAMEPAD_DPAD_LEFT,      // _gamepad_binary_button_dpad_left
	XINPUT_GAMEPAD_DPAD_RIGHT,     // _gamepad_binary_button_dpad_right
	XINPUT_GAMEPAD_START,          // _gamepad_binary_button_start
	XINPUT_GAMEPAD_BACK,           // _gamepad_binary_button_back
	XINPUT_GAMEPAD_LEFT_THUMB,     // _gamepad_binary_button_left_thumb
	XINPUT_GAMEPAD_RIGHT_THUMB,    // _gamepad_binary_button_right_thumb
	XINPUT_GAMEPAD_A,              // _gamepad_binary_button_a
	XINPUT_GAMEPAD_B,              // _gamepad_binary_button_b
	XINPUT_GAMEPAD_X,              // _gamepad_binary_button_x
	XINPUT_GAMEPAD_Y,              // _gamepad_binary_button_y
	XINPUT_GAMEPAD_LEFT_SHOULDER,  // _gamepad_binary_button_left_bumper
	XINPUT_GAMEPAD_RIGHT_SHOULDER, // _gamepad_binary_button_right_bumper
};
static_assert(NUMBEROF(button_to_xinput_button_mask) == NUMBER_OF_GAMEPAD_BUTTONS);

static constexpr e_input_key_code k_gamepad_ui_shell_key_mapping[] =
{
	_key_not_a_key,  // _gamepad_analog_button_left_trigger
	_key_not_a_key,  // _gamepad_analog_button_right_trigger
	_key_up_arrow,   // _gamepad_binary_button_dpad_up
	_key_down_arrow, // _gamepad_binary_button_dpad_down
	_key_left_arrow, // _gamepad_binary_button_dpad_left
	_key_right_arrow,// _gamepad_binary_button_dpad_right
	_key_escape,     // _gamepad_binary_button_start
	_key_tab,        // _gamepad_binary_button_back
	_key_t,          // _gamepad_binary_button_left_thumb
	_key_y,          // _gamepad_binary_button_right_thumb
	_key_a,          // _gamepad_binary_button_a
	_key_b,          // _gamepad_binary_button_b
	_key_x,          // _gamepad_binary_button_x
	_key_y,          // _gamepad_binary_button_y
	_key_q,          // _gamepad_binary_button_left_bumper
	_key_e,          // _gamepad_binary_button_right_bumper
};
static_assert(NUMBEROF(k_gamepad_ui_shell_key_mapping) == NUMBER_OF_GAMEPAD_BUTTONS);

static constexpr e_input_key_code k_gamepad_in_game_key_mapping[] =
{
	_key_not_a_key,  // _gamepad_analog_button_left_trigger  (RMB, handled separately)
	_key_not_a_key,  // _gamepad_analog_button_right_trigger (LMB, handled separately)
	_key_up_arrow,   // _gamepad_binary_button_dpad_up
	_key_down_arrow, // _gamepad_binary_button_dpad_down
	_key_left_arrow, // _gamepad_binary_button_dpad_left
	_key_right_arrow,// _gamepad_binary_button_dpad_right
	_key_escape,     // _gamepad_binary_button_start
	_key_tab,        // _gamepad_binary_button_back
	_key_control,    // _gamepad_binary_button_left_thumb
	_key_not_a_key,  // _gamepad_binary_button_right_thumb   (RMB, handled separately)
	_key_space,      // _gamepad_binary_button_a
	_key_q,          // _gamepad_binary_button_b
	_key_shift,      // _gamepad_binary_button_x
	_key_e,          // _gamepad_binary_button_y
	_key_f,          // _gamepad_binary_button_left_bumper
	_key_r,          // _gamepad_binary_button_right_bumper
};
static_assert(NUMBEROF(k_gamepad_in_game_key_mapping) == NUMBER_OF_GAMEPAD_BUTTONS);

static constexpr e_input_key_code k_vk_to_key_code[256] =
{
	_key_not_a_key,     // 0x00
	_key_not_a_key,     // 0x01 VK_LBUTTON
	_key_not_a_key,     // 0x02 VK_RBUTTON
	_key_not_a_key,     // 0x03 VK_CANCEL
	_key_not_a_key,     // 0x04 VK_MBUTTON
	_key_not_a_key,     // 0x05 VK_XBUTTON1
	_key_not_a_key,     // 0x06 VK_XBUTTON2
	_key_not_a_key,     // 0x07
	_key_backspace,     // 0x08 VK_BACK
	_key_tab,           // 0x09 VK_TAB
	_key_not_a_key,     // 0x0A
	_key_not_a_key,     // 0x0B
	_key_not_a_key,     // 0x0C VK_CLEAR
	_key_return,        // 0x0D VK_RETURN
	_key_not_a_key,     // 0x0E
	_key_not_a_key,     // 0x0F
	_key_shift,         // 0x10 VK_SHIFT
	_key_control,       // 0x11 VK_CONTROL
	_key_alt,           // 0x12 VK_MENU
	_key_pause,         // 0x13 VK_PAUSE
	_key_caps_lock,     // 0x14 VK_CAPITAL
	_key_not_a_key,     // 0x15
	_key_not_a_key,     // 0x16
	_key_not_a_key,     // 0x17
	_key_not_a_key,     // 0x18
	_key_not_a_key,     // 0x19
	_key_not_a_key,     // 0x1A
	_key_escape,        // 0x1B VK_ESCAPE
	_key_not_a_key,     // 0x1C
	_key_not_a_key,     // 0x1D
	_key_not_a_key,     // 0x1E
	_key_not_a_key,     // 0x1F
	_key_space,         // 0x20 VK_SPACE
	_key_page_up,       // 0x21 VK_PRIOR
	_key_page_down,     // 0x22 VK_NEXT
	_key_end,           // 0x23 VK_END
	_key_home,          // 0x24 VK_HOME
	_key_left_arrow,    // 0x25 VK_LEFT
	_key_up_arrow,      // 0x26 VK_UP
	_key_right_arrow,   // 0x27 VK_RIGHT
	_key_down_arrow,    // 0x28 VK_DOWN
	_key_not_a_key,     // 0x29 VK_SELECT
	_key_not_a_key,     // 0x2A VK_PRINT
	_key_not_a_key,     // 0x2B VK_EXECUTE
	_key_print_screen,  // 0x2C VK_SNAPSHOT
	_key_insert,        // 0x2D VK_INSERT
	_key_delete,        // 0x2E VK_DELETE
	_key_not_a_key,     // 0x2F VK_HELP
	_key_0,             // 0x30 '0'
	_key_1,             // 0x31 '1'
	_key_2,             // 0x32 '2'
	_key_3,             // 0x33 '3'
	_key_4,             // 0x34 '4'
	_key_5,             // 0x35 '5'
	_key_6,             // 0x36 '6'
	_key_7,             // 0x37 '7'
	_key_8,             // 0x38 '8'
	_key_9,             // 0x39 '9'
	_key_not_a_key,     // 0x3A
	_key_not_a_key,     // 0x3B
	_key_not_a_key,     // 0x3C
	_key_not_a_key,     // 0x3D
	_key_not_a_key,     // 0x3E
	_key_not_a_key,     // 0x3F
	_key_not_a_key,     // 0x40
	_key_a,             // 0x41 'A'
	_key_b,             // 0x42 'B'
	_key_c,             // 0x43 'C'
	_key_d,             // 0x44 'D'
	_key_e,             // 0x45 'E'
	_key_f,             // 0x46 'F'
	_key_g,             // 0x47 'G'
	_key_h,             // 0x48 'H'
	_key_i,             // 0x49 'I'
	_key_j,             // 0x4A 'J'
	_key_k,             // 0x4B 'K'
	_key_l,             // 0x4C 'L'
	_key_m,             // 0x4D 'M'
	_key_n,             // 0x4E 'N'
	_key_o,             // 0x4F 'O'
	_key_p,             // 0x50 'P'
	_key_q,             // 0x51 'Q'
	_key_r,             // 0x52 'R'
	_key_s,             // 0x53 'S'
	_key_t,             // 0x54 'T'
	_key_u,             // 0x55 'U'
	_key_v,             // 0x56 'V'
	_key_w,             // 0x57 'W'
	_key_x,             // 0x58 'X'
	_key_y,             // 0x59 'Y'
	_key_z,             // 0x5A 'Z'
	_key_left_windows,  // 0x5B VK_LWIN
	_key_right_windows, // 0x5C VK_RWIN
	_key_menu,          // 0x5D VK_APPS
	_key_not_a_key,     // 0x5E
	_key_not_a_key,     // 0x5F VK_SLEEP
	_keypad_0,          // 0x60 VK_NUMPAD0
	_keypad_1,          // 0x61 VK_NUMPAD1
	_keypad_2,          // 0x62 VK_NUMPAD2
	_keypad_3,          // 0x63 VK_NUMPAD3
	_keypad_4,          // 0x64 VK_NUMPAD4
	_keypad_5,          // 0x65 VK_NUMPAD5
	_keypad_6,          // 0x66 VK_NUMPAD6
	_keypad_7,          // 0x67 VK_NUMPAD7
	_keypad_8,          // 0x68 VK_NUMPAD8
	_keypad_9,          // 0x69 VK_NUMPAD9
	_keypad_multiply,   // 0x6A VK_MULTIPLY
	_keypad_add,        // 0x6B VK_ADD
	_keypad_enter,      // 0x6C VK_SEPARATOR
	_keypad_subtract,   // 0x6D VK_SUBTRACT
	_keypad_decimal,    // 0x6E VK_DECIMAL
	_keypad_divide,     // 0x6F VK_DIVIDE
	_key_f1,            // 0x70 VK_F1
	_key_f2,            // 0x71 VK_F2
	_key_f3,            // 0x72 VK_F3
	_key_f4,            // 0x73 VK_F4
	_key_f5,            // 0x74 VK_F5
	_key_f6,            // 0x75 VK_F6
	_key_f7,            // 0x76 VK_F7
	_key_f8,            // 0x77 VK_F8
	_key_f9,            // 0x78 VK_F9
	_key_f10,           // 0x79 VK_F10
	_key_f11,           // 0x7A VK_F11
	_key_f12,           // 0x7B VK_F12
	_key_not_a_key,     // 0x7C VK_F13
	_key_not_a_key,     // 0x7D VK_F14
	_key_not_a_key,     // 0x7E VK_F15
	_key_not_a_key,     // 0x7F VK_F16
	_key_not_a_key,     // 0x80 VK_F17
	_key_not_a_key,     // 0x81 VK_F18
	_key_not_a_key,     // 0x82 VK_F19
	_key_not_a_key,     // 0x83 VK_F20
	_key_not_a_key,     // 0x84 VK_F21
	_key_not_a_key,     // 0x85 VK_F22
	_key_not_a_key,     // 0x86 VK_F23
	_key_not_a_key,     // 0x87 VK_F24
	_key_not_a_key,     // 0x88
	_key_not_a_key,     // 0x89
	_key_not_a_key,     // 0x8A
	_key_not_a_key,     // 0x8B
	_key_not_a_key,     // 0x8C
	_key_not_a_key,     // 0x8D
	_key_not_a_key,     // 0x8E
	_key_not_a_key,     // 0x8F
	_keypad_num_lock,   // 0x90 VK_NUMLOCK
	_key_scroll_lock,   // 0x91 VK_SCROLL
	_key_not_a_key,     // 0x92
	_key_not_a_key,     // 0x93
	_key_not_a_key,     // 0x94
	_key_not_a_key,     // 0x95
	_key_not_a_key,     // 0x96
	_key_not_a_key,     // 0x97
	_key_not_a_key,     // 0x98
	_key_not_a_key,     // 0x99
	_key_not_a_key,     // 0x9A
	_key_not_a_key,     // 0x9B
	_key_not_a_key,     // 0x9C
	_key_not_a_key,     // 0x9D
	_key_not_a_key,     // 0x9E
	_key_not_a_key,     // 0x9F
	_key_left_shift,    // 0xA0 VK_LSHIFT
	_key_right_shift,   // 0xA1 VK_RSHIFT
	_key_left_control,  // 0xA2 VK_LCONTROL
	_key_right_control, // 0xA3 VK_RCONTROL
	_key_left_alt,      // 0xA4 VK_LMENU
	_key_right_alt,     // 0xA5 VK_RMENU
	_key_not_a_key,     // 0xA6
	_key_not_a_key,     // 0xA7
	_key_not_a_key,     // 0xA8
	_key_not_a_key,     // 0xA9
	_key_not_a_key,     // 0xAA
	_key_not_a_key,     // 0xAB
	_key_not_a_key,     // 0xAC
	_key_not_a_key,     // 0xAD
	_key_not_a_key,     // 0xAE
	_key_not_a_key,     // 0xAF
	_key_not_a_key,     // 0xB0
	_key_not_a_key,     // 0xB1
	_key_not_a_key,     // 0xB2
	_key_not_a_key,     // 0xB3
	_key_not_a_key,     // 0xB4
	_key_not_a_key,     // 0xB5
	_key_not_a_key,     // 0xB6
	_key_not_a_key,     // 0xB7
	_key_not_a_key,     // 0xB8
	_key_not_a_key,     // 0xB9
	_key_semicolon,     // 0xBA VK_OEM_1
	_key_equal,         // 0xBB VK_OEM_PLUS
	_key_comma,         // 0xBC VK_OEM_COMMA
	_key_dash,          // 0xBD VK_OEM_MINUS
	_key_period,        // 0xBE VK_OEM_PERIOD
	_key_forwardslash,  // 0xBF VK_OEM_2
	_key_backquote,     // 0xC0 VK_OEM_3
	_key_not_a_key,     // 0xC1
	_key_not_a_key,     // 0xC2
	_key_not_a_key,     // 0xC3
	_key_not_a_key,     // 0xC4
	_key_not_a_key,     // 0xC5
	_key_not_a_key,     // 0xC6
	_key_not_a_key,     // 0xC7
	_key_not_a_key,     // 0xC8
	_key_not_a_key,     // 0xC9
	_key_not_a_key,     // 0xCA
	_key_not_a_key,     // 0xCB
	_key_not_a_key,     // 0xCC
	_key_not_a_key,     // 0xCD
	_key_not_a_key,     // 0xCE
	_key_not_a_key,     // 0xCF
	_key_not_a_key,     // 0xD0
	_key_not_a_key,     // 0xD1
	_key_not_a_key,     // 0xD2
	_key_not_a_key,     // 0xD3
	_key_not_a_key,     // 0xD4
	_key_not_a_key,     // 0xD5
	_key_not_a_key,     // 0xD6
	_key_not_a_key,     // 0xD7
	_key_not_a_key,     // 0xD8
	_key_not_a_key,     // 0xD9
	_key_not_a_key,     // 0xDA
	_key_left_bracket,  // 0xDB VK_OEM_4
	_key_backslash,     // 0xDC VK_OEM_5
	_key_right_bracket, // 0xDD VK_OEM_6
	_key_apostrophe,    // 0xDE VK_OEM_7
	_key_not_a_key,     // 0xDF VK_OEM_8
	_key_not_a_key,     // 0xE0
	_key_not_a_key,     // 0xE1
	_key_not_a_key,     // 0xE2 VK_OEM_102
	_key_not_a_key,     // 0xE3
	_key_not_a_key,     // 0xE4
	_key_not_a_key,     // 0xE5
	_key_not_a_key,     // 0xE6
	_key_not_a_key,     // 0xE7
	_key_not_a_key,     // 0xE8
	_key_not_a_key,     // 0xE9
	_key_not_a_key,     // 0xEA
	_key_not_a_key,     // 0xEB
	_key_not_a_key,     // 0xEC
	_key_not_a_key,     // 0xED
	_key_not_a_key,     // 0xEE
	_key_not_a_key,     // 0xEF
	_key_not_a_key,     // 0xF0
	_key_not_a_key,     // 0xF1
	_key_not_a_key,     // 0xF2
	_key_not_a_key,     // 0xF3
	_key_not_a_key,     // 0xF4
	_key_not_a_key,     // 0xF5
	_key_not_a_key,     // 0xF6 VK_ATTN
	_key_not_a_key,     // 0xF7 VK_CRSEL
	_key_not_a_key,     // 0xF8 VK_EXSEL
	_key_not_a_key,     // 0xF9 VK_EREOF
	_key_not_a_key,     // 0xFA VK_PLAY
	_key_not_a_key,     // 0xFB VK_ZOOM
	_key_not_a_key,     // 0xFC VK_NONAME
	_key_not_a_key,     // 0xFD VK_PA1
	_key_not_a_key,     // 0xFE VK_OEM_CLEAR
	_key_not_a_key,     // 0xFF
};
static_assert(NUMBEROF(k_vk_to_key_code) == 256);

/* ---------- ppc */

// exports

// hooks

REX_PPC_HOOK(input_update);

/* ---------- public code */

void update_button(unsigned char* frames, unsigned short* msec, bool down, unsigned long elapsed_msec)
{
	*frames = down ? MIN(*frames + 1, k_unsigned_char_max) : 0;
	*msec = down ? MIN(*msec + elapsed_msec, k_unsigned_short_max) : 0;
}

void update_button(unsigned char* frames, rex::be<unsigned short>* msec, bool down, unsigned long elapsed_msec)
{
	*frames = down ? MIN(*frames + 1, k_unsigned_char_max) : 0;
	*msec = down ? MIN(*msec + elapsed_msec, k_unsigned_short_max) : 0;
}

static LRESULT CALLBACK input_rawinput_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_INPUT)
	{
		UINT size = sizeof(RAWINPUT);
		BYTE buffer[sizeof(RAWINPUT)];

		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
				buffer, &size, sizeof(RAWINPUTHEADER)) != static_cast<UINT>(-1))
		{
			input_rawinput_process(reinterpret_cast<const RAWINPUT*>(buffer));
		}
	}

	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

bool input_rawinput_initialize(void)
{
	WNDCLASSEXW wc = {};
	wc.cbSize        = sizeof(wc);
	wc.lpfnWndProc   = input_rawinput_wndproc;
	wc.hInstance     = GetModuleHandleW(nullptr);
	wc.lpszClassName = L"input_rawinput";
	RegisterClassExW(&wc);

	rawinput_globals.window = CreateWindowExW(
		0,
		L"input_rawinput",
		nullptr,
		0, 0, 0, 0, 0,
		HWND_MESSAGE,
		nullptr,
		GetModuleHandleW(nullptr),
		nullptr);

	bool result = false;
	if (rawinput_globals.window != nullptr)
	{
		RAWINPUTDEVICE devices[2] = {};

		devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		devices[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
		devices[0].dwFlags     = RIDEV_INPUTSINK;
		devices[0].hwndTarget  = rawinput_globals.window;

		devices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		devices[1].usUsage     = HID_USAGE_GENERIC_KEYBOARD;
		devices[1].dwFlags     = RIDEV_INPUTSINK;
		devices[1].hwndTarget  = rawinput_globals.window;

		result = RegisterRawInputDevices(devices, ARRAYSIZE(devices), sizeof(RAWINPUTDEVICE)) == TRUE;
	}

	return result;
}

bool input_initialize(void)
{
	memset(&input_globals, 0, sizeof(input_globals));
	input_globals.initialized = true;
	input_globals.__flags = input_globals.__flags | FLAG(0);
	input_update();

	return input_globals.initialized;
}

void input_update(void)
{
	if (!rawinput_globals.initialized)
	{
		rawinput_globals.initialized = true;
		input_rawinput_initialize();
	}

	if (input_globals.initialized)
	{
		unsigned long time = system_milliseconds();
		long elapsed_msec = static_cast<long>(CLAMP(time - input_globals.last_milliseconds, 0, 100));

		input_globals.suppressed_flag = false;
		input_globals.last_milliseconds = time;

		for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
		{
			input_globals.valid_gamepads[controller_index] = false;
		}

		input_update_gamepads(elapsed_msec);
		input_update_gamepads_rumble();
	}
}

void input_flush(void)
{
	memset(input_globals.gamepad_states, 0, sizeof(input_globals.gamepad_states));
}

void input_suppress(void)
{
	input_globals.suppressed_flag = true;
}

void input_feedback_suppress(bool suppress)
{
	input_globals.rumble_suppressed_flag = suppress;
}

bool input_has_gamepad(short gamepad_index)
{
	assert(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	return input_globals.valid_gamepads[gamepad_index];
}

gamepad_state_be const* input_get_gamepad_state(short gamepad_index)
{
	assert(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	gamepad_state_be const* result;
	if (input_has_gamepad(gamepad_index))
	{
		if (input_globals.suppressed_flag)
		{
			result = &input_globals.gamepad_suppressed;
		}
		else
		{
			result = &input_globals.gamepad_states[gamepad_index];
		}
	}
	else
	{
		result = nullptr;
	}
	return result;
}

void input_clear_all_rumblers(void)
{
	for (short gamepad_index = 0; gamepad_index < k_number_of_controllers; gamepad_index++)
	{
		if (input_has_gamepad(gamepad_index))
		{
			input_set_gamepad_rumbler_state(gamepad_index, 0, 0);
		}
	}
	input_update_gamepads_rumble();
}

void input_set_gamepad_rumbler_state(short gamepad_index, unsigned short left_speed, unsigned short right_speed)
{
	assert(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	e_controller_index controller_index = static_cast<e_controller_index>(gamepad_index);
	rumble_state_be* rumble_state = &input_globals.rumble_states[controller_index];
	if (user_interface_controller_get_rumble_enabled(controller_index))
	{
		rumble_state->left_speed = left_speed;
		rumble_state->right_speed = right_speed;
	}
	else
	{
		rumble_state->left_speed = 0;
		rumble_state->right_speed = 0;
	}
}

void update_threshold(unsigned char* analog_buttons, bool trigger_down, unsigned char duration_ms)
{
	if (trigger_down)
	{
		unsigned char msec_down = CLAMP_LOWER(duration_ms, 0, 32);
		if (*analog_buttons <= msec_down)
		{
			*analog_buttons = msec_down;
		}
	}
	else
	{
		unsigned char msec_down = CLAMP_UPPER(duration_ms, 64, 255);
		if (*analog_buttons >= msec_down)
		{
			*analog_buttons = msec_down;
		}
	}
}

bool input_update_gamepad(unsigned long gamepad_index, unsigned long elapsed_msec, gamepad_state* in_out_gamepad_state, debug_gamepad_data* out_debug_gamepad_data)
{
	(void)(out_debug_gamepad_data);
	assert(in_out_gamepad_state != nullptr);

	e_controller_index controller_index = static_cast<e_controller_index>(gamepad_index);
	bool result = false;

	DWORD xinput_user_index = static_cast<DWORD>(controller_index);
	XINPUT_STATE xinput_state;
	if (XInputGetState(xinput_user_index, &xinput_state) == ERROR_SUCCESS)
	{
		for (long button_index = FIRST_GAMEPAD_BINARY_BUTTON; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
		{
			bool binary_down = TEST_MASK(xinput_state.Gamepad.wButtons, button_to_xinput_button_mask[button_index]);

			update_button(
				&in_out_gamepad_state->button_frames[button_index],
				&in_out_gamepad_state->button_msec[button_index],
				binary_down,
				elapsed_msec);
		}

		update_thumbstick(xinput_state.Gamepad.sThumbLX, xinput_state.Gamepad.sThumbLY, &in_out_gamepad_state->sticks[_gamepad_stick_left]);
		update_thumbstick(xinput_state.Gamepad.sThumbRX, xinput_state.Gamepad.sThumbRY, &in_out_gamepad_state->sticks[_gamepad_stick_right]);

		update_trigger(xinput_state.Gamepad.bLeftTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_left_trigger]);
		update_trigger(xinput_state.Gamepad.bRightTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_right_trigger]);

		for (long button_index = 0; button_index < NUMBER_OF_GAMEPAD_ANALOG_BUTTONS; button_index++)
		{
			bool binary_down = in_out_gamepad_state->analog_buttons[button_index] > in_out_gamepad_state->analog_button_thresholds[button_index];

			update_button(
				&in_out_gamepad_state->button_frames[button_index],
				&in_out_gamepad_state->button_msec[button_index],
				binary_down,
				elapsed_msec);

			update_threshold(
				&in_out_gamepad_state->analog_button_thresholds[button_index],
				binary_down,
				in_out_gamepad_state->analog_buttons[button_index]);
		}

		result = true;
	}

	return result;
}

bool input_update_gamepad(unsigned long gamepad_index, unsigned long elapsed_msec, gamepad_state_be* in_out_gamepad_state, debug_gamepad_data_be* out_debug_gamepad_data)
{
	(void)(out_debug_gamepad_data);
	assert(in_out_gamepad_state != nullptr);

	bool result = false;

	bool pc_joystick_enabled = REXCVAR_GET(enable_pc_joystick);
	if (pc_joystick_enabled)
	{
		// ------------------------------------------------------------
		// PC controls (only controller slot 0)
		// ------------------------------------------------------------

		if (gamepad_index == 0)
		{
			const e_input_key_code* key_mapping = game_is_ui_shell()
				? k_gamepad_ui_shell_key_mapping
				: k_gamepad_in_game_key_mapping;

			for (long button_index = 0; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
			{
				e_input_key_code key_code = key_mapping[button_index];
				bool down = key_code != _key_not_a_key && rawinput_globals.key_down[key_code];

				update_button(
					&in_out_gamepad_state->button_frames[button_index],
					&in_out_gamepad_state->button_msec[button_index],
					down,
					elapsed_msec);
			}

			// WASD -> left stick
			float lx = 0.0f, ly = 0.0f;
			if (rawinput_globals.key_down[_key_a]) lx -= 1.0f;
			if (rawinput_globals.key_down[_key_d]) lx += 1.0f;
			if (rawinput_globals.key_down[_key_w]) ly += 1.0f;
			if (rawinput_globals.key_down[_key_s]) ly -= 1.0f;

			update_thumbstick(
				static_cast<short>(lx * k_short_max),
				static_cast<short>(ly * k_short_max),
				&in_out_gamepad_state->sticks[_gamepad_stick_left]);

			// Mouse -> right stick
			const float mouse_sensitivity = static_cast<float>(REXCVAR_GET(pc_joystick_sensitivity));

			float sx = CLAMP(rawinput_globals.mouse_delta[_mouse_delta_x] * mouse_sensitivity, -1.0f, 1.0f);
			float sy = CLAMP(rawinput_globals.mouse_delta[_mouse_delta_y] * mouse_sensitivity, -1.0f, 1.0f);

			rawinput_globals.mouse_delta[_mouse_delta_x] = 0;
			rawinput_globals.mouse_delta[_mouse_delta_y] = 0;

			update_thumbstick(
				static_cast<short>(sx * k_short_max),
				static_cast<short>(-sy * k_short_max), // Y inverted: screen-down = look-down
				&in_out_gamepad_state->sticks[_gamepad_stick_right]);

			// Mouse buttons -> triggers
			in_out_gamepad_state->analog_buttons[_gamepad_analog_button_left_trigger]  = rawinput_globals.mouse_buttons[_mouse_button_right] ? 1.0f : 0.0f;
			in_out_gamepad_state->analog_buttons[_gamepad_analog_button_right_trigger] = rawinput_globals.mouse_buttons[_mouse_button_left]  ? 1.0f : 0.0f;

			result = true;
		}
		else
		{
			result = false;
		}
	}
	else
	{
		// ------------------------------------------------------------
		// XInput gamepad
		// ------------------------------------------------------------

		e_controller_index controller_index = static_cast<e_controller_index>(gamepad_index);

		DWORD xinput_user_index = static_cast<DWORD>(controller_index);
		XINPUT_STATE xinput_state{};

		if (XInputGetState(xinput_user_index, &xinput_state) == ERROR_SUCCESS)
		{
			for (long button_index = FIRST_GAMEPAD_BINARY_BUTTON; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
			{
				bool binary_down = TEST_MASK(xinput_state.Gamepad.wButtons, button_to_xinput_button_mask[button_index]);

				update_button(
					&in_out_gamepad_state->button_frames[button_index],
					&in_out_gamepad_state->button_msec[button_index],
					binary_down,
					elapsed_msec);
			}

			update_thumbstick(xinput_state.Gamepad.sThumbLX, xinput_state.Gamepad.sThumbLY, &in_out_gamepad_state->sticks[_gamepad_stick_left]);
			update_thumbstick(xinput_state.Gamepad.sThumbRX, xinput_state.Gamepad.sThumbRY, &in_out_gamepad_state->sticks[_gamepad_stick_right]);

			update_trigger(xinput_state.Gamepad.bLeftTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_left_trigger]);
			update_trigger(xinput_state.Gamepad.bRightTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_right_trigger]);

			result = true;
		}
		else
		{
			result = false;
		}
	}

	for (long button_index = 0; button_index < NUMBER_OF_GAMEPAD_ANALOG_BUTTONS; button_index++)
	{
		bool binary_down = in_out_gamepad_state->analog_buttons[button_index] >
			in_out_gamepad_state->analog_button_thresholds[button_index];

		update_button(
			&in_out_gamepad_state->button_frames[button_index],
			&in_out_gamepad_state->button_msec[button_index],
			binary_down,
			elapsed_msec);

		update_threshold(
			&in_out_gamepad_state->analog_button_thresholds[button_index],
			binary_down,
			in_out_gamepad_state->analog_buttons[button_index]);
	}

	return result;
}

void input_xinput_update_rumble_state(unsigned long user_index, rumble_state const* rumble_state, bool suppressed)
{
	assert(rumble_state != nullptr);

	if (input_xinput_available())
	{
		XINPUT_VIBRATION vibration = {};
		if (!suppressed)
		{
			vibration.wLeftMotorSpeed = rumble_state->left_speed;
			vibration.wRightMotorSpeed = rumble_state->right_speed;
		}
		XInputSetState(user_index, &vibration);
	}
}

void input_xinput_update_rumble_state(unsigned long user_index, rumble_state_be const* rumble_state, bool suppressed)
{
	assert(rumble_state != nullptr);

	if (input_xinput_available())
	{
		XINPUT_VIBRATION vibration = {};
		if (!suppressed)
		{
			vibration.wLeftMotorSpeed = rumble_state->left_speed;
			vibration.wRightMotorSpeed = rumble_state->right_speed;
		}
		XInputSetState(user_index, &vibration);
	}
}

bool input_xinput_available(void)
{
	return true;
}

/* ---------- private code */

static void input_rawinput_process(const RAWINPUT* raw)
{
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		const RAWMOUSE& m = raw->data.mouse;

		if (!(m.usFlags & MOUSE_MOVE_ABSOLUTE))
		{
			rawinput_globals.mouse_delta[_mouse_delta_x] += m.lLastX;
			rawinput_globals.mouse_delta[_mouse_delta_y] += m.lLastY;
		}

		if (m.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)   rawinput_globals.mouse_buttons[_mouse_button_left]   = true;
		if (m.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)     rawinput_globals.mouse_buttons[_mouse_button_left]   = false;
		if (m.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) rawinput_globals.mouse_buttons[_mouse_button_middle] = true;
		if (m.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)   rawinput_globals.mouse_buttons[_mouse_button_middle] = false;
		if (m.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)  rawinput_globals.mouse_buttons[_mouse_button_right]  = true;
		if (m.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)    rawinput_globals.mouse_buttons[_mouse_button_right]  = false;
		if (m.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)      rawinput_globals.mouse_buttons[_mouse_button_button4] = true;
		if (m.usButtonFlags & RI_MOUSE_BUTTON_4_UP)        rawinput_globals.mouse_buttons[_mouse_button_button4] = false;
		if (m.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)      rawinput_globals.mouse_buttons[_mouse_button_button5] = true;
		if (m.usButtonFlags & RI_MOUSE_BUTTON_5_UP)        rawinput_globals.mouse_buttons[_mouse_button_button5] = false;

		if (m.usButtonFlags & RI_MOUSE_WHEEL)
		{
			rawinput_globals.mouse_delta[_mouse_delta_wheel] += static_cast<short>(m.usButtonData);
		}
	}
	else if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		const RAWKEYBOARD& kb = raw->data.keyboard;

		e_input_key_code key_code = k_vk_to_key_code[kb.VKey];
		if (key_code != _key_not_a_key)
		{
			rawinput_globals.key_down[key_code] = !(kb.Flags & RI_KEY_BREAK);
		}
	}
}

static void input_rawinput_poll(void)
{
	HWND foreground = GetForegroundWindow();
	DWORD foreground_pid = 0;
	GetWindowThreadProcessId(foreground, &foreground_pid);

	if (foreground_pid == GetCurrentProcessId() && foreground != GetConsoleWindow())
	{
		RECT clip = {};
		clip.left   = (GetSystemMetrics(SM_CXSCREEN) - 100) / 2;
		clip.top    = (GetSystemMetrics(SM_CYSCREEN) - 100) / 2;
		clip.right  = clip.left + 100;
		clip.bottom = clip.top  + 100;
		ClipCursor(&clip);
	}
	else
	{
		ClipCursor(nullptr);
	}

	MSG msg;
	while (PeekMessageW(&msg, rawinput_globals.window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

static void update_trigger(unsigned char input, unsigned char* output)
{
	if (input < k_trigger_dead_zone)
	{
		input = 0;
	}

	*output = input;
}

static void update_thumbstick(short input_x, short input_y, point2d* output)
{
	real scale_x = fabsf(static_cast<real>(input_x > 0 ? k_short_max : k_short_min));
	real scale_y = fabsf(static_cast<real>(input_y > 0 ? k_short_max : k_short_min));

	real_vector2d thumbstick;
	set_real_vector2d(
		&thumbstick,
		static_cast<real>(input_x) / scale_x,
		static_cast<real>(input_y) / scale_y);

	real_vector2d thumbstick_l_direction = thumbstick;
	real magnitude = normalize2d(&thumbstick_l_direction);
	if (magnitude > 1.0f)
	{
		thumbstick = thumbstick_l_direction;
	}
	else if (magnitude < k_thumbstick_dead_zone)
	{
		set_real_vector2d(&thumbstick, 0.0f, 0.0f);
	}
	else
	{
		thumbstick = thumbstick_l_direction;
		real adjusted_magnitude = (magnitude - k_thumbstick_dead_zone) / (1.0f - k_thumbstick_dead_zone);
		scale_vector2d(&thumbstick, adjusted_magnitude, &thumbstick);
	}

	output->x = static_cast<short>(thumbstick.i * scale_x);
	output->y = static_cast<short>(thumbstick.j * scale_y);
}

static void update_thumbstick(short input_x, short input_y, point2d_be* output)
{
	real scale_x = fabsf(static_cast<real>(input_x > 0 ? k_short_max : k_short_min));
	real scale_y = fabsf(static_cast<real>(input_y > 0 ? k_short_max : k_short_min));

	real_vector2d thumbstick;
	set_real_vector2d(
		&thumbstick,
		static_cast<real>(input_x) / scale_x,
		static_cast<real>(input_y) / scale_y);

	real_vector2d thumbstick_l_direction = thumbstick;
	real magnitude = normalize2d(&thumbstick_l_direction);
	if (magnitude > 1.0f)
	{
		thumbstick = thumbstick_l_direction;
	}
	else if (magnitude < k_thumbstick_dead_zone)
	{
		set_real_vector2d(&thumbstick, 0.0f, 0.0f);
	}
	else
	{
		thumbstick = thumbstick_l_direction;
		real adjusted_magnitude = (magnitude - k_thumbstick_dead_zone) / (1.0f - k_thumbstick_dead_zone);
		scale_vector2d(&thumbstick, adjusted_magnitude, &thumbstick);
	}

	output->x = static_cast<short>(thumbstick.i * scale_x);
	output->y = static_cast<short>(thumbstick.j * scale_y);
}

static void input_update_gamepads(long elapsed_msec)
{
	bool pc_joystick_enabled = REXCVAR_GET(enable_pc_joystick);
	if (pc_joystick_enabled)
	{
		input_rawinput_poll();
	}

	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		gamepad_state_be* state = &input_globals.gamepad_states[controller_index];
		bool is_valid = input_update_gamepad(static_cast<unsigned long>(controller_index), static_cast<unsigned long>(elapsed_msec), state, nullptr);

		if (is_valid)
		{
			input_globals.valid_gamepads[controller_index] = is_valid;
		}
		else
		{
			memset(state, 0, sizeof(*state));
		}
	}
}

static void input_update_gamepads_rumble(void)
{
	bool suppress_rumble = input_globals.rumble_suppressed_flag || input_globals.suppressed_flag;
	if (game_in_progress())
	{
		if (game_time_get_paused())
		{
			suppress_rumble = true;
		}
	}
	else
	{
		suppress_rumble = true;
	}

	for (short gamepad_index = 0; gamepad_index < NUMBEROF(input_globals.rumble_states); gamepad_index++)
	{
		input_xinput_update_rumble_state(gamepad_index, &input_globals.rumble_states[gamepad_index], suppress_rumble);
	}
}
