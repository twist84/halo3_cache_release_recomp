#ifndef __SHELL_XENON_H__
#define __SHELL_XENON_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

struct s_file_reference;

/* ---------- prototypes */

extern _inline bool shell_has_utility_drive();
extern _inline void shell_flush_utility_drive();
extern _inline void shell_flush_utility_drive_async();
extern _inline void shell_utility_drive_flush_path(s_file_reference const *file);
extern _inline void shell_utility_drive_flush_path(char const *file);
extern _inline bool shell_file_is_utility_drive_path(s_file_reference const *file);
extern _inline bool shell_file_is_utility_drive_path(char const *file_name);
extern _inline bool shell_utility_drive_was_cleared_on_startup();

extern void utility_drive_initialize(void);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __SHELL_XENON_H__
