/* ---------- headers */

#include "physics/character_physics.h"

#include "cseries/constants.h"
#include "cseries/cseries_macros.h"
#include "math/real_math.h"
#include "objects/objects.h"
#include "physics/havok.h"

#include "rex_macros.h"

#include <cassert>

/* ---------- constants */

/* ---------- definitions */

// $TODO move these to actual homes

class c_animation_id
{
public:
private:
	rex::be<short> m_subgraph; // 0x0
	rex::be<short> m_index; // 0x2
};
static_assert(sizeof(c_animation_id) == 4);

class c_global_material_type
{
public:

private:
	rex::be<short> m_index; // 0x0
};
static_assert(sizeof(c_global_material_type) == 2);

class c_character_physics_mode_ground_datum
{
public:
	enum e_contact_type
	{
		_contact_type_none = 0,
		_contact_type_structure,
		_contact_type_object,
		_contact_type_instanced_geometry,
		k_contact_type_count,
	};

public:
	void initialize();
	void dispose();

private:
	real_vector3d m_last_localized_physics_velocity; // 0x0
	rex::be<long> m_last_time_animation_velocity; // 0xC
	real_vector3d m_last_animation_velocity; // 0x10
	rex::be<long> m_support_havok_component_index; // 0x1C
	rex::be<long> m_support_havok_component_rigid_body_index; // 0x20
	real_matrix4x3 m_support_havok_component_matrix; // 0x24
	rex::be<long> m_surface_index; // 0x58
	rex::be<long> m_support_object_index; // 0x5C
	c_global_material_type m_ground_material_type; // 0x60
	unsigned char m_ground_physics_update_ticks; // 0x62
	unsigned char m_flags; // 0x63
	real_vector3d m_ground_plane; // 0x64
	rex::be<float> m_landing_velocity; // 0x70
};
static_assert(sizeof(c_character_physics_mode_ground_datum) == 116);

class c_character_physics_mode_flying_datum
{
public:
	void initialize();
	void dispose();

private:
	rex::be<long> m_turning_disabled_counter; // 0x0
	real_vector3d m_uprighting_angular_velocity; // 0x4
};
static_assert(sizeof(c_character_physics_mode_flying_datum) == 16);

class c_character_physics_mode_dead_datum
{
public:
	void initialize();
	void dispose();

private:
	real_vector3d m_resting_surface_normal; // 0x0
	unsigned char m_on_restable_slope_ticks; // 0xC
	unsigned char m_contact_with_ground_ticks; // 0xD
	unsigned char m_relaxation_ticks; // 0xE
	unsigned char m_active_ticks; // 0xF
	unsigned char m_near_zero_velocity_ticks; // 0x10
	bool m_on_fixed_surface; // 0x11
	unsigned char m_dead_size; // 0x12
	real_vector3d m_local_physics_last_velocity; // 0x14
	real_point3d m_object_relative_safe_position; // 0x20
	rex::be<long> m_z_transition_start_time; // 0x2C
	rex::be<long> m_time_of_death_node_accelertion; // 0x30
	rex::be<long> m_death_node_index; // 0x34
	real_vector3d m_death_node_acceleration; // 0x38
};
static_assert(sizeof(c_character_physics_mode_dead_datum) == 68);

class c_character_physics_mode_sentinel_datum
{
public:
	void initialize(bool climbing, real_point3d const* sentinel_physics_position);
	void dispose(long object_index);

private:
	real_point3d m_sentinel_physics_position; // 0x0
	real_vector3d m_accumulated_sentinel_physics_translation; // 0xC
	real_vector3d m_sentinel_velocity; // 0x18
	c_animation_id m_animation_id; // 0x24
	bool m_climbing; // 0x28
	bool m_accumulated_sentinel_physics_translation_flag; // 0x29
};
static_assert(sizeof(c_character_physics_mode_sentinel_datum) == 44);

