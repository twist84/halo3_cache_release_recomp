#ifndef __REX_MACROS_H__
#define __REX_MACROS_H__
#pragma once

/* ---------- headers */

#include "generated/halo3_cache_release_config.h"
#include "generated/halo3_cache_release_init.h"

#include <rex/rex_app.h>
#include <rex/ppc/function.h>	

/* ---------- constants */

#define REX_PPC_EXTERN_IMPORT(function) \
	PPC_EXTERN_IMPORT(__imp__rex_##function)

#define REX_PPC_INVOKE(return_type, function, ...) \
	rex::GuestToHostFunction<return_type>(__imp__rex_##function, __VA_ARGS__)

#define REX_DATA_REFERENCE(address, type, name) \
	type& name = *rex::Runtime::instance()->memory()->TranslateVirtual<type*>(address)

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __REX_MACROS_H__
