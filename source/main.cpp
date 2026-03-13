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

REXCVAR_DEFINE_STRING(username, "User", "Rex/User", "override the users name");

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

// networking

bool transport_network_available()
{
	return true;
}
REX_PPC_HOOK(transport_network_available);

#include <WinSock.h>

// it isn't as easy as this but it should be

REX_PPC_HOOK(WSAStartup);
REX_PPC_HOOK(WSACleanup);
REX_PPC_HOOK(socket);
REX_PPC_HOOK(closesocket);
REX_PPC_HOOK(shutdown);
REX_PPC_HOOK(ioctlsocket);
REX_PPC_HOOK(setsockopt);
REX_PPC_HOOK(bind);
REX_PPC_HOOK(connect);
REX_PPC_HOOK(listen);
REX_PPC_HOOK(accept);
REX_PPC_HOOK(select);
REX_PPC_HOOK(recv);
REX_PPC_HOOK(recvfrom);
REX_PPC_HOOK(send);
REX_PPC_HOOK(sendto);
REX_PPC_HOOK(inet_addr);
REX_PPC_HOOK(WSAGetLastError);
REX_PPC_HOOK(__WSAFDIsSet);

#pragma endregion
