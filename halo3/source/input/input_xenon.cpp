/* ---------- headers */

#define __STATIC_ARRAYS_TEMPLATES__

#include "input/input_xenon.h"

#include "cseries/constants.h"
#include "cseries/cseries_windows.h"
#include "game/game.h"
#include "input/input.h"
#include "input/input_agnostic.h"
#include "input/input_xinput.h"
#include "interface/user_interface_controller.h"
#include "math/real_math.h"

#include "rex_macros.h"

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

/* ---------- prototypes */

static void input_update_gamepads(long elapsed_msec);
static void input_update_gamepads_rumble();

static void update_threshold(unsigned char* threshold, bool down, unsigned char current_value);
static void update_trigger(unsigned char input, unsigned char* output);
static void update_thumbstick(short input_x, short input_y, point2d* output);
static void update_thumbstick(short input_x, short input_y, point2d_be* output);

/* ---------- globals */

//static input_globals_xenon input_globals;
static REX_DATA_REFERENCE_DECLARE(0x828423F0, input_globals_xenon, input_globals);

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

static constexpr int k_gamepad_ui_shell_key_mapping[] =
{
	NONE,      // _gamepad_analog_button_left_trigger
	NONE,      // _gamepad_analog_button_right_trigger
	VK_UP,     // _gamepad_binary_button_dpad_up
	VK_DOWN,   // _gamepad_binary_button_dpad_down
	VK_LEFT,   // _gamepad_binary_button_dpad_left
	VK_RIGHT,  // _gamepad_binary_button_dpad_right
	VK_ESCAPE, // _gamepad_binary_button_start
	VK_TAB,    // _gamepad_binary_button_back
	'T',       // _gamepad_binary_button_left_thumb
	'Y',       // _gamepad_binary_button_right_thumb
	'A',       // _gamepad_binary_button_a
	'B',       // _gamepad_binary_button_b
	'X',       // _gamepad_binary_button_x
	'Y',       // _gamepad_binary_button_y
	'Q',       // _gamepad_binary_button_left_bumper
	'E',       // _gamepad_binary_button_right_bumper
};
static_assert(NUMBEROF(k_gamepad_ui_shell_key_mapping) == NUMBER_OF_GAMEPAD_BUTTONS);

static constexpr int k_gamepad_in_game_key_mapping[] =
{
	VK_MBUTTON, // _gamepad_analog_button_left_trigger
	VK_LBUTTON, // _gamepad_analog_button_right_trigger
	VK_UP,      // _gamepad_binary_button_dpad_up
	VK_DOWN,    // _gamepad_binary_button_dpad_down
	VK_LEFT,    // _gamepad_binary_button_dpad_left
	VK_RIGHT,   // _gamepad_binary_button_dpad_right
	VK_ESCAPE,  // _gamepad_binary_button_start
	VK_TAB,     // _gamepad_binary_button_back
	VK_CONTROL, // _gamepad_binary_button_left_thumb
	VK_RBUTTON, // _gamepad_binary_button_right_thumb
	VK_SPACE,   // _gamepad_binary_button_a
	'Q',        // _gamepad_binary_button_b
	VK_SHIFT,   // _gamepad_binary_button_x
	'E',        // _gamepad_binary_button_y
	'F',        // _gamepad_binary_button_left_bumper
	'R',        // _gamepad_binary_button_right_bumper
};
static_assert(NUMBEROF(k_gamepad_in_game_key_mapping) == NUMBER_OF_GAMEPAD_BUTTONS);

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

static POINT s_prev_mouse_pos;
static bool s_mouse_initialized = false;

bool input_update_gamepad(unsigned long gamepad_index, unsigned long elapsed_msec, gamepad_state_be* in_out_gamepad_state, debug_gamepad_data_be* out_debug_gamepad_data)
{
	(void)(out_debug_gamepad_data);
	assert(in_out_gamepad_state != nullptr);

	bool result;

	e_controller_index controller_index = static_cast<e_controller_index>(gamepad_index);

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
	else
	{
		result = false;
	}

	// keyboard and mouse
	if (xinput_user_index == 0)
	{
		for (long button_index = FIRST_GAMEPAD_BINARY_BUTTON; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
		{
			int key = game_is_ui_shell() ? k_gamepad_ui_shell_key_mapping[button_index] : k_gamepad_in_game_key_mapping[button_index];
			if (key != NONE)
			{
				bool key_down = GetAsyncKeyState(key) & 0x8000;

				update_button(
					&in_out_gamepad_state->button_frames[button_index],
					&in_out_gamepad_state->button_msec[button_index],
					key_down,
					elapsed_msec);
			}
		}

		{
			short thumb_lx = 0;
			short thumb_ly = 0;

			if (GetAsyncKeyState('A') & 0x8000) thumb_lx -= 32767;
			if (GetAsyncKeyState('D') & 0x8000) thumb_lx += 32767;
			if (GetAsyncKeyState('W') & 0x8000) thumb_ly += 32767;
			if (GetAsyncKeyState('S') & 0x8000) thumb_ly -= 32767;

			update_thumbstick(thumb_lx, thumb_ly, &in_out_gamepad_state->sticks[_gamepad_stick_left]);
		}

		{
			// mouse -> right stick
			POINT mouse_pos;
			GetCursorPos(&mouse_pos);

			if (!s_mouse_initialized)
			{
				s_prev_mouse_pos = mouse_pos;
				s_mouse_initialized = true;
			}

			LONG dx = mouse_pos.x - s_prev_mouse_pos.x;
			LONG dy = mouse_pos.y - s_prev_mouse_pos.y;

			s_prev_mouse_pos = mouse_pos;

			const float sensitivity = 800.0f;

			short thumb_rx = (short)(dx * sensitivity);
			short thumb_ry = (short)(-dy * sensitivity);

			if (thumb_rx >  32767) thumb_rx =  32767;
			if (thumb_rx < -32768) thumb_rx = -32768;
			if (thumb_ry >  32767) thumb_ry =  32767;
			if (thumb_ry < -32768) thumb_ry = -32768;

			update_thumbstick(thumb_rx, thumb_ry, &in_out_gamepad_state->sticks[_gamepad_stick_right]);
		}

		for (long button_index = FIRST_GAMEPAD_ANALOG_BUTTON; button_index < NUMBER_OF_GAMEPAD_ANALOG_BUTTONS; button_index++)
		{
			int key = game_is_ui_shell() ? k_gamepad_ui_shell_key_mapping[button_index] : k_gamepad_in_game_key_mapping[button_index];
			if (key != NONE)
			{
				bool key_down = GetAsyncKeyState(key) & 0x8000;
				update_trigger(key_down ? k_unsigned_char_max : 0, &in_out_gamepad_state->analog_buttons[button_index]);

				result = true;
			}
		}

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

bool input_xinput_available()
{
	return true;
}

/* ---------- private code */

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

static void input_update_gamepads_rumble()
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
