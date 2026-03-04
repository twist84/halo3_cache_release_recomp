/* ---------- headers */

#include "cseries/cseries_macros.h"
#include "cache/physical_memory_map.h"
#include "memory/static_array.h"

#include "rex_macros.h"

#include <cassert>

/* ---------- constants */

constexpr unsigned long k_physical_memory_page_mask = MASK(16);

/* ---------- definitions */

struct s_physical_memory_stage
{
	rex::be<unsigned long> low_address; // 0x0
	rex::be<unsigned long> high_address; // 0x4
	rex::guest_addr_t next_available_zero_allocation; // 0x8 // char*
	rex::be<unsigned long> allocation_count; // 0xC
};
static_assert(sizeof(s_physical_memory_stage) == 0x10);

struct s_physical_memory_globals
{
	rex::guest_addr_t resize_region_listener; // 0x0 // c_physical_memory_contiguous_region_listener*
	rex::be<memory_stage> current_stage; // 0x4
	rex::be<unsigned long> minimum_address; // 0x8
	rex::be<unsigned long> maximum_address; // 0xC
	rex::guest_addr_t no_mans_land; // 0x10 // char*
	c_static_array<s_physical_memory_stage, k_memory_stage_count> memory_stages; // 0x14
};
static_assert(sizeof(s_physical_memory_globals) == 0x84);

/* ---------- prototypes */

/* ---------- globals */

REX_DATA_REFERENCE_DECLARE(0x82845568, s_physical_memory_globals, physical_memory_globals);

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(physical_memory_stage_pop);

// hooks

REX_PPC_HOOK(physical_memory_stage_pop);

/* ---------- public code */

void physical_memory_stage_push(memory_stage stage)
{
	memory_stage new_stage = static_cast<memory_stage>(physical_memory_globals.current_stage + 1);
	assert(stage == new_stage);

	physical_memory_globals.memory_stages[new_stage] = physical_memory_globals.memory_stages[physical_memory_globals.current_stage];
	physical_memory_globals.current_stage = new_stage;

	assert(!TEST_MASK(physical_memory_globals.memory_stages[new_stage].low_address, k_physical_memory_page_mask));
	assert(!TEST_MASK(physical_memory_globals.memory_stages[new_stage].high_address, k_physical_memory_page_mask));
}

void physical_memory_stage_pop(memory_stage stage)
{
	REX_PPC_INVOKE(physical_memory_stage_pop, stage);
}

/* ---------- private code */
