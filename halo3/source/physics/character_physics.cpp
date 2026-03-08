/* ---------- headers */

#include "physics/character_physics.h"

#include "rex_macros.h"

#include <cassert>

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(rex_c_character_physics_mode_sentinel_datum_dispose);

// hooks

// midasm_hooks

bool midasm_hook__c_character_physics_component_set_mode()
{
	REX_PPC_CONTEXT_REF(ctx);
	REX_PPC_MEMBASE_PTR(base);

	bool result;
	if (ctx.r11.u32 == c_character_physics_component::_mode_posture ||
		ctx.r11.u32 == c_character_physics_component::_mode_climbing)
	{
		// addi r3,r31,8 # this
		ctx.r3.s64 = ctx.r31.s64 + 8;

		// lwz r4,4(r31) # m_object_index
		ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);

		// bl 0x82227438
		ctx.lr = 0x8220CAAC;
		rex_c_character_physics_mode_sentinel_datum_dispose(ctx, base);

		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

/* ---------- public code */

/* ---------- private code */
