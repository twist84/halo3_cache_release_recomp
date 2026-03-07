#ifndef __CHARACTER_PHYSICS_H__
#define __CHARACTER_PHYSICS_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_character_physics_override
{
	_character_physics_default = 0,
	_character_physics_player,
	_character_physics_biped,

	k_total_character_physics_overrides,
};

/* ---------- definitions */

class c_character_physics_mode_ground_datum;
class c_character_physics_mode_flying_datum;
class c_character_physics_mode_dead_datum;
class c_character_physics_mode_sentinel_datum;
class c_character_physics_mode_melee_datum;

class c_character_physics_component
{
public:
	enum e_mode
	{
		_mode_none = 0,

		_mode_first,

		_mode_ground = _mode_first,
		_mode_flying,
		_mode_dead,
		_mode_posture,
		_mode_climbing,
		_mode_melee,

		_mode_last = _mode_melee,

		k_mode_count,
	};

	enum e_flag
	{
		_slipping_bit = 0,
		_no_contact_with_ground_bit,

		k_flag_count,
	};

public:
	void initialize(long object_index);
	void set_mode(e_mode mode);
	e_mode get_mode() const;
	bool is_sentinel_mode() const;
	bool is_immune_to_collision_damage() const;
	c_character_physics_mode_ground_datum* get_mode_ground();
	c_character_physics_mode_flying_datum* get_mode_flying();
	c_character_physics_mode_dead_datum* get_mode_dead();
	c_character_physics_mode_sentinel_datum* get_mode_sentinel();
	c_character_physics_mode_melee_datum* get_mode_melee();

public:
	short m_flags; // 0x0

private:
	unsigned char m_mode; // 0x2
	unsigned char m_collision_damage_imunity_counter; // 0x3
	long m_object_index; // 0x4
	unsigned long m_character_physics_mode_datum_buffer[30 /* confirm this */]; // 0x8
};
static_assert(sizeof(c_character_physics_component) == 0x80);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __CHARACTER_PHYSICS_H__
