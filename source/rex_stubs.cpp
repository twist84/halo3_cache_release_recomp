#pragma region includes

#include "cseries/cseries_macros.h"
#include "input/input_constants.h"
#include "interface/gui_string_ids.h"
#include "interface/user_interface.h"
#include "main/main.h"
#include "render/screen_postprocess.h"
#include "tag_files/string_id_globals.h"

#include "rex_macros.h"

#pragma endregion

#pragma region game_stubs

PPC_STUB(rex_saved_film_directory_delete_contents)
PPC_STUB(rex_autosave_queue_save_current_game_variant_to_queue)

#pragma endregion

#pragma region QoL

void post_setup_callback(void)
{
	REX_DATA_REFERENCE(0x826C7920, c_screen_postprocess::s_settings, x_settings_internal);
	x_settings_internal.m_postprocess = false;

	REX_DATA_REFERENCE(0x82881954, bool, disable_main_loop_throttle);
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
PPC_HOOK(rex_XUserGetName, XUserGetName);

PPC_HOOK(rex_main_game_unload_and_prepare_for_next_game, main_game_unload_and_prepare_for_next_game);

#pragma endregion

#pragma region midasm_hooks

void midasm_hook__chud_text_widget_compute_geometry(PPCRegister& r5, PPCRegister& r11)
{
	switch (r11.u32)
	{
	case 0:
		r5.s64 = 6;
		break;

	case 1:
		r5.s64 = 5;
		break;

	case 2:
		r5.s64 = 0;
		break;
		
	case 3:
		r5.s64 = 1;
		break;

	case 4:
		r5.s64 = 2;
		break;

	case 5:
		r5.s64 = 3;
		break;

	case 6:
		r5.s64 = 4;
		break;

	case 7:
		r5.s64 = 7;
		break;

	case 8:
		r5.s64 = 8;
		break;

	case 9:
		r5.s64 = 9;
		break;

	case 10:
		r5.s64 = 10;
		break;

	default:
		break;
	}
}

void midasm_hook__c_havok_component_build_physics_model_component(PPCRegister& r11)
{
	switch (r11.u32)
	{
	case 0:
		r11.s64 = 2;
		break;

	case 1:
		r11.s64 = 3;
		break;

	case 2:
		r11.s64 = 4;
		break;

	case 3:
		r11.s64 = 5;
		break;

	case 4:
		r11.s64 = 6;
		break;

	case 5:
		r11.s64 = 7;
		break;

	default:
		break;
	}
}

#pragma endregion

/*
	error G211B3073: use of undeclared label 'loc_8220CAA0'

	c_character_physics_component::set_mode

loc_8220CAA0:
	// addi r3,r31,8
	ctx.r3.s64 = ctx.r31.s64 + 8;
	// lwz r4,4(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x82227438
	ctx.lr = 0x8220CAAC;
	rex_c_character_physics_mode_sentinel_datum_dispose(ctx, base);
loc_8220CAAC:
*/
