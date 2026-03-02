#ifndef __CSERIES_MACROS_H__
#define __CSERIES_MACROS_H__
#pragma once

/* ---------- headers */

#ifdef _DEBUG
#include <limits>
#include <type_traits>
#endif

/* ---------- constants */

#ifdef INT64_BITS
#undef INT64_BITS
#endif
#ifdef INT64_BITS_BITS
#undef INT64_BITS_BITS
#endif
#ifdef UNSIGNED_LONG_MAX
#undef UNSIGNED_LONG_MAX
#endif
#ifdef LONG_MAX
#undef LONG_MAX
#endif
#ifdef LONG_MIN
#undef LONG_MIN
#endif
#ifdef LONG_BITS
#undef LONG_BITS
#endif
#ifdef LONG_BITS_BITS
#undef LONG_BITS_BITS
#endif
#ifdef LONG_MASK
#undef LONG_MASK
#endif
#ifdef UNSIGNED_SHORT_MAX
#undef UNSIGNED_SHORT_MAX
#endif
#ifdef SHORT_BITS
#undef SHORT_BITS
#endif
#ifdef SHORT_BITS_BITS
#undef SHORT_BITS_BITS
#endif
#ifdef SHORT_MASK
#undef SHORT_MASK
#endif
#ifdef UNSIGNED_CHAR_MAX
#undef UNSIGNED_CHAR_MAX
#endif
#ifdef CHAR_MAX
#undef CHAR_MAX
#endif
#ifdef CHAR_MIN
#undef CHAR_MIN
#endif
#ifdef CHAR_BITS
#undef CHAR_BITS
#endif
#ifdef CHAR_BITS_BITS
#undef CHAR_BITS_BITS
#endif
#ifdef CHAR_MASK
#undef CHAR_MASK
#endif
#ifdef BYTE_BITS
#undef BYTE_BITS
#endif
#ifdef BYTE_BITS_BITS
#undef BYTE_BITS_BITS
#endif
#ifdef WCHAR_BITS
#undef WCHAR_BITS
#endif

enum
{
#ifndef INT64_BITS
    INT64_BITS = 64, // 0x0040
#endif // INT64_BITS 
#ifndef INT64_BITS_BITS
    INT64_BITS_BITS = 6, // 0x0006
#endif // INT64_BITS_BITS 
#ifndef UNSIGNED_LONG_MAX
    UNSIGNED_LONG_MAX = -1, // 0xFF
#endif // UNSIGNED_LONG_MAX 
#ifndef LONG_MAX
    LONG_MAX = 2147483647, // 0x7FFFFFFF
#endif // LONG_MAX 
#ifndef LONG_MIN
    LONG_MIN = -2147483648, // 0x80000000
#endif // LONG_MIN 
#ifndef LONG_BITS
    LONG_BITS = 32, // 0x0020
#endif // LONG_BITS 
#ifndef LONG_BITS_BITS
    LONG_BITS_BITS = 5, // 0x0005
#endif // LONG_BITS_BITS 
#ifndef LONG_MASK
    LONG_MASK = -1, // 0xFF
#endif // LONG_MASK 
#ifndef UNSIGNED_SHORT_MAX
    UNSIGNED_SHORT_MAX = 65535, // 0xFFFF
#endif // UNSIGNED_SHORT_MAX 
#ifndef SHORT_BITS
    SHORT_BITS = 16, // 0x0010
#endif // SHORT_BITS 
#ifndef SHORT_BITS_BITS
    SHORT_BITS_BITS = 4, // 0x0004
#endif // SHORT_BITS_BITS 
#ifndef SHORT_MASK
    SHORT_MASK = 65535, // 0xFFFF
#endif // SHORT_MASK 
#ifndef UNSIGNED_CHAR_MAX
    UNSIGNED_CHAR_MAX = 255, // 0x00FF
#endif // UNSIGNED_CHAR_MAX 
#ifndef CHAR_MAX
    CHAR_MAX = 127, // 0x007F
#endif // CHAR_MAX 
#ifndef CHAR_MIN
    CHAR_MIN = -128, // 0x80
#endif // CHAR_MIN 
#ifndef CHAR_BITS
    CHAR_BITS = 8, // 0x0008
#endif // CHAR_BITS 
#ifndef CHAR_BITS_BITS
    CHAR_BITS_BITS = 3, // 0x0003
#endif // CHAR_BITS_BITS 
#ifndef CHAR_MASK
    CHAR_MASK = 255, // 0x00FF
#endif // CHAR_MASK 
#ifndef BYTE_BITS
    BYTE_BITS = 8, // 0x0008
#endif // BYTE_BITS 
#ifndef BYTE_BITS_BITS
    BYTE_BITS_BITS = 3, // 0x0003
#endif // BYTE_BITS_BITS 
#ifndef WCHAR_BITS
    WCHAR_BITS = 16, // 0x0010
#endif // WCHAR_BITS 
};

#define ENUM_OPERATORS(TYPE) \
static TYPE& operator++(TYPE& value, int __formal) \
{ \
    value = static_cast<TYPE>(value + 1); \
    return value; \
} \
 \
static TYPE& operator--(TYPE& value, int __formal) \
{ \
    value = static_cast<TYPE>(value - 1); \
    return value; \
} \

#define GET_BITS(bitfield, shift, mask) (((bitfield) & (mask)) >> (shift))
#define SET_BITS(value, shift, mask) (((value) << (shift)) & (mask))
#define ROUND_UP_TO_MULTIPLE(value, multiple) (((value) + (multiple) - 1) / (multiple) * (multiple))

#define BITS_TO_BYTES(bits) (((bits) + 7) / 8)
#define BYTES_TO_BITS(bytes) ((bytes) * 8)

#define FLAG(shift) (1u << (shift))
#define FLAG_RANGE(min_bit, max_bit) (RANGE(max_bit) & ~RANGE(min_bit))

#define SET_BIT(flags, bit, enable) do { if ((enable)) { (flags) |= FLAG((bit)); } else { (flags) &= ~FLAG((bit)); } } while(false)

#define TEST_MASK(flags, mask) (((flags) & (mask)) != 0)
#define TEST_BIT(flags, shift) (((flags) & static_cast<decltype(flags)>(FLAG(shift))) != 0)
#define OFFSETOF(t, d) __builtin_offsetof(t, d)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define SIZEOF_BITS(expression) (sizeof(expression) * 8)
#define IN_RANGE(value, begin, end) ((value) > (begin) && (value) < (end))
#define IN_RANGE_INCLUSIVE(value, begin, end) ((value) >= (begin) && (value) <= (end))
#define VALID_BITS(flags, max_bits) (((flags) & ~RANGE((max_bits))) == 0)
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count))

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(x, low) ((x) < (low) ? (x) : (low))
#define MAX(x, high) ((x) > (high) ? (x) : (high))
#define CLAMP(x, low, high) ((x) < (low) ? (low) : (x) > (high) ? (high) : (x))
#define CLAMP_LOWER(x, low, high) ((x) >= (high) - (low) ? (x) - (high) : (low))
#define CLAMP_UPPER(x, low, high) ((x) <= (high) - (low) ? (x) + (low) : (high))

#define CONCATX(x, y) x##y
#define CONCAT(x, y)  CONCATX(x, y)

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __CSERIES_MACROS_H__
