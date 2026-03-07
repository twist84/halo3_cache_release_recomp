/* ---------- headers */

#include "physics/havok.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

REX_DATA_REFERENCE_DECLARE(0x826F943C, s_havok_constants, g_havok_constants);

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(havok_object_rebuild);

// hooks

/* ---------- public code */

void havok_object_rebuild(long object_index)
{
    havok_object_rebuild(object_index, 0);
}

void havok_object_rebuild(long object_index, long flags)
{
    REX_PPC_INVOKE2(void, havok_object_rebuild, object_index, flags);
}

/* ---------- private code */
