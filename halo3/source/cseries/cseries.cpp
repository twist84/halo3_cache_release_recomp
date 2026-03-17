/* ---------- headers */

#include "cseries/cseries.h"

#include "multithreading/threads.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(cseries_dispose);

// hooks

/* ---------- public code */

void cseries_initialize(void)
{
	initialize_thread_management();
	initialize_synchronization_objects();
}

void cseries_dispose(void)
{
	REX_PPC_INVOKE(cseries_dispose);
}

/* ---------- private code */
