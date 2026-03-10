#pragma once
#ifndef __INPUT_XENON_H__
#define __INPUT_XENON_H__

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

struct rumble_state;

/* ---------- prototypes */

extern void input_set_gamepad_rumbler_state(short gamepad_index, unsigned short left_speed, unsigned short right_speed);
extern void input_xinput_update_rumble_state(unsigned long user_index, rumble_state const* rumble_state, bool suppressed);
extern bool input_xinput_available();

/* ---------- globals */

/* ---------- public code */

#endif // __INPUT_XENON_H__
