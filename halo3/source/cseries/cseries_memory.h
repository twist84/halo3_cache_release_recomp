#ifndef __CSERIES_MEMORY_H__
#define __CSERIES_MEMORY_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

extern "C"
{
    void* csmemcpy(void* destination, const void* source, size_t size);
    void* csmemmove(void* destination, const void* source, size_t size);
    void* csmemset(void* buffer, int c, size_t size);
    int csmemcmp(const void* p1, const void* p2, size_t size);
}

/* ---------- globals */

/* ---------- public code */

#endif // __CSERIES_MEMORY_H__