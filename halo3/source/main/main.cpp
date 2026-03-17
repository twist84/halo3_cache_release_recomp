/* ---------- headers */

#include "main/main.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct _main_globals
{
	bool startup_sequence_performed;
	bool halted_with_errors;
	bool run_xdemos;
	bool exit_game;
	bool save_core;
	bool load_core;
	char core_file_name[64];
	bool reset_map;
	bool reset_map_random_seed;
	bool revert_map;
	bool save_map;
	bool save_map_and_exit;
	bool skip_cinematic;
	bool prepare_to_switch_zone_set;
	volatile bool switch_zone_set;
	bool reverted_by_scripting;
	bool keep_playing_cinematic_outros_on_revert;
	bool ui_saving_files;
	bool byte_828D7B19;
	bool byte_828D7B1A;
	bool cheat_drop_tag;
	rex::be<long> switch_zone_set_index;
	rex::be<long> dword_828D7B20;
	rex::be<long> deactivating_designer_zone_mask;
	rex::be<long> cheat_drop_tag_index;
};
static_assert(sizeof(_main_globals) == 0x64);

/* ---------- prototypes */

/* ---------- globals */

REX_DATA_REFERENCE_DECLARE(0x82881954, bool, disable_main_loop_throttle);

/* ---------- private variables */

static REX_DATA_REFERENCE_DECLARE(0x828D7AC8, _main_globals, main_globals);

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(_internal_halt_render_thread_and_lock_resources);
REX_PPC_EXTERN_IMPORT(unlock_resources_and_resume_render_thread);
REX_PPC_EXTERN_IMPORT(main_loop);

// hooks

/* ---------- public code */

extern unlock_token _internal_halt_render_thread_and_lock_resources(char const* file, long line)
{
	return REX_PPC_INVOKE(_internal_halt_render_thread_and_lock_resources, file, line);
}

extern void unlock_resources_and_resume_render_thread(unlock_token token)
{
	REX_PPC_INVOKE(unlock_resources_and_resume_render_thread, token);
}

void main_loop(void)
{
	REX_PPC_INVOKE(main_loop);

#if 0
	main_loop_enter();
	restricted_region_unlock_primary(k_global_render_data_region);
	restricted_region_unlock_primary(k_game_state_render_region);

	unsigned long previous_loop_time = system_milliseconds();
	unlock_token render_thread_lock_token = 0;

	while (!main_globals.exit_game)
	{
		unsigned long current_loop_time = system_milliseconds();
		unsigned long loop_time_advance = current_loop_time - previous_loop_time;
		previous_loop_time = current_loop_time;

		if (!disable_main_loop_throttle && loop_time_advance < 7)
		{
			unsigned long sleepy_time = 7 - loop_time_advance;
			sleep(sleepy_time);
		}
		else
		{
			main_set_single_thread_request_flag(_single_thread_for_user_request, !g_render_thread_user_setting);

			bool single_threaded_mode_active;
			if (render_thread_enabled())
			{
				single_threaded_mode_active = false;
				main_thread_process_pending_messages();
				main_loop_body_multi_threaded();
			}
			else
			{
				single_threaded_mode_active = true;
				main_thread_process_pending_messages();
				main_loop_body_single_threaded();
			}

			bool single_threaded_desired = g_single_thread_request_flags.peek() != 0;
			if (single_threaded_desired != single_threaded_mode_active)
			{
				if (single_threaded_mode_active)
				{
					unlock_resources_and_resume_render_thread(render_thread_lock_token);
				}
				else
				{
					render_thread_lock_token = _internal_halt_render_thread_and_lock_resources();
				}
			}
		}
	}

	enable_render_thread(false);
	restricted_region_lock_primary(k_game_state_render_region);
	restricted_region_lock_primary(k_global_render_data_region);
	main_loop_exit();
#endif
}

/* ---------- private code */
