/* ---------- headers */

#include "cseries/cseries_windows.h"

#include "rex_macros.h"

#include <time.h>

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(GetTickCount);

// hooks

/* ---------- public code */

unsigned long system_milliseconds(void)
{
	return GetTickCount();
}

/* ---------- private code */
