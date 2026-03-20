/* ---------- headers */

#include "camera/director.h"

#include "cseries/constants.h"
#include "input/controllers.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

static e_director_mode choose_appropriate_director(long user_index);
static void change_debug_director_if_necessary(long user_index);

/* ---------- globals */

REXCVAR_DEFINE_INT32(director_mode_override, NONE, "Blam/Camera", "");

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(director_update);
REX_PPC_EXTERN_IMPORT(director_set_mode);

REX_PPC_EXTERN_IMPORT(choose_appropriate_director);

// hooks

REX_PPC_HOOK(director_update);

/* ---------- public code */

void director_update(float dt)
{
	REX_PPC_INVOKE(director_update, dt);

#if 1
	long user_index = 0;
	change_debug_director_if_necessary(user_index);
#else
	for (long user_index = 0; user_index < 4; user_index++)
	{
		if (players_user_is_active(user_index))
		{
			change_debug_director_if_necessary(user_index);
			director_get(user_index)->update(dt);
		}
	}
#endif
}

void director_set_mode(long user_index, e_director_mode director_mode)
{
	REX_PPC_INVOKE(director_set_mode, user_index, director_mode);
}

/* ---------- private code */

static e_director_mode choose_appropriate_director(long user_index)
{
	e_director_mode desired_mode = REX_PPC_INVOKE(choose_appropriate_director, user_index);
	return desired_mode;
}

static void change_debug_director_if_necessary(long user_index)
{
#if 1
	long director_mode = REXCVAR_GET(director_mode_override);
	if (director_mode != NONE)
	{
		director_set_mode(user_index, static_cast<e_director_mode>(director_mode));
	}
#else
	// $IMPLEMENT
	(void)(director_mode_override);
#endif
}
