/* ---------- headers */

#include "game/game.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

REX_PPC_EXTERN_IMPORT(game_dispose_from_old_map);
REX_PPC_EXTERN_IMPORT(game_in_progress);

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

void game_dispose_from_old_map(void)
{
	REX_PPC_INVOKE(void, game_dispose_from_old_map);
}

bool game_in_progress(void)
{
	return REX_PPC_INVOKE(bool, game_in_progress);
}

/* ---------- private code */
