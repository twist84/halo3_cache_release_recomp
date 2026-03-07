/* ---------- headers */

#include "math/real_math.h"
#include "objects/objects.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(object_get_origin);

// hooks

/* ---------- public code */

real_point3d* object_get_origin(long object_index, real_point3d* origin)
{
	return REX_PPC_INVOKE(object_get_origin, object_index, origin);
}

/* ---------- private code */
