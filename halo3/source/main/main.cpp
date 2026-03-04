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

/* ---------- private code */
