#pragma once
#ifndef __THREAD_LOCAL_STORAGE_H__
#define __THREAD_LOCAL_STORAGE_H__

/* ---------- headers */

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

constexpr unsigned long k_tls_game_globals_offset = 0x80;

/* ---------- prototypes */

PPC_EXTERN_IMPORT(__imp__get_thread_local_by_offset);

/* ---------- globals */

/* ---------- public code */

template<typename t_type>
t_type* get_thread_local_by_offset(unsigned long offset)
{
	t_type* result = rex::GuestToHostFunction<t_type*>(__imp__get_thread_local_by_offset, offset);
	return result;
}

#endif // __CSERIES_STRING_H__