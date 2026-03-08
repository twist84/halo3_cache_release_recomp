/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

extern "C"
{
	real const private_zero_vector[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	real_orientation private_identity_orientation =
	{
		.rotation =
		{
			.v =
			{
				.i = 0.0f,
				.j = 0.0f,
				.k = 0.0f
			},
			.w = 1.0f
		},
		.translation =
		{
			.x = 0.0f,
			.y = 0.0f,
			.z = 0.0f
		},
		.scale = 1.0f
	};
	real_matrix4x3 private_identity4x3 =
	{
		.scale = 1.0f,
		.n =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f }
		},
	};
	real_matrix4x3 private_negative_identity4x3 =
	{
		.scale = 1.0f,
		.n =
		{
			{ -1.0f,  0.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f,  0.0f }
		},
	};
	real_rectangle3d private_null_rectangle =
	{
		.x0 = k_real_max,
		.x1 = k_real_min,
		.y0 = k_real_max,
		.y1 = k_real_min,
		.z0 = k_real_max,
		.z1 = k_real_min,
	};
}

real_vector3d const* global_forward3d = &private_identity4x3.forward;
real_vector3d const* global_left3d = &private_identity4x3.left;
real_vector3d const* global_up3d = &private_identity4x3.up;
real_vector3d const* global_backward3d = &private_negative_identity4x3.forward;
real_vector3d const* global_right3d = &private_negative_identity4x3.left;
real_vector3d const* global_down3d = &private_negative_identity4x3.up;

real_euler_angles2d const* global_zero_angles2d = reinterpret_cast<real_euler_angles2d const*>(private_zero_vector);
real_euler_angles3d const* global_zero_angles3d = reinterpret_cast<real_euler_angles3d const*>(private_zero_vector);
real_vector4d const* global_zero_vector4d = reinterpret_cast<real_vector4d const*>(private_zero_vector);

real_quaternion const* global_identity_quaternion = &private_identity_orientation.rotation;
real_matrix4x3 const* global_identity4x3 = &private_identity4x3;
real_matrix4x3 const* global_negative_identity4x3 = &private_negative_identity4x3;
real_orientation const* global_identity_orientation = &private_identity_orientation;

real_rectangle2d const* global_null_rectangle2d = reinterpret_cast<real_rectangle2d const*>(&private_null_rectangle);
real_rectangle3d const* global_null_rectangle3d = reinterpret_cast<real_rectangle3d const*>(&private_null_rectangle);
real_rectangle2d const* global_zero_rectangle2d = reinterpret_cast<real_rectangle2d const*>(private_zero_vector);
real_rectangle3d const* global_zero_rectangle3d = reinterpret_cast<real_rectangle3d const*>(private_zero_vector);

real_vector3d const* global_zero_vector3d = reinterpret_cast<real_vector3d const *>(private_zero_vector);

extern real_point2d const* global_origin2d;
real_vector2d const* global_forward2d = reinterpret_cast<real_vector2d const*>(&private_identity4x3.forward);
real_vector2d const* global_left2d = reinterpret_cast<real_vector2d const*>(&private_identity4x3.left);
real_vector2d const* global_backward2d = reinterpret_cast<real_vector2d const*>(&private_negative_identity4x3.forward);
real_vector2d const* global_right2d = reinterpret_cast<real_vector2d const*>(&private_negative_identity4x3.left);
real_point3d const* global_origin3d = reinterpret_cast<real_point3d const*>(&private_zero_vector);
real_point3d const* global_x_axis3d = reinterpret_cast<real_point3d const*>(&private_identity4x3.forward);
real_point3d const* global_y_axis3d = reinterpret_cast<real_point3d const*>(&private_identity4x3.left);
real_point3d const* global_z_axis3d = reinterpret_cast<real_point3d const*>(&private_identity4x3.up);
real_point3d const* global_negative_x_axis3d = reinterpret_cast<real_point3d const*>(&private_negative_identity4x3.forward);
real_point3d const* global_negative_y_axis3d = reinterpret_cast<real_point3d const*>(&private_negative_identity4x3.left);
real_point3d const* global_negative_z_axis3d = reinterpret_cast<real_point3d const*>(&private_negative_identity4x3.up);

/* ---------- public code */

/* ---------- private code */
