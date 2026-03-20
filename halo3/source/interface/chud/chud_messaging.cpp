/* ---------- headers */

#include "interface/chud/chud_messaging.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

static void chud_post_action_response(long user_index);

/* ---------- globals */

REXCVAR_DEFINE_BOOL(chud_debug_messages, false, "Blam/Interface/Chud", "");

//bool chud_debug_messages;
REX_DATA_REFERENCE_DECLARE(0x82881D7B, bool, chud_debug_messages);

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(chud_post_action_response);

// hooks

REX_PPC_HOOK(chud_post_action_response);

/* ---------- public code */

/* ---------- private code */

static void chud_post_action_response(long user_index)
{
	chud_debug_messages = REXCVAR_GET(chud_debug_messages);

	REX_PPC_INVOKE(chud_post_action_response, user_index);
}
