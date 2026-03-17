#ifndef __MAIN_H__
#define __MAIN_H__
#pragma once

/* ---------- headers */

#include "game/game_options.h"
#include "cseries/cseries_macros.h"

#include <rex/types.h>

/* ---------- constants */

#define WAIT_FOR_RENDER_THREAD() c_wait_for_render_thread CONCATX(__render_thread_lock_, __LINE__)(__FILE__, __LINE__)

enum e_map_memory_configuration
{
	_map_memory_configuration_none = 0,
	_map_memory_configuration_main_menu,
	_map_memory_configuration_multiplayer,
	_map_memory_configuration_campaign,

	k_map_memory_configuration_count
};

enum
{
	_game_loaded_status_none = 0,
	_game_loaded_status_map_loading,
	_game_loaded_status_map_loaded,
	_game_loaded_status_map_loaded_failure,
	_game_loaded_status_map_unloading,
	_game_loaded_status_pregame,

	k_game_loaded_status_count,
};

/* ---------- definitions */

typedef unsigned long unlock_token;
class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(char const* file, long line);
	~c_wait_for_render_thread(void);
private:
	unlock_token m_token;
};
static_assert(sizeof(c_wait_for_render_thread) == 4);

/* ---------- prototypes */

/* ---------- globals */

extern bool& disable_main_loop_throttle;

/* ---------- public code */

extern unlock_token _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern void unlock_resources_and_resume_render_thread(unlock_token token);
extern void main_loop(void);

// main game

extern void main_game_unload_and_prepare_for_next_game(game_options const* options);

/* ---------- private code */

#endif // __MAIN_H__
