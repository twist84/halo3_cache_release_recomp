/* ---------- headers */

#include "main/main.h"

#include "cseries/cseries_string.h"
#include "cache/physical_memory_map.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "interface/user_interface.h"
#include "networking/network_memory.h"
#include "networking/online/online_files.h"
#include "scenario/scenario.h"
#include "simulation/simulation.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct s_configure_memory
{
	void(*dispose_memory_proc)(); // 0x0
	void(*configure_memory_proc)(e_map_memory_configuration); // 0x4
};
static_assert(sizeof(s_configure_memory) == 0x10);

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

e_map_memory_configuration compute_desired_map_memory_configuration(const game_options* options);

static void main_game_configure_map_memory(game_options const* options);
static void main_game_configure_map_memory_pop();
static void main_game_configure_map_memory_push(e_map_memory_configuration desired_memory_configuration);
static void main_game_internal_close_caches();
static void main_game_internal_pregame_unload();
static void main_game_internal_map_unload_begin();
static void main_game_internal_map_unload_complete();
static void data_mine_insert_single_player_game_options(char const* event_name);

/* ---------- globals */

/* ---------- private variables */

static s_configure_memory const g_configure_memory_procs[] =
{
	{ network_memory_shared_dispose, network_memory_shared_initialize },
	{ user_interface_memory_dispose, user_interface_memory_initialize },
	{ online_files_memory_dispose, online_files_memory_initialize }
};

static REX_DATA_REFERENCE_DECLARE(0x828D7B30, s_main_game_globals, main_game_globals);

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(main_game_configure_map_memory);
REX_PPC_EXTERN_IMPORT(main_game_internal_close_caches);
REX_PPC_EXTERN_IMPORT(main_game_unload_and_prepare_for_next_game);
REX_PPC_EXTERN_IMPORT(data_mine_insert_single_player_game_options);

// hooks

REX_PPC_HOOK(main_game_unload_and_prepare_for_next_game);

/* ---------- public code */

e_map_memory_configuration compute_desired_map_memory_configuration(const game_options* options)
{
	e_map_memory_configuration desired_memory_configuration;
	if (options != nullptr)
	{
		switch (options->game_mode)
		{
		case _game_mode_campaign:
			desired_memory_configuration = _map_memory_configuration_campaign;
			break;
		case _game_mode_multiplayer:
			desired_memory_configuration = _map_memory_configuration_multiplayer;
			break;
		case _game_mode_ui_shell:
			desired_memory_configuration = _map_memory_configuration_main_menu;
			break;
		default:
			break;
		}
	}
	else
	{
		desired_memory_configuration = _map_memory_configuration_none;
	}
	return desired_memory_configuration;
}

void main_game_configure_map_memory(game_options const* options)
{
	//REX_PPC_INVOKE(main_game_configure_map_memory, options);

	e_map_memory_configuration desired_memory_configuration = compute_desired_map_memory_configuration(options);
	if (main_game_globals.map_memory_configuration != desired_memory_configuration)
	{
		main_game_configure_map_memory_pop();
		main_game_configure_map_memory_push(desired_memory_configuration);
	}
}

void main_game_configure_map_memory_pop()
{
	if (main_game_globals.map_memory_configuration > _map_memory_configuration_none)
	{
		for (long index = NUMBEROF(g_configure_memory_procs) - 1; index >= 0; index--)
		{
			if (g_configure_memory_procs[index].dispose_memory_proc)
			{
				g_configure_memory_procs[index].dispose_memory_proc();
			}
		}

		physical_memory_stage_pop(_memory_stage_map_configuration);
		main_game_globals.map_memory_configuration = _map_memory_configuration_none;
	}
}

void main_game_configure_map_memory_push(e_map_memory_configuration desired_memory_configuration)
{
	if (desired_memory_configuration > _map_memory_configuration_none)
	{
		physical_memory_stage_push(_memory_stage_map_configuration);
		main_game_globals.map_memory_configuration = desired_memory_configuration;

		for (long index = 0; index < NUMBEROF(g_configure_memory_procs); index++)
		{
			if (g_configure_memory_procs[index].configure_memory_proc)
			{
				g_configure_memory_procs[index].configure_memory_proc(desired_memory_configuration);
			}
		}
	}
}

void main_game_internal_close_caches(void)
{
	REX_PPC_INVOKE(main_game_internal_close_caches);

	//tag_resources_prepare_for_new_map();
	//cache_file_tag_resources_dispose_from_old_map();
	//scenario_unload();
}

void main_game_internal_map_unload_begin(void)
{
	assert(main_game_globals.game_loaded_status == _game_loaded_status_map_loaded);
	//assert(!bink_playback_active());

	main_game_internal_close_caches();
	main_game_globals.game_loaded_status = _game_loaded_status_map_unloading;
	csstrnzcpy(main_game_globals.game_loaded_scenario_path, "", NUMBEROF(main_game_globals.game_loaded_scenario_path));
}

void main_game_internal_map_unload_complete(void)
{
	assert(main_game_globals.game_loaded_status == _game_loaded_status_map_unloading);

	physical_memory_stage_pop(_memory_stage_level_initialize);
	main_game_globals.game_loaded_status = 0;
}

void main_game_internal_pregame_unload(void)
{
	assert(main_game_globals.game_loaded_status == _game_loaded_status_pregame);

	physical_memory_stage_pop(_memory_stage_level_initialize);
	physical_memory_stage_pop(_memory_stage_map_configuration);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

void main_game_unload_and_prepare_for_next_game(game_options const* options)
{
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
	REX_PPC_INVOKE(data_mine_insert_single_player_game_options, event_name);
}
