#pragma once
#ifndef __STATIC_ARRAY_H__
#define __STATIC_ARRAY_H__

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

template<typename t_type, long t_count>
using c_static_array = t_type[t_count];

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __STATIC_ARRAY_H__