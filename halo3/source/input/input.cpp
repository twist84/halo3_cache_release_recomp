/* ---------- headers */

#include "input/input.h"

#include "input/controllers.h"
#include "input/input_constants.h"

#include <cassert>

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

bool input_any_controller_button_down(e_gamepad_button button)
{
    assert(VALID_INDEX(button, NUMBER_OF_GAMEPAD_BUTTONS));

    bool button_down = false;

    for (e_controller_index controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
    {
        gamepad_state const* gamepad = input_get_gamepad_state(static_cast<short>(controller_index));
        if (gamepad != nullptr && gamepad->button_frames[button])
        {
            button_down = true;
        }
    }

    return button_down;
}

/* ---------- private code */