class c_character_physics_mode_melee_datum
{
public:
	void initialize();
	void dispose();

private:
	rex::be<unsigned long> m_flags; // 0x0
	rex::be<long> m_time_to_target; // 0x4
	rex::be<long> m_counter; // 0x8
	rex::be<long> m_maximum_counter; // 0xC
	real_point3d __unknown10; // 0x10
	real_vector3d __unknown1C; // 0x1C
	rex::be<float> m_initial_target_velocity_dot_vector; // 0x28
	rex::be<float> __unknown2C; // 0x2C
	real_point3d __unknown30; // 0x30
	real_vector3d __unknown3C; // 0x3C
	rex::be<float> m_maximum_distance; // 0x48
	rex::be<float> m_deceleration_per_tick; // 0x4C
	rex::be<float> m_deceleration_end_speed_per_tick; // 0x50
	real_vector3d m_initial_aiming_vector; // 0x54
	rex::be<long> m_melee_target_unit_index; // 0x60
	rex::be<long> m_exit_invisible_crouched_uber_melee_offset; // 0x64
};
static_assert(sizeof(c_character_physics_mode_melee_datum) == 0x68);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

// hooks

// $TODO c_character_physics_component::set_mode

/* ---------- public code */

void c_character_physics_component::initialize(long object_index)
{
	m_object_index = object_index;
	m_mode = _mode_none;
	m_flags = 0;
	assert(m_object_index != NONE);
}

#if 0
void c_character_physics_component::set_mode(e_mode mode)
{
	e_mode old_mode = get_mode();
	switch (old_mode)
	{
	case _mode_ground:
		get_mode_ground()->dispose();
		break;

	case _mode_flying:
		get_mode_flying()->dispose();
		break;

	case _mode_dead:
		get_mode_dead()->dispose();
		break;

	case _mode_posture:
	case _mode_climbing:
		get_mode_sentinel()->dispose(m_object_index);
		break;

	case _mode_melee:
		get_mode_melee()->dispose();
		break;

	default:
		throw "unreachable";
		break;
	}

	m_mode = mode;

	switch (get_mode())
	{
	case _mode_ground:
		get_mode_ground()->initialize();
		break;

	case _mode_flying:
		get_mode_flying()->initialize();
		break;

	case _mode_dead:
		get_mode_dead()->initialize();
		break;

	case _mode_posture:
	case _mode_climbing:
	{
		real_point3d position = {};
		object_get_origin(m_object_index, &position);
		get_mode_sentinel()->initialize(get_mode() == _mode_climbing, &position);
	}
	break;

	case _mode_melee:
		get_mode_melee()->initialize();
		break;

	default:
		throw "unreachable";
		break;
	}

	if (old_mode != get_mode())
	{
		if (old_mode == _mode_dead || get_mode() == _mode_dead
			|| old_mode == _mode_posture || old_mode == _mode_climbing
			|| get_mode() == _mode_posture || get_mode() == _mode_climbing)
		{
			havok_object_rebuild(m_object_index);
		}
	}
}
#endif

c_character_physics_component::e_mode c_character_physics_component::get_mode() const
{
	assert(IN_RANGE_INCLUSIVE(m_mode, _mode_first, _mode_last));

	return static_cast<e_mode>(m_mode);
}

bool c_character_physics_component::is_sentinel_mode() const
{
	e_mode mode = get_mode();
	return mode == _mode_posture || mode == _mode_climbing;
}

bool c_character_physics_component::is_immune_to_collision_damage() const
{
	return m_collision_damage_imunity_counter != 0;
}

c_character_physics_mode_ground_datum *c_character_physics_component::get_mode_ground()
{
	assert(get_mode() == _mode_ground);
	return reinterpret_cast<c_character_physics_mode_ground_datum*>(m_character_physics_mode_datum_buffer);
}

c_character_physics_mode_flying_datum *c_character_physics_component::get_mode_flying()
{
	assert(get_mode() == _mode_flying);
	return reinterpret_cast<c_character_physics_mode_flying_datum*>(m_character_physics_mode_datum_buffer);
}

c_character_physics_mode_dead_datum *c_character_physics_component::get_mode_dead()
{
	assert(get_mode() == _mode_dead);
	return reinterpret_cast<c_character_physics_mode_dead_datum*>(m_character_physics_mode_datum_buffer);
}

c_character_physics_mode_sentinel_datum *c_character_physics_component::get_mode_sentinel()
{
	assert(is_sentinel_mode());
	return reinterpret_cast<c_character_physics_mode_sentinel_datum*>(m_character_physics_mode_datum_buffer);
}

c_character_physics_mode_melee_datum *c_character_physics_component::get_mode_melee()
{
	assert(get_mode() == _mode_melee);
	return reinterpret_cast<c_character_physics_mode_melee_datum*>(m_character_physics_mode_datum_buffer);
}

/* ---------- private code */
