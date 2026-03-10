#ifndef __USER_INTERFACE_CONTROLLER_H__
#define __USER_INTERFACE_CONTROLLER_H__
#pragma once

/* ---------- headers */

#include "cseries/cseries_macros.h"

/* ---------- constants */

FORWARD_DECLARE_ENUM(e_controller_index);
FORWARD_DECLARE_ENUM(e_campaign_id);
FORWARD_DECLARE_ENUM(e_map_id);

/* ---------- definitions */

struct s_user_interface_controller;
struct s_event_record;
struct s_player_identifier;
struct s_player_configuration;

/* ---------- prototypes */

extern inline s_user_interface_controller* user_interface_controller_get(e_controller_index controller_index);
extern void user_interface_controller_initialize();
extern void user_interface_controller_dispose();
extern void user_interface_queue_event(s_event_record const* event);
extern void user_interface_controller_update();
extern void user_interface_controller_event_manager_suppress(bool suppress);
extern bool user_interface_controller_connected(e_controller_index controller_index);
extern long user_interface_controller_get_user_index(e_controller_index controller_index);
extern void user_interface_controller_set_user_index(e_controller_index controller_index, long user_index);
extern e_controller_index user_interface_controller_get_controller_for_user(long user_index);
extern short user_interface_controller_get_signed_in_controller_count();
extern short user_interface_controller_get_signed_in_non_guest_controller_count();
extern void user_interface_controller_set_desired_team_index(e_controller_index controller_index, long desired_team_index);
extern long user_interface_controller_get_team_index(e_controller_index controller_index);
extern void user_interface_controller_set_griefer(e_controller_index controller_index, bool griefer);
extern bool user_interface_controller_get_griefer(e_controller_index controller_index, bool griefer);
extern void user_interface_controller_set_bungienet_user(e_controller_index controller_index, unsigned long bungienet_user_flags);
extern unsigned long user_interface_controller_get_bungienet_user(e_controller_index controller_index);
extern void user_interface_controller_set_hopper_access(e_controller_index controller_index, unsigned char hopper_access_flags);
extern unsigned char user_interface_controller_get_hopper_access(e_controller_index controller_index);
extern bool user_interface_controller_get_notification_pending(e_controller_index controller_index);
extern void user_interface_controller_set_notification_pending(e_controller_index controller_index, bool notification_pending);
extern void user_interface_controller_clear_voting_for_all_controllers();
extern void user_interface_controller_set_desires_veto(e_controller_index controller_index, bool desires_veto);
extern void user_interface_controller_set_desires_rematch(e_controller_index controller_index, bool desires_rematch);
extern void user_interface_controller_get_player_configuration(e_controller_index controller_index, s_player_identifier* player_identifier, s_player_configuration* player_data);
extern void user_interface_controller_update_network_properties(e_controller_index controller_index);
extern bool user_interface_controller_xbox_live_account_signed_in(e_controller_index controller_index);
extern void user_interface_controller_xbox_live_account_set_signed_in(e_controller_index controller_index, bool signed_in);
extern bool user_interface_controller_xbox_live_is_guest_account(e_controller_index controller_index);
extern e_controller_index user_interface_controller_get_first_xbox_live_signed_in_non_guest();
extern short user_interface_controller_get_signed_in_live_account_count();
extern short user_interface_controller_get_signed_in_master_live_account_count();
extern short user_interface_controller_get_signed_in_controllers_mask();
extern short user_interface_controller_get_live_account_controllers_mask();
extern e_controller_index user_interface_controller_get_next_controller(e_controller_index controller_index);
extern bool user_interface_controller_is_player_profile_valid(e_controller_index controller_index);
extern bool user_interface_controller_is_anyone_signed_into_xbox_live();
extern bool user_interface_controller_get_rumble_enabled(e_controller_index controller_index);
extern bool user_interface_controller_get_impulse_triggers_enabled(e_controller_index controller_index);
extern bool user_interface_controller_get_autolevel_enabled(e_controller_index controller_index);
extern wchar_t const* user_interface_controller_get_player_profile_name(e_controller_index controller_index);
extern long user_interface_get_player_last_used_hopper(e_controller_index controller_index);
extern void user_interface_set_player_last_used_hopper(e_controller_index controller_index, unsigned short hopper_identifier);
extern void user_interface_set_last_campaign_level(e_campaign_id campaign_id, e_map_id map_id);
extern void user_interface_controller_hack_invert_look_all_controllers();
extern void user_interface_controller_hack_look_speed_all_controllers(short look_speed);
extern void user_interface_controller_reset_any_offline_controllers();
extern bool user_interface_controller_voice_allowed(e_controller_index controller_index);
extern bool user_interface_controller_voice_is_for_friends_only(e_controller_index controller_index);
extern bool user_interface_any_controller_voice_not_allowed();
extern bool user_interface_any_controller_voice_is_for_friends_only();
extern void user_interface_controller_detached(e_controller_index controller_index);
extern void user_interface_controller_attached(e_controller_index controller_index);
extern void user_interface_controllers_perform_storage_device_selection_checks();
extern inline bool dpad_button_tabbed(bool tab_is_vertical, unsigned char button_frames, unsigned short button_msec);
extern void user_interface_controller_reset(e_controller_index controller_index);
extern void user_interface_controller_switch_to_offline(e_controller_index controller_index);
extern void user_interface_get_last_campaign_level(e_campaign_id* campaign_id, e_map_id* map_id);
extern char* user_interface_get_last_campaign_level_path(char* path, long maximum_characters);

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __USER_INTERFACE_CONTROLLER_H__
