#ifndef __RUNTIME_STATE_H__
#define __RUNTIME_STATE_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

struct s_runtime_state_globals;

/* ---------- prototypes */

extern void runtime_state_shell_initialize();
extern void runtime_state_shell_dispose();
extern void runtime_state_initialize();
extern void runtime_state_dispose();
extern void runtime_state_initialize_for_new_map();
extern void runtime_state_dispose_from_old_map();
extern unsigned long runtime_state_get_buffer_address(long* buffer_size); // void const*

// runtime state windows

extern unsigned long runtime_state_allocate_buffer(long size); // void*
extern void runtime_state_free_buffer(unsigned long buffer); // void*

/* ---------- globals */

//extern s_runtime_state_globals g_runtime_state_globals;
extern s_runtime_state_globals& g_runtime_state_globals;

/* ---------- public code */

/* ---------- private code */

#endif // __RUNTIME_STATE_H__
