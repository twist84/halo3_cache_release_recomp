/* ---------- headers */

#include "interface/user_interface.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(user_interface_memory_dispose);
REX_PPC_EXTERN_IMPORT(user_interface_memory_initialize);

// hooks

/* ---------- public code */

void user_interface_memory_dispose(void)
{
	REX_PPC_INVOKE(user_interface_memory_dispose);
}

void user_interface_memory_initialize(e_map_memory_configuration memory_configuration)
{
	REX_PPC_INVOKE(user_interface_memory_initialize, memory_configuration);
}

/* ---------- private code */
