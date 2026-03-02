/* ---------- headers */

#include "main/main.h"

#include "cseries/cseries_string.h"
#include "cache/physical_memory_map.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "scenario/scenario.h"
#include "simulation/simulation.h"

#include "rex_macros.h"

/* ---------- constants */

#define MAIN_GAME_GLOBALS() REX_DATA_REFERENCE(0x828D7B30, s_main_game_globals, main_game_globals)

/* ---------- definitions */

/*
struct s_main_game_globals
{
	e_map_memory_configuration map_memory_configuration; // 0x0
	long game_loaded_status; // 0x4
	char game_loaded_scenario_path[260]; // 0x8
	bool map_reset_in_progress; // 0x10C
	bool map_advance_pending; // 0x10D
	bool map_change_pending; // 0x10E
	bool map_change_pending_unload; // 0x10F
	unsigned long map_change_initiate_time; // 0x110
	unsigned long map_change_timer; // 0x114
	game_options pending_game_options; // 0x118
	bool load_core_on_game_launch; // 0xD418
	char core_name[255]; // 0xD419
	long launch_player_count; // 0xD518
	char pad[4]; // 0xD51C
	game_options launch_game_options; // 0xD520
};
static_assert(sizeof(s_main_game_globals) == 0x1A820);
*/

/* ---------- prototypes */

REX_PPC_EXTERN_IMPORT(main_game_configure_map_memory);
REX_PPC_EXTERN_IMPORT(main_game_internal_close_caches);
REX_PPC_EXTERN_IMPORT(main_game_unload_and_prepare_for_next_game);
REX_PPC_EXTERN_IMPORT(data_mine_insert_single_player_game_options);

static void data_mine_insert_single_player_game_options(char const* event_name);

/* ---------- globals */

/* ---------- private variables */

//static
//{
//    //extern s_main_game_globals main_game_globals;
//}

/* ---------- public code */

void main_game_configure_map_memory(game_options const* options)
{
	REX_PPC_INVOKE(void, main_game_configure_map_memory, options);
}

void main_game_internal_close_caches(void)
{
	REX_PPC_INVOKE(void, main_game_internal_close_caches);

	//tag_resources_prepare_for_new_map();
	//cache_file_tag_resources_dispose_from_old_map();
	//scenario_unload();
}

void main_game_internal_map_unload_begin(void)
{
	MAIN_GAME_GLOBALS();

	assert(main_game_globals.game_loaded_status == _game_loaded_status_map_loaded);
	//assert(!bink_playback_active());

	main_game_internal_close_caches();
	main_game_globals.game_loaded_status = _game_loaded_status_map_unloading;
	csstrnzcpy(main_game_globals.game_loaded_scenario_path, "", NUMBEROF(main_game_globals.game_loaded_scenario_path));
}

void main_game_internal_map_unload_complete(void)
{
	MAIN_GAME_GLOBALS();

	assert(main_game_globals.game_loaded_status == _game_loaded_status_map_unloading);

	physical_memory_stage_pop(_memory_stage_level_initialize);
	main_game_globals.game_loaded_status = 0;
}

void main_game_internal_pregame_unload(void)
{
	MAIN_GAME_GLOBALS();

	assert(main_game_globals.game_loaded_status == _game_loaded_status_pregame);

	physical_memory_stage_pop(_memory_stage_level_initialize);
	physical_memory_stage_pop(_memory_stage_map_configuration);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

void main_game_unload_and_prepare_for_next_game(game_options const* options)
{
	MAIN_GAME_GLOBALS();

	WAIT_FOR_RENDER_THREAD();

	if (game_in_progress())
	{
		data_mine_insert_single_player_game_options("game finish");
		game_engine_game_ending();
		simulation_stop();
		scenario_switch_zone_set(-1);
		game_dispose_from_old_map();
	}

	long game_loaded_status = main_game_globals.game_loaded_status;
	switch (game_loaded_status)
	{
	case _game_loaded_status_none:
		break;
	case _game_loaded_status_map_loading:
	case _game_loaded_status_map_loaded_failure:
	case _game_loaded_status_map_unloading:
		return;
	case _game_loaded_status_map_loaded:
	{
		main_game_internal_map_unload_begin();
		scenario_unload();
		main_game_internal_map_unload_complete();
	}
	break;
	case _game_loaded_status_pregame:
	{
		main_game_internal_pregame_unload();
	}
	break;
	default:
		throw "unreachable";
	}

	assert(main_game_globals.game_loaded_status == _game_loaded_status_none);
	main_game_configure_map_memory(options);
}

c_wait_for_render_thread::c_wait_for_render_thread(char const* file, long line) :
	m_token(_internal_halt_render_thread_and_lock_resources(file, line))
{
}

c_wait_for_render_thread::~c_wait_for_render_thread(void)
{
	unlock_resources_and_resume_render_thread(m_token);
}

/* ---------- private code */

static void data_mine_insert_single_player_game_options(char const* event_name)
{
	REX_PPC_INVOKE(void, data_mine_insert_single_player_game_options, event_name);
}
