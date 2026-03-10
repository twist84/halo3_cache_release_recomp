#ifndef __INPUT_H__
#define __INPUT_H__
#pragma once

/* ---------- headers */

#include "input/input_constants.h"
#include "input/controllers.h"
#include "input/input_agnostic.h"
#include "input/input_xinput.h"
#include "math/integer_math.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct gamepad_state
{
    unsigned char analog_buttons[NUMBER_OF_GAMEPAD_ANALOG_BUTTONS]; // 0x0
    unsigned char analog_button_thresholds[NUMBER_OF_GAMEPAD_ANALOG_BUTTONS]; // 0x2
    unsigned char button_frames[NUMBER_OF_GAMEPAD_BUTTONS]; // 0x4
    rex::be<unsigned short> button_msec[NUMBER_OF_GAMEPAD_BUTTONS]; // 0x14
    point2d sticks[NUMBER_OF_GAMEPAD_STICKS]; // 0x34
};
static_assert(sizeof(gamepad_state) == 0x3C);

/* ---------- prototypes */

extern bool input_initialize();
extern bool input_update_gamepad(unsigned long gamepad_index, unsigned long elapsed_msec, gamepad_state* in_out_gamepad_state, debug_gamepad_data* out_debug_gamepad_data);
extern void input_update();
extern void input_flush();
extern void input_suppress();
extern void input_feedback_suppress(bool suppress);
extern bool input_has_gamepad(short gamepad_index);
extern gamepad_state const* input_get_gamepad_state(short gamepad_index);
extern void input_clear_all_rumblers();
extern void input_set_gamepad_rumbler_state(short gamepad_index, unsigned short left_speed, unsigned short right_speed);
extern bool input_any_controller_button_down(e_gamepad_button button);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __INPUT_H__
