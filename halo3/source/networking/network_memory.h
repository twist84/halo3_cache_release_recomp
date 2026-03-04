#ifndef __NETWORK_MEMORY_H__
#define __NETWORK_MEMORY_H__
#pragma once

/* ---------- headers */

#include "cseries/platform.h"

/* ---------- constants */

FORWARD_DECLARE_ENUM(e_map_memory_configuration);

/* ---------- definitions */

/* ---------- prototypes */

extern void network_memory_shared_initialize(e_map_memory_configuration memory_configuration);
extern void network_memory_shared_dispose();

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __NETWORK_MEMORY_H__
