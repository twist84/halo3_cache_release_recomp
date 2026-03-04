/* ---------- headers */

#include "networking/online/online_files.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

REX_DATA_REFERENCE_DECLARE(0x82862E96, bool, g_online_is_connected_to_live);

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(online_files_memory_dispose);
REX_PPC_EXTERN_IMPORT(online_files_memory_initialize);

// hooks

/* ---------- public code */

void online_files_memory_dispose(void)
{
	REX_PPC_INVOKE(online_files_memory_dispose);
}

void online_files_memory_initialize(e_map_memory_configuration memory_configuration)
{
	REX_PPC_INVOKE(online_files_memory_initialize, memory_configuration);
}

/* ---------- private code */
