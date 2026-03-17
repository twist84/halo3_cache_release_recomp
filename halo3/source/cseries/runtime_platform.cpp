/* ---------- headers */

#include "cseries/runtime_platform.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

//extern e_runtime_platform_type active_type;
REX_DATA_REFERENCE_DECLARE(0x82881D70, e_runtime_platform_type, active_type);

/* ---------- private variables */

/* ---------- public code */

e_runtime_platform_type get_runtime_platform_type()
{
    return active_type;
}

e_runtime_platform_type set_runtime_platform_type(e_runtime_platform_type new_type)
{
    e_runtime_platform_type old_type = active_type;
    active_type = new_type;
    return old_type;
}

bool runtime_platform_needs_byteswapping()
{
    return active_type == _runtime_platform_not_xbox;
}

bool runtime_platform_on_target_platform()
{
    return active_type != _runtime_platform_not_xbox;
}

/* ---------- private code */
