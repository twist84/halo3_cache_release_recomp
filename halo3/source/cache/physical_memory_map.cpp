/* ---------- headers */

#include "cache/physical_memory_map.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

REX_PPC_EXTERN_IMPORT(physical_memory_stage_pop);

/* ---------- globals */

/* ---------- private variables */

/* ---------- public code */

//void physical_memory_stage_push(memory_stage stage)
//{
//}

void physical_memory_stage_pop(memory_stage stage) // 0x1801DB9D0
{
    REX_PPC_INVOKE(void, physical_memory_stage_pop, stage);
}

/* ---------- private code */
