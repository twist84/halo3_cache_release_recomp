#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_director_mode
{
	_director_mode_game = 0,
	_director_mode_saved_film,
	_director_mode_observer,
	_director_mode_debug,
	_director_mode_cable_cam,
	_director_mode_editor,

	k_number_of_director_modes,
};

/* ---------- definitions */

/* ---------- prototypes */

extern void director_update(float dt);
extern void director_set_mode(long user_index, e_director_mode director_mode);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __DIRECTOR_H__
