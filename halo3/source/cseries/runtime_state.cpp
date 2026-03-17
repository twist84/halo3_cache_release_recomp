/* ---------- headers */

#include "cseries/runtime_state.h"

#include "cseries/cseries_memory.h"
#include "cseries/runtime_state_constants.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct s_runtime_state_globals
{
    bool locked; // 0x0
    bool fatal_error_recorded; // 0x1
    bool unused_flags[2]; // 0x2
    unsigned long base_address; // 0x4, void*, ppc_pvoid_t
    rex::be<long> allocated_size; // 0x8
    rex::be<unsigned long> size_checksum; // 0xC
};
static_assert(sizeof(s_runtime_state_globals) == 0x10);

/* ---------- prototypes */

/* ---------- globals */

//s_runtime_state_globals g_runtime_state_globals;
REX_DATA_REFERENCE_DECLARE(0x8288193C, s_runtime_state_globals, g_runtime_state_globals);

/* ---------- private variables */

/* ---------- public code */

void runtime_state_shell_initialize()
{
    assert(g_runtime_state_globals.base_address == 0); // nullptr
    g_runtime_state_globals.base_address = runtime_state_allocate_buffer(k_runtime_state_size);
    assert(g_runtime_state_globals.base_address != 0); // nullptr

    g_runtime_state_globals.allocated_size = 0;
}

void runtime_state_shell_dispose()
{
    if (g_runtime_state_globals.base_address != 0) // nullptr
    {
        runtime_state_free_buffer(g_runtime_state_globals.base_address);
        csmemset(&g_runtime_state_globals, 0, sizeof(s_runtime_state_globals));
    }
}

void runtime_state_initialize()
{
    assert(g_runtime_state_globals.base_address != 0); // nullptr
}

void runtime_state_dispose()
{
}

void runtime_state_initialize_for_new_map()
{
    g_runtime_state_globals.locked = true;
}

void runtime_state_dispose_from_old_map()
{
}

unsigned long runtime_state_get_buffer_address(long* buffer_size) // void const*
{
    assert(buffer_size != nullptr);

    *buffer_size = g_runtime_state_globals.allocated_size;
    return g_runtime_state_globals.base_address;
}

/* ---------- private code */
