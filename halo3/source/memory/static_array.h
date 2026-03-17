#pragma once
#ifndef __STATIC_ARRAY_H__
#define __STATIC_ARRAY_H__

/* ---------- headers */

#include "cseries/cseries_macros.h"

#include "rex_macros.h"

/* ---------- constants */

#define BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT) (((BIT_COUNT) + (LONG_BITS - 1)) >> 5)
#define BIT_VECTOR_SIZE_IN_BYTES(BIT_COUNT) (4 * BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT))
#define BIT_VECTOR_TEST_FLAG(BIT_VECTOR, BIT) ((BIT_VECTOR[BIT >> 5] & (1 << (BIT & (LONG_BITS - 1)))) != 0)
#define BIT_VECTOR_OR_FLAG(BIT_VECTOR, BIT) (void)(BIT_VECTOR[BIT >> 5] |= (1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_AND_FLAG(BIT_VECTOR, BIT) (void)(BIT_VECTOR[BIT >> 5] &= ~(1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_SET_FLAG(BIT_VECTOR, BIT, VALUE) (void)(BIT_VECTOR[BIT >> 5] = VALUE ? (BIT_VECTOR[BIT >> 5] | (1 << (BIT & (LONG_BITS - 1)))) : (BIT_VECTOR[BIT >> 5] & ~(1 << (BIT & (LONG_BITS - 1)))))

/* ---------- definitions */

// $TODO add template functions

template<typename t_type, long k_count>
class c_static_array
{
public:
	typedef t_type* iterator;
	typedef t_type const* const_iterator;

	t_type& operator[](long index);
	t_type const& operator[](long) const;

//private:
	t_type m_elements[k_count]; // 0x0
};
static_assert(sizeof(c_static_array<long, 6>) == 24);

template<typename t_type, long k_count>
class c_static_stack
{
public:
	typedef t_type* iterator;
	typedef t_type const* const_iterator;

//private:
	rex::be<long> m_count; // 0x0
	t_type m_elements[k_count]; // 0x4
};
static_assert(sizeof(c_static_stack<long, 8>) == 36);

template<long k_count>
class c_static_flags_no_init
{
public:
	enum
	{
		k_size_in_dwords = BIT_VECTOR_SIZE_IN_LONGS(k_count)
	};

//private:
	rex::be<unsigned long> m_flags[k_size_in_dwords]; // 0x0
};
static_assert(sizeof(c_static_flags_no_init<256>) == 0x20);

template<long k_count>
class c_static_flags :
	public c_static_flags_no_init<k_count>
{
public:
};
static_assert(sizeof(c_static_flags<256>) == 0x20);

template<long k_count>
class c_static_string
{
public:
	typedef char t_char_buffer[k_count];

//private:
	t_char_buffer m_string; // 0x0
};
static_assert(sizeof(c_static_string<256>) == 256);

template<long k_count>
class c_static_wchar_string
{
public:
	typedef wchar_t t_wchar_buffer[k_count];

//private:
	t_wchar_buffer m_string; // 0x0
};
static_assert(sizeof(c_static_wchar_string<256>) == 512);

template<typename t_enum, typename t_storage, t_enum k_num_flags>
class c_flags_no_init
{
public:

//private:
	rex::be<t_storage> m_flags; // 0x0
};

template<typename t_enum, typename t_storage, t_enum k_num_flags>
class c_flags :
	public c_flags_no_init<t_enum, t_storage, k_num_flags>
{
public:
	typedef c_flags_no_init<t_enum, t_storage, k_num_flags> t_flags_no_init;
};

template<typename t_enum, typename t_storage, t_enum min, t_enum max>
class c_enum
{
public:

//private:
	rex::be<t_storage> m_enum_value; // 0x0
};

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#ifdef __STATIC_ARRAYS_TEMPLATES__

// $TODO add template implementations

template<typename t_type, long k_count>
_inline t_type& c_static_array<t_type, k_count>::operator[](long index)
{
	return m_elements[index];
}

template<typename t_type, long k_count>
_inline t_type const& c_static_array<t_type, k_count>::operator[](long index) const
{
	return m_elements[index];
}

#endif // __STATIC_ARRAYS_TEMPLATES__

#endif // __STATIC_ARRAY_H__