/* ---------- headers */

#include "game/game.h"

#include "game/game_options.h"
#include "memory/thread_local_storage.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct game_globals_storage
{
	bool initializing;
	bool map_active;
	bool __unknown2;
	bool __unknown3;
	rex::be<unsigned long> active_structure_bsp_mask;
	rex::be<unsigned long> active_designer_zone_mask;
	rex::be<unsigned long> active_cinematic_zone_mask;
	game_options options;
	bool game_in_progress;
	bool game_lost;
	bool game_revert;
	bool __unknownD313;
	rex::be<long> game_lost_wait_time;
	bool game_finished;
	bool __unknownD319;
	bool __unknownD31A;
	bool __unknownD31B;
	rex::be<long> game_finished_wait_time;
	rex::be<long> game_ragdoll_count;
	char cluster_pvs[512];
	char cluster_pvs_local[512];
	char cluster_activation[512];
	bool __unknownD924;
	bool had_an_update_tick_this_frame;
	bool __unknownD926;
	bool __unknownD927;
	bool scripted_camera_pvs;
	bool __unknownD929;
	rex::be<unsigned short> scripted;
	rex::be<unsigned long> scripted_object_index;
};
static_assert(sizeof(game_globals_storage) == 0xD930);

/* ---------- prototypes */

/* ---------- globals */

thread_local game_globals_storage* game_globals = nullptr;

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(__tls_set_g_game_globals_allocator);
REX_PPC_EXTERN_IMPORT(game_dispose_from_old_map);
REX_PPC_EXTERN_IMPORT(game_in_progress);
REX_PPC_EXTERN_IMPORT(game_initialize);

// hooks

REX_PPC_HOOK(__tls_set_g_game_globals_allocator);
REX_PPC_HOOK(game_in_progress);

/* ---------- public code */

void __tls_set_g_game_globals_allocator(void* new_address)
{
	REX_PPC_INVOKE(__tls_set_g_game_globals_allocator, new_address);

	game_globals = static_cast<game_globals_storage*>(new_address);
}

void game_dispose_from_old_map(void)
{
	REX_PPC_INVOKE(game_dispose_from_old_map);
}

bool game_in_progress(void)
{
	bool result;
	if (game_globals != nullptr)
	{
		result = game_globals->game_in_progress;
	}
	else
	{
		result = false;
	}
	return result;
}

void game_initialize(void)
{
	REX_PPC_INVOKE(game_initialize);
}

bool game_is_ui_shell()
{
	bool result;
	if (game_globals != nullptr)
	{
		result = game_globals->options.game_mode == _game_mode_ui_shell;
	}
	else
	{
		result = false;
	}
	return result;
}

/* ---------- private code */
