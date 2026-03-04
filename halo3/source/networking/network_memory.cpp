/* ---------- headers */

#include "networking/network_memory.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(network_memory_shared_dispose);
REX_PPC_EXTERN_IMPORT(network_memory_shared_initialize);

// hooks

/* ---------- public code */

void network_memory_shared_dispose(void)
{
    REX_PPC_INVOKE(network_memory_shared_dispose);
}

void network_memory_shared_initialize(e_map_memory_configuration memory_configuration)
{
    REX_PPC_INVOKE(network_memory_shared_initialize, memory_configuration);
}

/* ---------- private code */
