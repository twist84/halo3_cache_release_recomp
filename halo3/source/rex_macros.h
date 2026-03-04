#ifndef __REX_MACROS_H__
#define __REX_MACROS_H__
#pragma once

/* ---------- headers */

#include "generated/halo3_cache_release_config.h"
#include "generated/halo3_cache_release_init.h"

#include <rex/rex_app.h>
#include <rex/ppc/function.h>	

#include <type_traits>

/* ---------- constants */

#define REX_PPC_EXTERN_IMPORT(function) \
	PPC_EXTERN_IMPORT(__imp__rex_##function)

#define REX_PPC_INVOKE(function, ...) \
	rex::GuestToHostFunction<function_return_t<decltype(function)>>(__imp__rex_##function, __VA_ARGS__)

#define REX_PPC_HOOK(function) \
	PPC_HOOK(rex_##function, function)

#define REX_DATA_REFERENCE_DECLARE(address, type, name) \
	type& name = *reinterpret_cast<type*>(0x100000000 + address)

#define REX_DATA_REFERENCE_DECLARE_ARRAY(address, type, name, count) \
	type(&name)[count] = *reinterpret_cast<type(*)[count]>(0x100000000 + address)

/* ---------- definitions */

template <typename t_type>
struct function_t;

template <typename t_return_type, typename... t_args>
struct function_t<t_return_type(t_args...)>
{
	using return_type = t_return_type;
};

template <typename t_return_type, typename... t_args>
struct function_t<t_return_type(*)(t_args...)>
{
	using return_type = t_return_type;
};

template <typename t_return_type, typename t_class, typename... t_args>
struct function_t<t_return_type(t_class::*)(t_args...)>
{
	using return_type = t_return_type;
};

template <typename t_return_type, typename t_class, typename... t_args>
struct function_t<t_return_type(t_class::*)(t_args...) const>
{
	using return_type = t_return_type;
};

template <typename t_function>
using function_return_t =
typename function_t<std::remove_cvref_t<t_function>>::return_type;

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __REX_MACROS_H__
