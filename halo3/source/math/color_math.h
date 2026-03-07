#ifndef __COLOR_MATH_H__
#define __COLOR_MATH_H__
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

enum
{
    _rgb_colors_interpolate_in_hsv_space_bit = 0,
    _rgb_colors_interpolate_along_farthest_hue_path_bit,
    NUMBER_OF_RGB_COLORS_INTERPOLATE_FLAGS,
};

enum e_global_color
{
    _global_color_white,
    _global_color_grey,
    _global_color_black,
    _global_color_red,
    _global_color_green,
    _global_color_blue,
    _global_color_yellow,
    _global_color_cyan,
    _global_color_magenta,
    _global_color_pink,
    _global_color_lightblue,
    _global_color_orange,
    _global_color_purple,
    _global_color_aqua,
    _global_color_darkgreen,
    _global_color_salmon,
    _global_color_violet,
    k_number_of_global_colors,
};

/* ---------- definitions */

typedef unsigned long pixel32;
typedef unsigned long logluv32;

union byte_abgr_color
{
	unsigned char n[4];
	unsigned long integer;
	struct
	{
		unsigned char alpha;
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	};
};
static_assert(sizeof(byte_abgr_color) == 0x4);

union real_abgr_color
{
	real n[4];
	struct
	{
		real alpha;
		real blue;
		real green;
		real red;
	};
};
static_assert(sizeof(real_abgr_color) == 0x10);

union half_abgr_color
{
	half n[4];
	struct
	{
		half alpha;
		half blue;
		half green;
		half red;
	};
};
static_assert(sizeof(half_abgr_color) == 0x8);

union byte_rgb_color
{
	unsigned char n[3];
	struct
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
};
static_assert(sizeof(byte_rgb_color) == 0x3);

union rgb_color
{
	unsigned short n[3];
	struct
	{
		unsigned short red;
		unsigned short green;
		unsigned short blue;
	};
};
static_assert(sizeof(rgb_color) == 0x6);

union real_rgb_color
{
	real n[3];
	struct
	{
		real red;
		real green;
		real blue;
	};
};
static_assert(sizeof(real_rgb_color) == 0xC);

union half_rgb_color
{
	half n[3];
	struct
	{
		half red;
		half green;
		half blue;
	};
};
static_assert(sizeof(half_rgb_color) == 0x6);

union byte_argb_color
{
	unsigned char n[4];
	unsigned long integer;
	struct
	{
		unsigned char alpha;
		union
		{
			byte_rgb_color rgb;
			struct
			{
				unsigned char red;
				unsigned char green;
				unsigned char blue;
			};
		};
	};
};
static_assert(sizeof(byte_argb_color) == 0x4);

union byte_rgba_color
{
	unsigned char n[4];
	unsigned long integer;
	struct
	{
		union
		{
			byte_rgb_color rgb;
			struct
			{
				unsigned char red;
				unsigned char green;
				unsigned char blue;
			};
		};
		unsigned char alpha;
	};
};
static_assert(sizeof(byte_rgba_color) == 0x4);

union argb_color
{
	unsigned short n[4];
	struct
	{
		unsigned short alpha;
		union
		{
			rgb_color rgb;
			struct
			{
				unsigned short red;
				unsigned short green;
				unsigned short blue;
			};
		};
	};
};
static_assert(sizeof(argb_color) == 0x8);

union real_argb_color
{
	real n[4];
	struct
	{
		real alpha;
		union
		{
			real_rgb_color rgb;
			struct
			{
				real red;
				real green;
				real blue;
			};
		};
	};
};
static_assert(sizeof(real_argb_color) == 0x10);

union half_argb_color
{
	half n[4];
	struct
	{
		half alpha;
		half red;
		half green;
		half blue;
	};
};
static_assert(sizeof(half_argb_color) == 0x8);

union hsv_color
{
	half n[3];
	struct
	{
		half hue;
		half saturation;
		half value;
	};
};
static_assert(sizeof(hsv_color) == 0x6);

union real_hsv_color
{
	real n[3];
	struct
	{
		real hue;
		real saturation;
		real value;
	};
};
static_assert(sizeof(real_hsv_color) == 0xC);

union ahsv_color
{
	half n[4];
	struct
	{
		half alpha;
		union
		{
			hsv_color hsv;
			struct
			{
				half hue;
				half saturation;
				half value;
			};
		};
	};
};
static_assert(sizeof(ahsv_color) == 0x8);

union real_ahsv_color
{
	real n[4];
	struct
	{
		real alpha;
		union
		{
			real_hsv_color hsv;
			struct
			{
				real hue;
				real saturation;
				real value;
			};
		};
	};
};
static_assert(sizeof(real_ahsv_color) == 0x10);

struct real_linear_rgb_color
{
	real red;
	real green;
	real blue;
};
static_assert(sizeof(real_linear_rgb_color) == 0xC);

struct real_linear_xyz_color
{
	real x;
	real y;
	real z;
};
static_assert(sizeof(real_linear_xyz_color) == 0xC);

struct real_rgb_radiance
{
	real red;
	real green;
	real blue;
};
static_assert(sizeof(real_rgb_radiance) == 0xC);

struct real_rgb_mini_lightprobe
{
	real red_terms[4];
	real green_terms[4];
	real blue_terms[4];
};
static_assert(sizeof(real_rgb_mini_lightprobe) == 0x30);

struct half_rgb_mini_lightprobe
{
	half red_terms[4];
	half green_terms[4];
	half blue_terms[4];
};
static_assert(sizeof(half_rgb_mini_lightprobe) == 0x18);

struct real_rgb_lightprobe
{
	real red_terms[9];
	real green_terms[9];
	real blue_terms[9];
};
static_assert(sizeof(real_rgb_lightprobe) == 0x6C);

struct real_ycbcr_color
{
	real y;
	real cb;
	real cr;
};
static_assert(sizeof(real_ycbcr_color) == 0xC);

struct real_ycbcr_lightprobe
{
	real_ycbcr_color terms[9];
};
static_assert(sizeof(real_ycbcr_lightprobe) == 0x6C);

struct half_linear_rgb_color
{
	half red;
	half green;
	half blue;
	half pad;
};
static_assert(sizeof(half_linear_rgb_color) == 0x8);

struct half_rgb_lightprobe
{
	half red_terms[9];
	half green_terms[9];
	half blue_terms[9];
	half pad;
};
static_assert(sizeof(half_rgb_lightprobe) == 0x38);

struct half_rgb_lightprobe_with_dominant_light
{
	half_vector3d dominant_light_direction;
	unsigned short pad;
	half_linear_rgb_color dominant_light_intensity;
	half_rgb_lightprobe quadratic_probe;
};
static_assert(sizeof(half_rgb_lightprobe_with_dominant_light) == 0x48);

struct real_xyz_radiance
{
	real x;
	real y;
	real z;
};
static_assert(sizeof(real_xyz_radiance) == 0xC);

struct s_faux_logluv_lightprobe
{
	unsigned long terms[9];
};
static_assert(sizeof(s_faux_logluv_lightprobe) == 0x24);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __COLOR_MATH_H__
