#ifndef __GAME_H__
#define __GAME_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

extern void __tls_set_g_game_globals_allocator(void* new_address);
extern void game_dispose_from_old_map(void);
extern bool game_in_progress(void);
extern void game_initialize(void);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __GAME_H__
