// halo3_cache_release - ReXGlue Recompiled Project
//
// This file is yours to edit. 'rexglue migrate' will NOT overwrite it.

#include "generated/halo3_cache_release_config.h"
#include "generated/halo3_cache_release_init.h"

#include "halo3_cache_release_app.h"

REX_DEFINE_APP(halo3_cache_release, Halo3CacheReleaseApp::Create)

// BLAM!

#pragma region includes

#include "main/main.h"
#include "render/screen_postprocess.h"

#include "rex_macros.h"

#pragma endregion

#pragma region game_stubs

PPC_STUB(rex_saved_film_directory_delete_contents)
PPC_STUB(rex_autosave_queue_save_current_game_variant_to_queue)

#pragma endregion

#pragma region QoL

void post_setup_callback(void)
{
	c_screen_postprocess::x_settings_internal.m_postprocess = false;
	disable_main_loop_throttle = true;
}

#pragma endregion

#pragma region ppc_hooks

REXCVAR_DEFINE_STRING(username, "", "RexUser", "");

namespace rex::kernel::xam
{
	extern ppc_u32_result_t XamUserGetName_entry(ppc_u32_t user_index, ppc_pchar_t buffer,
		ppc_u32_t buffer_len);
}

ppc_u32_result_t XUserGetName(ppc_u32_t user_index, ppc_pchar_t buffer,
	ppc_u32_t buffer_len)
{
	ppc_u32_result_t result;

	auto username = REXCVAR_GET(username);
	if (username[0] != 0)
	{
		rex::string::util_copy_truncating(buffer, username, std::min(buffer_len.value(), uint32_t(16)));

		// X_E_SUCCESS
		result = 0U;
	}
	else
	{
		result = rex::kernel::xam::XamUserGetName_entry(user_index, buffer, buffer_len);
	}
	return result;
}
REX_PPC_HOOK(XUserGetName);

#pragma endregion

/*
	error G211B3073: use of undeclared label 'loc_8220CAA0'

	$IMPLEMENT c_character_physics_component::set_mode
	switch (old_mode)
	{
		case _mode_posture:
		case _mode_climbing:
			get_mode_sentinel()->dispose(m_object_index);
			break;
	}

loc_8220CAA0:
	// addi r3,r31,8 # this
	ctx.r3.s64 = ctx.r31.s64 + 8;
	// lwz r4,4(r31) # m_object_index
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x82227438
	ctx.lr = 0x8220CAAC;
	rex_c_character_physics_mode_sentinel_datum_dispose(ctx, base);
loc_8220CAAC:
*/
