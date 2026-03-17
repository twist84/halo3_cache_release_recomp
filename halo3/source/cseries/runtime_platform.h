#ifndef __RUNTIME_PLATFORM_H__
#define __RUNTIME_PLATFORM_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_runtime_platform_type
{
    _runtime_platform_xbox = 0,
    _runtime_platform_not_xbox,

    NUMBER_OF_RUNTIME_PLATFORMS,
};

/* ---------- definitions */

/* ---------- prototypes */

extern e_runtime_platform_type get_runtime_platform_type();
extern bool runtime_platform_needs_byteswapping();
extern bool runtime_platform_on_target_platform();

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __RUNTIME_PLATFORM_H__
