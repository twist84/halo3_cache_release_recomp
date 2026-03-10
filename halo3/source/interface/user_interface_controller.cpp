/* ---------- headers */

#include "interface/user_interface_controller.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(user_interface_controller_get_rumble_enabled);

// hooks

/* ---------- public code */

bool user_interface_controller_get_rumble_enabled(e_controller_index controller_index)
{
	return REX_PPC_INVOKE(user_interface_controller_get_rumble_enabled, controller_index);
}

/* ---------- private code */
