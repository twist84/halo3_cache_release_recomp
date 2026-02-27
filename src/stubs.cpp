#pragma region includes

#include "generated/halo3_cache_release_config.h"
#include "generated/halo3_cache_release_init.h"

#include <rex/rex_app.h>
#include <rex/ppc/function.h>

#pragma endregion

#pragma region xbox_stubs

PPC_STUB(_IoDismountVolume);
PPC_STUB(_XCustomUnregisterDynamicActions);
PPC_STUB(_XCustomSetDynamicActions);
PPC_STUB(_XCustomGetLastActionPressEx);
PPC_STUB(_XCustomGetCurrentGamercard);
PPC_STUB(_XamShowPlayerReviewUI);
PPC_STUB(_XamShowMessageComposeUI);
PPC_STUB(_XamShowGamerCardUIForXUID);
PPC_STUB(_XamShowFriendsUI);
PPC_STUB(_XamShowFriendRequestUI);
PPC_STUB(_XamShowCustomMessageComposeUI);
PPC_STUB(_NetDll_XNetUnregisterKey);
PPC_STUB(_NetDll_XNetUnregisterInAddr);
PPC_STUB(_NetDll_XNetServerToInAddr);
PPC_STUB(_NetDll_XNetRegisterKey);
PPC_STUB(_NetDll_XNetQosLookup);
PPC_STUB(_NetDll_XNetQosGetListenStats);
PPC_STUB(_NetDll_XNetGetConnectStatus);
PPC_STUB(_NetDll_XNetCreateKey);
PPC_STUB(_NetDll_XNetConnect);
PPC_STUB(_XNetLogonGetTitleID);
PPC_STUB(_XamVoiceSubmitPacket);
PPC_STUB(roundevenf);

#pragma endregion

#pragma region gane_stubs

PPC_STUB(rex_hs_runtime_update); // bad switch in `c_havok_component::build_physics_model_component`

#pragma endregion

#pragma region game_externs

PPC_EXTERN_IMPORT(__imp__rex_cache_files_copy_fonts);
PPC_EXTERN_IMPORT(__imp__rex_c_window_manager_handle_global_controller_event);

#pragma endregion

/*
rex_c_platform_source_datum_queue_sound

	default:
+		goto loc_822E18C8;
-		__builtin_trap(); // Switch case out of range
*/

bool cache_files_copy_fonts(void)
{
	bool result = rex::GuestToHostFunction<bool>(__imp__rex_cache_files_copy_fonts);
	return result; // skip over dirty disk
}
PPC_HOOK(rex_cache_files_copy_fonts, cache_files_copy_fonts);

enum e_controller_index
{
	_controller0 = 0x0,
	_controller1 = 0x1,
	_controller2 = 0x2,
	_controller3 = 0x3,
	k_number_of_controllers = 0x4,
	k_no_controller = 0xFFFFFFFF,
	k_any_controller = 0xFF,
};

enum e_event_type
{
	_event_type_none = 0x0,
	_event_type_tab_up = 0x1,
	_event_type_tab_left = 0x2,
	_event_type_tab_down = 0x3,
	_event_type_tab_right = 0x4,
	_event_type_alt_stick_up = 0x5,
	_event_type_alt_stick_left = 0x6,
	_event_type_alt_stick_down = 0x7,
	_event_type_alt_stick_right = 0x8,
	_event_type_alt_tab_up = 0x9,
	_event_type_alt_tab_left = 0xA,
	_event_type_alt_tab_down = 0xB,
	_event_type_alt_tab_right = 0xC,
	_event_type_button_press = 0xD,
	_event_type_automation = 0xE,
	k_number_of_event_types = 0xF,
};

const char* k_event_type_names[]
{
	"none",
	"tab_up",
	"tab_left",
	"tab_down",
	"tab_right",
	"alt_stick_up",
	"alt_stick_left",
	"alt_stick_down",
	"alt_stick_right",
	"alt_tab_up",
	"alt_tab_left",
	"alt_tab_down",
	"alt_tab_right",
	"button_press",
	"automation",
};
static_assert(k_number_of_event_types == (sizeof(k_event_type_names) / sizeof(*k_event_type_names)));

