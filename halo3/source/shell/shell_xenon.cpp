/* ---------- headers */

#include "shell/shell_xenon.h"

#include "cache/physical_memory_map.h"
#include "main/main.h"
#include "shell/shell.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(utility_drive_initialize);

// hooks

/* ---------- public code */

// $IMPLEMENT utility functions

_inline bool shell_has_utility_drive(void)
{
    return true;
}

_inline void shell_flush_utility_drive(void)
{
}

_inline void shell_flush_utility_drive_async(void)
{
}

_inline void shell_utility_drive_flush_path(s_file_reference const* file)
{
    (void)(file);
}

_inline void shell_utility_drive_flush_path(char const* file)
{
    (void)(file);
}

_inline bool shell_file_is_utility_drive_path(s_file_reference const* file)
{
    (void)(file);
    return false;
}

_inline bool shell_file_is_utility_drive_path(char const* file_name)
{
    (void)(file_name);
    return false;
}

_inline bool shell_utility_drive_was_cleared_on_startup(void)
{
    return false;
}

#if 0 // $IMPLEMENT
int main(int argc, const char** argv, const char** envp)
{
    physical_memory_initialize();
    file_system_cache_initialize();

    physical_memory_stage_push(_memory_stage_game_initialize);

    if (shell_initialize(false))
    {
        main_loop();
        shell_dispose();
    }

    physical_memory_stage_pop(_memory_stage_game_initialize);
    physical_memory_dispose();

    system_exit(0);
    return 0;
}
#endif

bool shell_platform_initialize(void)
{
    utility_drive_initialize();

    return true;
}

void shell_platform_dispose(void)
{
}

void utility_drive_initialize(void)
{
    REX_PPC_INVOKE(utility_drive_initialize);

#if 0
    DWORD pdwLaunchDataSize = 0;
    XGetLaunchDataSize(&pdwLaunchDataSize);
#endif
}

/* ---------- private code */
