/* ---------- headers */

#include "multithreading/threads.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(initialize_thread_management);
REX_PPC_EXTERN_IMPORT(initialize_synchronization_objects);

// hooks


/* ---------- public code */

void initialize_thread_management(void)
{
    REX_PPC_INVOKE(initialize_thread_management);
}

void initialize_synchronization_objects(void)
{
    REX_PPC_INVOKE(initialize_synchronization_objects);
}

/* ---------- private code */
