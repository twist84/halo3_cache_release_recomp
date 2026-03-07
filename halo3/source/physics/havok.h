#ifndef __HAVOK_H__
#define __HAVOK_H__
#pragma once

/* ---------- headers */

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct s_havok_constants
{
	rex::be<float> havok_collision_tolerance;
	rex::be<float> havok_iterative_linear_cast_early_out_distance;
	rex::be<long> havok_environment_type;
	rex::be<long> havok_debug_mode;
	rex::be<float> havok_shape_radius;
	bool havok_disable_deactivation;
	bool havok_jumping_beans;
	bool havok_weld_environment;
	bool havok_enable_back_stepping;
	bool havok_enable_sweep_shapes;
	bool havok_display_stats;
	bool havok_initialize_profiling;
	bool havok_render_impacts;
	bool havok_render_proxies;
	bool havok_render_collision_damage;
	bool havok_play_impact_effects;
	bool havok_play_biped_impact_effects;
	bool havok_render_shape_properties;
	bool havok_cleanup_inactive_agents;
	bool havok_batch_add_entities_disabled;
	rex::be<float> havok_maximum_linear_velocity;
	rex::be<float> havok_minimum_maximum_penetration_depth;
	rex::be<long> havok_thread_count;
};
static_assert(sizeof(s_havok_constants) == 0x30);

/* ---------- prototypes */

extern void havok_object_rebuild(long object_index);
extern void havok_object_rebuild(long object_index, long flags);

/* ---------- globals */

extern s_havok_constants& g_havok_constants;

/* ---------- public code */

/* ---------- private code */

#endif // __HAVOK_H__