enum e_gamepad_button
{
	FIRST_GAMEPAD_ANALOG_BUTTON = 0x0,
	_gamepad_analog_button_left_trigger = 0x0,
	_gamepad_analog_button_right_trigger = 0x1,
	NUMBER_OF_GAMEPAD_ANALOG_BUTTONS = 0x2,
	FIRST_GAMEPAD_BINARY_BUTTON = 0x2,
	_gamepad_binary_button_dpad_up = 0x2,
	_gamepad_binary_button_dpad_down = 0x3,
	_gamepad_binary_button_dpad_left = 0x4,
	_gamepad_binary_button_dpad_right = 0x5,
	_gamepad_binary_button_start = 0x6,
	_gamepad_binary_button_back = 0x7,
	_gamepad_binary_button_left_thumb = 0x8,
	_gamepad_binary_button_right_thumb = 0x9,
	_gamepad_binary_button_a = 0xA,
	_gamepad_binary_button_b = 0xB,
	_gamepad_binary_button_x = 0xC,
	_gamepad_binary_button_y = 0xD,
	_gamepad_binary_button_left_bumper = 0xE,
	_gamepad_binary_button_right_bumper = 0xF,
	NUMBER_OF_GAMEPAD_BUTTONS = 0x10,
	NUMBER_OF_GAMEPAD_BINARY_BUTTONS = 0xE,
};

const char* k_gamepad_button_names[]
{
	"left_trigger",
	"right_trigger",
	"dpad_up",
	"dpad_down",
	"dpad_left",
	"dpad_right",
	"start",
	"back",
	"left_thumb",
	"right_thumb",
	"a",
	"b",
	"x",
	"y",
	"left_bumper",
	"right_bumper",
};
static_assert(NUMBER_OF_GAMEPAD_BUTTONS == (sizeof(k_gamepad_button_names) / sizeof(*k_gamepad_button_names)));

enum e_controller_component
{
	_controller_component_button_a = 0x0,
	_controller_component_button_b = 0x1,
	_controller_component_button_x = 0x2,
	_controller_component_button_y = 0x3,
	_controller_component_button_black = 0x4,
	_controller_component_button_white = 0x5,
	_controller_component_button_left_trigger = 0x6,
	_controller_component_button_right_trigger = 0x7,
	_controller_component_button_dpad_up = 0x8,
	_controller_component_button_dpad_down = 0x9,
	_controller_component_button_dpad_left = 0xA,
	_controller_component_button_dpad_right = 0xB,
	_controller_component_button_start = 0xC,
	_controller_component_button_back = 0xD,
	_controller_component_button_left_thumb = 0xE,
	_controller_component_button_right_thumb = 0xF,
	_controller_component_stick_left_thumb = 0x10,
	_controller_component_stick_right_thumb = 0x11,
	k_number_of_controller_components = 0x12,
	_controller_component_button_right_shoulder = 0x4,
	_controller_component_button_left_shoulder = 0x5,
};

const char* k_controller_component_names[]
{
	"button_a",
	"button_b",
	"button_x",
	"button_y",
	"button_right_shoulder",
	"button_left_shoulder",
	"button_left_trigger",
	"button_right_trigger",
	"button_dpad_up",
	"button_dpad_down",
	"button_dpad_left",
	"button_dpad_right",
	"button_start",
	"button_back",
	"button_left_thumb",
	"button_right_thumb",
	"stick_left_thumb",
	"stick_right_thumb",
};
static_assert(k_number_of_controller_components == (sizeof(k_controller_component_names) / sizeof(*k_controller_component_names)));

struct s_event_record
{
	rex::be<e_event_type>           type;
	rex::be<e_controller_index>     controller;
	rex::be<e_controller_component> component;
	rex::be<int16_t>                event_value;
};
static_assert(sizeof(s_event_record) == 0x10);

class c_gui_screen_widget;

