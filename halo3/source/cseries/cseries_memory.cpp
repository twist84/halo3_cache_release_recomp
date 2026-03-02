/* ---------- headers */

#include "cseries\cseries_memory.h"

#include <cassert>
#include <memory>

/* ---------- constants */

const size_t k_maximum_memcmp_size = 0x20000000;
const size_t k_maximum_memcpy_size = 0x20000000;
const size_t k_maximum_memmove_size = 0x20000000;
const size_t k_maximum_memset_size = 0x20000000;

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void* csmemcpy(void* destination, const void* source, size_t size)
{
	assert(size==0 || (destination && source));
	assert(size>=0 && size<k_maximum_memcpy_size);
	//assert(offset_pointer(source, size)<=destination || offset_pointer(destination, size)<=source);

	return memcpy(destination, source, size);
};

void* csmemmove(void* destination, const void* source, size_t size)
{
	assert(size==0 || (destination && source));
	assert(size>=0 && size<=k_maximum_memmove_size);

	return memmove(destination, source, size);
};

void memmove_guarded(void* destination, const void* source, size_t size, const void* bounds_start, size_t bounds_size)
{
	assert(size>=0 && size<=k_maximum_memmove_size);
	assert(bounds_size>=0);
	if (size > 0)
	{
		unsigned long write_start = (unsigned long)(static_cast<char const*>(destination));
		unsigned long write_end = (unsigned long)(write_start + size - 1);
		unsigned long bounds_lower = (unsigned long)(static_cast<char const*>(bounds_start));
		unsigned long bounds_upper = (unsigned long)(bounds_lower + bounds_size - 1);

		assert(bounds_upper>=bounds_lower);
		assert(bounds_size>0);
		assert(write_start>=bounds_lower && write_start<=bounds_upper);
		assert(write_end>=bounds_lower && write_end<=bounds_upper);

		memmove(destination, source, size);
	}
};

void* csmemset(void* buffer, int c, size_t size)
{
	assert(size==0 || buffer);
	assert(size>=0 && size<=k_maximum_memset_size);

	return memset(buffer, c, size);
};

int csmemcmp(const void* p1, const void* p2, size_t size)
{
	assert(size==0 || (p1 && p2));
	assert(size>=0 && size<=k_maximum_memcmp_size);

	return memcmp(p1, p2, size);
};

/* ---------- private code */

/* ---------- reverse engineering */