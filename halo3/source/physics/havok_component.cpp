/* ---------- headers */

#include "physics/havok_component.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

// hooks

// midasm_hooks

void midasm_hook__c_havok_component_build_physics_model_component(PPCRegister& r11)
{
	// inline call to `c_havok_component::rigid_body_motion_type_to_havok_motion_type`
	r11.s64 = c_havok_component::rigid_body_motion_type_to_havok_motion_type(r11.u32);
}

/* ---------- public code */

long c_havok_component::rigid_body_motion_type_to_havok_motion_type(long rigid_body_motion_type)
{
	long havok_motion_type;
	switch (rigid_body_motion_type)
	{
	case 0: // _rigid_body_motion_type_sphere
		havok_motion_type = 2; // MOTION_SPHERE_INERTIA
		break;

	case 1: // _rigid_body_motion_type_stabilized_sphere
		havok_motion_type = 3; // MOTION_STABILIZED_SPHERE_INERTIA
		break;

	case 2: // _rigid_body_motion_type_box
		havok_motion_type = 4; // MOTION_BOX_INERTIA
		break;

	case 3: // _rigid_body_motion_type_stabilized_box
		havok_motion_type = 5; // MOTION_STABILIZED_BOX_INERTIA
		break;

	case 4: // _rigid_body_motion_type_keyframed
		havok_motion_type = 6; // MOTION_KEYFRAMED
		break;

	case 5: // _rigid_body_motion_fixed
		havok_motion_type = 7; // MOTION_FIXED
		break;

	default:
		throw "this case should be unreachable";
		havok_motion_type = 1; // MOTION_DYNAMIC
		break;
	}
	return havok_motion_type;
}

/* ---------- private code */
