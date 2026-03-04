#ifndef __CHUD_DRAW_H__
#define __CHUD_DRAW_H__
#pragma once

/* ---------- headers */

#include "cseries/platform.h"

/* ---------- constants */

enum e_controller_component
{
    _controller_component_button_a = 0,
    _controller_component_button_b,
    _controller_component_button_x,
    _controller_component_button_y,
    _controller_component_button_black,
    _controller_component_button_white,
    _controller_component_button_left_trigger,
    _controller_component_button_right_trigger,
    _controller_component_button_dpad_up,
    _controller_component_button_dpad_down,
    _controller_component_button_dpad_left,
    _controller_component_button_dpad_right,
    _controller_component_button_start,
    _controller_component_button_back,
    _controller_component_button_left_thumb,
    _controller_component_button_right_thumb,
    _controller_component_stick_left_thumb,
    _controller_component_stick_right_thumb,

    k_number_of_controller_components,

    _controller_component_button_right_shoulder = _controller_component_button_black,
    _controller_component_button_left_shoulder = _controller_component_button_white,
};

FORWARD_DECLARE_ENUM(e_map_memory_configuration);

/* ---------- definitions */

/* ---------- prototypes */

void user_interface_memory_dispose(void);
void user_interface_memory_initialize(e_map_memory_configuration memory_configuration);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __CHUD_DRAW_H__