struct s_data_array
{
	char              name[32];
	rex::be<int32_t>  maximum_count;
	rex::be<int32_t>  size;
	rex::be<uint8_t>  alignment_bits;
	bool              valid;
	rex::be<uint16_t> flags;
	rex::be<uint32_t> signature;
	rex::be<uint32_t> allocation; // c_allocation_base*
	rex::be<int32_t>  first_possibly_free_absolute_index;
	rex::be<int32_t>  count;
	rex::be<int32_t>  actual_count;
	rex::be<int16_t>  next_identifier;
	rex::be<int16_t>  isolated_next_identifier;
	rex::be<uint32_t> data; // void*
	rex::be<uint32_t> in_use_bit_vector; // void*
	rex::be<uint32_t> offset_to_data;
	rex::be<uint32_t> offset_to_bit_vector;
};
static_assert(sizeof(s_data_array) == 0x54);

class c_synchronized_long
{
public:
	volatile rex::be<long> m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

union rectangle2d
{
	rex::be<int16_t>     n[4];
	struct
	{
		rex::be<int16_t> y0;
		rex::be<int16_t> x0;
		rex::be<int16_t> y1;
		rex::be<int16_t> x1;
	};
};
static_assert(sizeof(rectangle2d) == 0x8);

class c_window_manager
{
public:
	void handle_global_controller_event(const s_event_record* event);

public:
	rex::be<uint32_t>   m_active_screens; // s_data_array
	rex::be<uint32_t>   m_channels[5][10]; // c_gui_screen_widget
	c_synchronized_long m_current_channel_count[5];
	rectangle2d         m_last_known_viewport_bounds[5];
	rex::be<int32_t>    m_had_ui_frames_ago[4];
};
static_assert(sizeof(c_window_manager) == 0x118);

c_window_manager* window_manager_get()
{
	c_window_manager* result = rex::Runtime::instance()->memory()->TranslateVirtual<c_window_manager*>(0x8298CBC0);
	return result;
}

void user_interface_controller_input_event_submit(const s_event_record* event)
{
	c_window_manager* window_manager = window_manager_get();
	window_manager->handle_global_controller_event(event);
}

void c_window_manager::handle_global_controller_event(const s_event_record* event)
{
	s_data_array* active_screens = rex::Runtime::instance()->memory()->TranslateVirtual<s_data_array*>(m_active_screens);

	int16_t value = event->event_value.get();
	const char* controller_component_name = k_controller_component_names[event->component.get()];
	e_controller_index controller_index = event->controller.get();
	const char* event_type_name = k_event_type_names[event->type.get()];

	printf("user_interface_controller_input_event_submit(event = { %s, %d, %s, %d })\n",
		event_type_name,
		controller_index,
		controller_component_name,
		value);

	// broken but no crash, keeping the call
	rex::GuestToHostFunction<void>(__imp__rex_c_window_manager_handle_global_controller_event, this, event);
}

void event_manager_button_pressed(e_controller_index controller_index, uint8_t button)
{
	s_event_record event = {};

	event.type = _event_type_button_press;
	event.controller = controller_index;

	switch (button)
	{
	case _gamepad_analog_button_left_trigger:
		event.component = _controller_component_button_left_trigger;
		break;
	case _gamepad_analog_button_right_trigger:
		event.component = _controller_component_button_right_trigger;
		break;
	case _gamepad_binary_button_start:
		event.component = _controller_component_button_start;
		break;
	case _gamepad_binary_button_back:
		event.component = _controller_component_button_back;
		break;
	case _gamepad_binary_button_left_thumb:
		event.component = _controller_component_button_left_thumb;
		break;
	case _gamepad_binary_button_right_thumb:
		event.component = _controller_component_button_right_thumb;
		break;
	case _gamepad_binary_button_a:
		event.component = _controller_component_button_a;
		break;
	case _gamepad_binary_button_b:
		event.component = _controller_component_button_b;
		break;
	case _gamepad_binary_button_x:
		event.component = _controller_component_button_x;
		break;
	case _gamepad_binary_button_y:
		event.component = _controller_component_button_y;
		break;
	case _gamepad_binary_button_left_bumper:
		event.component = _controller_component_button_left_shoulder;
		break;
	case _gamepad_binary_button_right_bumper:
		event.component = _controller_component_button_right_shoulder;
		break;
	default:
		throw("unreachable");
		break;
	}
	event.event_value = 255;
	user_interface_controller_input_event_submit(&event);
}
PPC_HOOK(rex_event_manager_button_pressed, event_manager_button_pressed);
