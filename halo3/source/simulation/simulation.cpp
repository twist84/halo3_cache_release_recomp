/* ---------- headers */

#include "simulation/simulation.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

REX_PPC_EXTERN_IMPORT(simulation_stop);

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

void simulation_stop()
{
	REX_PPC_INVOKE(void, simulation_stop);
}

/* ---------- private code */
