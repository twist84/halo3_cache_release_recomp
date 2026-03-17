/* ---------- headers */

#include "shell/shell.h"

#include "cseries/cseries.h"
#include "cseries/runtime_state.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

void shell_halt_on_pure_virtual_call()
{
    throw "calling pure virtual function!";
}

bool shell_initialize(bool null_device)
{
    bool success;

    //_set_purecall_handler(&shell_halt_on_pure_virtual_call);

    cseries_initialize();
    runtime_state_shell_initialize();

    if (shell_platform_initialize())
    {
        //real_math_initialize();
        //async_initialize();
        //global_preferences_initialize();
        //font_initialize();
        //tag_files_open();
        //game_state_shell_initialize();
        //c_rasterizer::shell_initialize(false, null_device);
        //input_initialize();
        //sound_initialize();
        //minidump_compression_initialize();

        success = true;
    }
    else
    {
        success = false;
    }
    return success;
}

void shell_dispose()
{
    //game_state_shell_dispose();
    //minidump_compression_dispose();
    //sound_dispose();
    //tag_files_close();
    //font_dispose();
    //c_rasterizer::shell_dispose();
    shell_platform_dispose();
    runtime_state_shell_dispose();
    cseries_dispose();
}

/* ---------- private code */
