#ifndef __GAME_H__
#define __GAME_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign = 1,
	_game_mode_multiplayer = 2,
	_game_mode_ui_shell = 3,
	_game_mode_tool = 4,

	k_game_mode_count = 5,
	k_game_mode_bits = 3,
};

/* ---------- definitions */

/* ---------- prototypes */

extern void __tls_set_g_game_globals_allocator(void* new_address);
extern void game_dispose_from_old_map(void);
extern bool game_in_progress(void);
extern void game_initialize(void);

// game_time

extern void __tls_set_g_game_time_globals_allocator(void* new_address);
extern bool game_time_get_paused();

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __GAME_H__
