#ifndef __REAL_MATH_H__
#define __REAL_MATH_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

constexpr float k_degrees_to_radians = 0.0174533f;
constexpr float k_radians_to_degrees = 57.2957802f;
constexpr float k_real_precision = __FLT_EPSILON__;
constexpr float k_pi = 3.1415927f;
constexpr float k_2pi = k_pi * 2.0f;
constexpr float k_3pi = k_pi * 3.0f;
constexpr float k_half_pi = k_pi / 2.0f;
constexpr float k_quarter_pi = k_pi / 4.0f;
constexpr float k_one_over_root2 = 0.7071068f;
constexpr float k_cosine30 = 0.8660254f;
constexpr float k_sine30 = 0.5f;
constexpr float k_cosine45 = 0.7071068f;
constexpr float k_sine45 = 0.7071068f;
constexpr float k_cosine60 = 0.5f;
constexpr float k_sine60 = 0.8660254f;
constexpr float k_full_circle = 6.2831855f;
constexpr float k_half_circle = 3.1415927f;
constexpr float k_quarter_circle = 1.5707964f;
constexpr float k_eighth_circle = 0.7853982f;
constexpr float k_real_epsilon = 0.0001f;
constexpr float k_valid_real_epsilon = 0.001f;
constexpr float k_double_epsilon = 0.000001f;
constexpr float k_root_5 = 2.236068f;
constexpr float k_golden_ratio = 1.618034f;
constexpr float k_one_over_golden_ratio = 0.618034f;
constexpr float k_real_min = -__FLT_MAX__;
constexpr float k_real_max = __FLT_MAX__;

constexpr float REAL_MIN = -__FLT_MAX__;
constexpr float REAL_MAX = __FLT_MAX__;

/* ---------- definitions */

union byte_rectangle3d;
union long_point3d;

using real = float;
using half = unsigned short;

union real_bounds2d
{
	real n[2];
	struct
	{
		real min;
		real max;
	};
};
static_assert(sizeof(real_bounds2d) == 0x8);

union real_euler_angles2d
{
	real n[2];
	struct
	{
		real yaw;
		real pitch;
	};
};
static_assert(sizeof(real_euler_angles2d) == 0x8);

union real_euler_angles3d
{
	real n[3];
	struct
	{
		real yaw;
		real pitch;
		real roll;
	};
};
static_assert(sizeof(real_euler_angles3d) == 0xC);

union real_point2d
{
	real n[2];
	struct
	{
		real x;
		real y;
	};
	struct
	{
		real u;
		real v;
	};
};
static_assert(sizeof(real_point2d) == 0x8);

union real_point3d
{
	real n[3];
	struct
	{
		real x;
		real y;
		real z;
	};
	struct
	{
		real u;
		real v;
		real w;
	};
};
static_assert(sizeof(real_point3d) == 0xC);

union double_point3d
{
	double n[3];
	struct
	{
		double x;
		double y;
		double z;
	};
	struct
	{
		double u;
		double v;
		double w;
	};
};
static_assert(sizeof(double_point3d) == 0x18);

union real_vector2d
{
	real n[2];
	struct
	{
		real i;
		real j;
	};
};
static_assert(sizeof(real_vector2d) == 0x8);

union real_vector3d
{
	real n[3];
	struct
	{
		real i;
		real j;
		real k;
	};
};
static_assert(sizeof(real_vector3d) == 0xC);

union double_vector3d
{
	double n[3];
	struct
	{
		double i;
		double j;
		double k;
	};
};
static_assert(sizeof(double_vector3d) == 0x18);

union real_vector4d
{
	real n[4];
	struct
	{
		real i;
		real j;
		real k;
		real l;
	};
};
static_assert(sizeof(real_vector4d) == 0x10);

struct real_quaternion
{
	real_vector3d v;
	real w;
};
static_assert(sizeof(real_quaternion) == 0x10);

struct real_orientation
{
	real_quaternion rotation;
	real_point3d translation;
	real scale;
};
static_assert(sizeof(real_orientation) == 0x20);

struct real_matrix3x3
{
	union
	{
		real n[3][3];
		real_vector3d basis[3];
		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
		};
	};
};
static_assert(sizeof(real_matrix3x3) == 0x24);

struct real_matrix4x3
{
	real scale;
	union
	{
		real n[4][3];
		real_matrix3x3 matrix3x3;
		struct
		{
			real_vector3d basis[3];
			real_point3d origin;
		};
		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
			real_point3d position;
		};
	};
};
static_assert(sizeof(real_matrix4x3) == 0x34);

struct real_plane2d
{
	real_vector2d n;
	real d;
};
static_assert(sizeof(real_plane2d) == 0xC);

struct real_plane3d
{
	real_vector3d n;
	real d;
};
static_assert(sizeof(real_plane3d) == 0x10);

struct double_plane3d
{
	double_vector3d n;
	double d;
};
static_assert(sizeof(double_plane3d) == 0x20);

union real_rectangle2d
{
	real n[4];
	real m[2][2];
	struct
	{
		real x0;
		real x1;
		real y0;
		real y1;
	};
};
static_assert(sizeof(real_rectangle2d) == 0x10);

union real_rectangle3d
{
	real n[6];
	real m[3][2];
	struct
	{
		real x0;
		real x1;
		real y0;
		real y1;
		real z0;
		real z1;
	};
};
static_assert(sizeof(real_rectangle3d) == 0x18);

union half_vector3d
{
	half n[3];
	struct
	{
		half i;
		half j;
		half k;
	};
};
static_assert(sizeof(half_vector3d) == 0x6);

/* ---------- prototypes */

/* ---------- globals */

extern real_rectangle3d const* global_null_rectangle3d;
extern real_vector3d const* global_forward3d;
extern real_vector3d const* global_zero_vector3d;
extern real_point3d const* global_origin3d;
extern real_vector3d const* global_up3d;
extern real_vector3d const* global_down3d;
extern real_matrix4x3 const* global_identity4x3;
extern real_orientation const* global_identity_orientation;
extern real_vector3d const* global_left3d;
extern real_quaternion const* global_identity_quaternion;
extern real_point2d const* global_origin2d;
extern real_point2d const* global_x_axis2d;
extern real_point2d const* global_y_axis2d;
extern real_point2d const* global_negative_x_axis2d;
extern real_point2d const* global_negative_y_axis2d;
extern real_vector2d const* global_zero_vector2d;
extern real_vector2d const* global_forward2d;
extern real_vector2d const* global_left2d;
extern real_vector2d const* global_backward2d;
extern real_vector2d const* global_right2d;
extern real_point3d const* global_x_axis3d;
extern real_point3d const* global_y_axis3d;
extern real_point3d const* global_z_axis3d;
extern real_point3d const* global_negative_x_axis3d;
extern real_point3d const* global_negative_y_axis3d;
extern real_point3d const* global_negative_z_axis3d;
extern real_vector3d const* global_backward3d;
extern real_vector3d const* global_right3d;
extern real_euler_angles2d const* global_zero_angles2d;
extern real_euler_angles3d const* global_zero_angles3d;
extern real_vector4d const* global_zero_vector4d;
extern real_matrix4x3 const* global_negative_identity4x3;
extern real_rectangle2d const* global_null_rectangle2d;
extern real_rectangle2d const* global_zero_rectangle2d;
extern real_rectangle3d const* global_zero_rectangle3d;

/* ---------- public code */

/* ---------- private code */

#endif // __REAL_MATH_H__
