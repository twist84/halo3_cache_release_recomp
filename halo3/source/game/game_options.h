#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__
#pragma once

/* ---------- headers */

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

struct game_options
{
	rex::be<long> game_mode;
	char __data[0xD300 - 0x4];
};

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __GAME_OPTIONS_H__
