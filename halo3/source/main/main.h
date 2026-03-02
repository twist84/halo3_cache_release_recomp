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
	k_map_memory_configuration_count = 4
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

struct _main_globals
{
	bool startup_sequence_performed;
	bool halted_with_errors;
	bool run_xdemos;
	bool exit_game;
	bool save_core;
	bool load_core;
	char core_file_name[64];
	bool reset_map;
	bool reset_map_random_seed;
	bool revert_map;
	bool save_map;
	bool save_map_and_exit;
	bool skip_cinematic;
	bool prepare_to_switch_zone_set;
	volatile bool switch_zone_set;
	bool reverted_by_scripting;
	bool keep_playing_cinematic_outros_on_revert;
	bool ui_saving_files;
	bool byte_828D7B19;
	bool byte_828D7B1A;
	bool cheat_drop_tag;
	rex::be<long> switch_zone_set_index;
	rex::be<long> dword_828D7B20;
	rex::be<long> deactivating_designer_zone_mask;
	rex::be<long> cheat_drop_tag_index;
};
static_assert(sizeof(_main_globals) == 0x64);

// main game

struct s_main_game_globals
{
	rex::be<e_map_memory_configuration> map_memory_configuration; // 0x0
	rex::be<long> game_loaded_status; // 0x4
	char game_loaded_scenario_path[260]; // 0x8
	bool map_reset_in_progress; // 0x10C
	bool map_advance_pending; // 0x10D
	bool map_change_pending; // 0x10E
	bool map_change_pending_unload; // 0x10F
	rex::be<unsigned long> map_change_initiate_time; // 0x110
	rex::be<unsigned long> map_change_timer; // 0x114
	game_options pending_game_options; // 0x118
};
static_assert(sizeof(s_main_game_globals) == 0xD418);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

extern unlock_token _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern void unlock_resources_and_resume_render_thread(unlock_token token);

// main game

extern void main_game_unload_and_prepare_for_next_game(game_options const* options);

/* ---------- private code */

#endif // __MAIN_H__
