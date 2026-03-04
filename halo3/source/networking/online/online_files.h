#ifndef __ONLINE_FILES_H__
#define __ONLINE_FILES_H__
#pragma once

/* ---------- headers */

#include "cseries/platform.h"

/* ---------- constants */

FORWARD_DECLARE_ENUM(e_map_memory_configuration);

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

extern bool& g_online_is_connected_to_live;

/* ---------- public code */

extern void online_files_memory_dispose(void);
extern void online_files_memory_initialize(e_map_memory_configuration memory_configuration);

/* ---------- private code */

#endif // __ONLINE_FILES_H__
