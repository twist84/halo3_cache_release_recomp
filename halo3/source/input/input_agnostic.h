#ifndef __INPUT_AGNOSTIC_H__
#define __INPUT_AGNOSTIC_H__
#pragma once

/* ---------- headers */

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

extern inline void update_button(unsigned char* frames, unsigned short* msec, bool down, unsigned long elapsed_msec);
extern inline void update_button(unsigned char* frames, rex::be<unsigned short>* msec, bool down, unsigned long elapsed_msec);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __INPUT_AGNOSTIC_H__
