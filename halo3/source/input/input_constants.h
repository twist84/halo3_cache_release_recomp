#ifndef __INPUT_CONSTANTS_H__
#define __INPUT_CONSTANTS_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_gamepad_button
{
	FIRST_GAMEPAD_ANALOG_BUTTON = 0,
	_gamepad_analog_button_left_trigger = FIRST_GAMEPAD_ANALOG_BUTTON,
	_gamepad_analog_button_right_trigger,
	NUMBER_OF_GAMEPAD_ANALOG_BUTTONS,
	FIRST_GAMEPAD_BINARY_BUTTON = NUMBER_OF_GAMEPAD_ANALOG_BUTTONS,
	_gamepad_binary_button_dpad_up = FIRST_GAMEPAD_BINARY_BUTTON,
	_gamepad_binary_button_dpad_down,
	_gamepad_binary_button_dpad_left,
	_gamepad_binary_button_dpad_right,
	_gamepad_binary_button_start,
	_gamepad_binary_button_back,
	_gamepad_binary_button_left_thumb,
	_gamepad_binary_button_right_thumb,
	_gamepad_binary_button_a,
	_gamepad_binary_button_b,
	_gamepad_binary_button_x,
	_gamepad_binary_button_y,
	_gamepad_binary_button_left_bumper,
	_gamepad_binary_button_right_bumper,
	NUMBER_OF_GAMEPAD_BUTTONS,
	NUMBER_OF_GAMEPAD_BINARY_BUTTONS = NUMBER_OF_GAMEPAD_BUTTONS - FIRST_GAMEPAD_BINARY_BUTTON,
};

enum e_gamepad_stick
{
	_gamepad_stick_left = 0,
	_gamepad_stick_right,
	NUMBER_OF_GAMEPAD_STICKS,
};

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __INPUT_CONSTANTS_H__
