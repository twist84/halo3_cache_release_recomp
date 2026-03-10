/* ---------- headers */

#include "game/game.h"

#include "rex_macros.h"

#include <cassert>

/* ---------- constants */

/* ---------- definitions */

struct game_time_globals_definition
{
	bool initialized; // 0x0
	bool paused; // 0x1
	short tick_rate; // 0x2
	float tick_length; // 0x4
	long time; // 0x8
	float speed; // 0xC
	float leftover_ticks; // 0x10
	float rate_scale_timer; // 0x14
	float rate_scale_duration; // 0x18
	float rate_scale_initial; // 0x1C
	float rate_scale_final; // 0x20
	long game_message_tick; // 0x24
};
static_assert(sizeof(game_time_globals_definition) == 0x28);

/* ---------- prototypes */

/* ---------- globals */

thread_local game_time_globals_definition* game_time_globals = nullptr;

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(__tls_set_g_game_time_globals_allocator);

// hooks

REX_PPC_HOOK(__tls_set_g_game_time_globals_allocator);

/* ---------- public code */

void __tls_set_g_game_time_globals_allocator(void* new_address)
{
	REX_PPC_INVOKE(__tls_set_g_game_time_globals_allocator, new_address);

	game_time_globals = static_cast<game_time_globals_definition *>(new_address);
}

bool game_time_get_paused()
{
	assert(game_time_globals != nullptr);

	bool game_time_is_paused = game_time_globals->initialized && game_time_globals->paused;
	return game_time_is_paused;
}

/* ---------- private code */
