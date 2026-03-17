#ifndef __SHELL_H__
#define __SHELL_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_shell_application_type
{
    _shell_application_tool = 0,
    _shell_application_editor,
    _shell_application_game,
    k_shell_application_count,
};

enum e_shell_tool_type
{
    _shell_tool_invalid = 0,
    _shell_tool_interactive,
    _shell_tool_command_line,
    k_shell_tool_count,
};

enum e_shell_user_query
{
    _shell_query_yes = 0,
    _shell_query_yes_no,
    _shell_query_yes_no_cancel,
    _shell_query_message,

    k_number_of_shell_user_queries = 4,

    k_user_input_at_console_query_types = 7,
};

/* ---------- definitions */

/* ---------- prototypes */

extern void shell_halt_on_pure_virtual_call();
extern bool shell_initialize(bool null_device);
extern void shell_dispose();

// shell xenon

extern void shell_platform_dispose(void);
extern bool shell_platform_initialize(void);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __SHELL_H__
