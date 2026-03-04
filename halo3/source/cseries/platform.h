#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#pragma once

/* ---------- headers */

#include <vadefs.h>
#include <vcruntime.h>
#include <stdarg.h>

/* ---------- constants */

#define _WARNINGS_PUSH() __pragma(clang diagnostic push)
#define _WARNINGS_POP() __pragma(clang diagnostic pop)
#define WARNINGS_PUSH() _WARNINGS_PUSH() (void)(0)
#define WARNINGS_POP() _WARNINGS_POP() (void)(0)

#define _IGNORE_WARNING(warning) __pragma(clang diagnostic ignored warning)
#define IGNORE_WARNING(warning) _IGNORE_WARNING(warning); (void)(0)

#define _IGNORE_WARNING_PUSH(warning) _WARNINGS_PUSH(); __pragma(clang diagnostic ignored warning)
#define IGNORE_WARNING_PUSH(warning) _IGNORE_WARNING_PUSH(warning); (void)(0)
#define _IGNORE_WARNING_POP() _WARNINGS_POP();
#define IGNORE_WARNING_POP() _IGNORE_WARNING_POP(); (void)(0)

#define _IGNORE_ALL_WARNINGS_PUSH() _IGNORE_WARNING_PUSH("-Weverything")
#define IGNORE_ALL_WARNINGS_PUSH() _IGNORE_ALL_WARNINGS_PUSH(); (void)(0)
#define _IGNORE_ALL_WARNINGS_POP() _WARNINGS_POP()
#define IGNORE_ALL_WARNINGS_POP() _IGNORE_ALL_WARNINGS_POP(); (void)(0)

#define FORWARD_DECLARE_ENUM(_name) \
_IGNORE_ALL_WARNINGS_PUSH(); \
enum _name; \
_IGNORE_ALL_WARNINGS_POP();


#define ALWAYS_INLINE __attribute__((always_inline))
#define FORCE_INLINE __forceinline ALWAYS_INLINE

template <typename t_type>
constexpr bool is_enum = __is_enum(t_type);

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __PLATFORM_H__
