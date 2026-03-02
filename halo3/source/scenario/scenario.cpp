/* ---------- headers */

#include "scenario/scenario.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

REX_PPC_EXTERN_IMPORT(scenario_switch_zone_set_internal);
REX_PPC_EXTERN_IMPORT(scenario_unload);

static bool scenario_switch_zone_set_internal(long new_zone_set_index, bool unload_old_bsps);

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

bool scenario_switch_zone_set(long zone_set_index)
{
	return scenario_switch_zone_set_internal(zone_set_index, true);
}

static bool scenario_switch_zone_set_internal(long new_zone_set_index, bool unload_old_bsps)
{
	return REX_PPC_INVOKE(bool, scenario_switch_zone_set_internal, new_zone_set_index, unload_old_bsps);
}

void scenario_unload(void)
{
	REX_PPC_INVOKE(void, scenario_unload);
}

/* ---------- private code */
