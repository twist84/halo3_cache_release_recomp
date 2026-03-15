#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__
#pragma once

/* ---------- headers */

#include "cseries/platform.h"

#include "rex_macros.h"

/* ---------- constants */

FORWARD_DECLARE_ENUM(e_experience_rank);
FORWARD_DECLARE_ENUM(e_experience_grade);
FORWARD_DECLARE_ENUM(e_controller_index);
FORWARD_DECLARE_ENUM(e_campaign_id);
FORWARD_DECLARE_ENUM(e_map_id);
FORWARD_DECLARE_ENUM(e_game_engine_type);

/* ---------- definitions */

// $TODO move to objects/emblems.h
struct s_emblem_info
{
	unsigned char foreground_emblem_index; // 0x0
	unsigned char background_emblem_index; // 0x1
	unsigned char emblem_info_flags; // 0x2
	char primary_color_index; // 0x3
	char secondary_color_index; // 0x4
	char background_color_index; // 0x5
	unsigned short pad; // 0x6
};
static_assert(sizeof(s_emblem_info) == 0x8);

// $TODO move to game/players.h
struct s_player_identifier
{
	unsigned char identifier[8]; // 0x0
};
static_assert(sizeof(s_player_identifier) == 0x8);

// $TODO move to game/players.h
struct s_machine_identifier
{
	unsigned char identifier[6]; // 0x0
};
static_assert(sizeof(s_machine_identifier) == 0x6);

// $TODO move to game/players.h
struct s_player_appearance
{
	unsigned char change_color_index[3]; // 0x0
	unsigned char player_model_choice; // 0x3
	s_emblem_info emblem_info; // 0x4
	unsigned char model_permutations[2][4]; // 0xC
	wchar_t service_tag[4]; // 0x14

	char __data[4]; // 0x1C
};
static_assert(sizeof(s_player_appearance) == 0x20);

// $TODO move to game/players.h
struct s_queried_player_global_statistics
{
	bool valid; // 0x0
	rex::be<long> experience_base; // 0x4
	rex::be<long> experience_penalty; // 0x8
	rex::be<long> highest_skill; // 0xC
};
static_assert(sizeof(s_queried_player_global_statistics) == 0x10);

// $TODO move to game/players.h
struct s_queried_player_displayed_statistics
{
	bool stats_valid; // 0x0
	rex::be<long> ranked_played; // 0x4
	rex::be<long> ranked_completed; // 0x8
	rex::be<long> ranked_win; // 0xC
	rex::be<long> unranked_played; // 0x10
	rex::be<long> unranked_completed; // 0x14
	rex::be<long> unranked_win; // 0x18
	rex::be<long> custom_played; // 0x1C
	rex::be<long> custom_completed; // 0x20
	rex::be<unsigned long> custom_win; // 0x24
	rex::be<unsigned long> last_played; // 0x28
};
static_assert(sizeof(s_queried_player_displayed_statistics) == 0x2C);

// $TODO move to game/players.h
struct s_queried_player_hopper_statistics
{
	bool stats_valid; // 0x0
	rex::be<unsigned short> identifier; // 0x2
	rex::be<float> mu; // 0x4
	rex::be<float> sigma; // 0x8
	rex::be<long> old_skill; // 0xC
	rex::be<long> games_played; // 0x10
	rex::be<long> games_completed; // 0x14
	rex::be<long> games_won; // 0x18
};
static_assert(sizeof(s_queried_player_hopper_statistics) == 0x1C);

// $TODO move to game/players.h
struct s_queried_player_statistics
{
	s_queried_player_global_statistics global_statistics; // 0x0
	s_queried_player_displayed_statistics displayed_statistics; // 0x10
	s_queried_player_hopper_statistics hopper_statistics; // 0x3C
};
static_assert(sizeof(s_queried_player_statistics) == 0x58);

// $TODO move to game/players.h
struct s_player_configuration_from_client
{
	wchar_t desired_name[32]; // 0x0
	s_player_appearance appearance; // 0x40
	rex::be<unsigned long long> player_xuid; // 0x60
	bool is_silver_or_gold_live; // 0x68
	bool is_online_enabled; // 0x69
	unsigned char user_selected_team_index; // 0x6A
	bool desires_veto; // 0x6B
	bool desires_rematch; // 0x6C
	unsigned char hopper_access_flags; // 0x6C
	bool is_free_live_gold_account; // 0x6E
	bool has_beta_permissions; // 0x6F
	bool is_griefer; // 0x70
	unsigned char bungienet_user_flags; // 0x71
	char campaign_completion; // 0x72
	char campaign_percentage; // 0x73
	rex::be<long> gamer_region; // 0x74
	rex::be<long> gamer_zone; // 0x78
	rex::be<unsigned long> cheat_flags; // 0x7C
	rex::be<unsigned long> ban_flags; // 0x80
	rex::be<long> repeated_play_coefficient; // 0x84
	bool experience_growth_banned; // 0x88
	s_queried_player_statistics queried_stats; // 0x8C
};
static_assert(sizeof(s_player_configuration_from_client) == 0xE8);

// $TODO move to game/players.h
struct s_calculated_player_global_statistics
{
	bool valid; // 0x0
	rex::be<long> experience; // 0x4
	rex::be<e_experience_rank> rank; // 0x8
	rex::be<e_experience_grade> grade; // 0xC
};
static_assert(sizeof(s_calculated_player_global_statistics) == 0x10);

// $TODO move to game/players.h
struct s_calculated_player_hopper_statistics
{
	bool valid; // 0x0
	rex::be<long> skill; // 0x4
	rex::be<long> skill_to_display; // 0x8
	rex::be<long> skill_update_weight; // 0xC
};
static_assert(sizeof(s_calculated_player_hopper_statistics) == 0x10);

// $TODO move to game/players.h
struct s_calculated_player_statistics
{
	s_calculated_player_global_statistics global_statistics; // 0x0
	s_calculated_player_hopper_statistics hopper_statistics; // 0x10
};
static_assert(sizeof(s_calculated_player_statistics) == 0x20);

// $TODO move to game/players.h
struct s_player_configuration_from_host
{
	rex::be<long> team_index; // 0x0
	s_calculated_player_statistics player_statistics; // 0x4

	char __data[4]; // 0x24
};
static_assert(sizeof(s_player_configuration_from_host) == 0x28);

// $TODO move to game/players.h
struct s_player_configuration
{
	s_player_configuration_from_client client; // 0x0
	s_player_configuration_from_host host; // 0xE8
};
static_assert(sizeof(s_player_configuration) == 0x110);

// move scenario/scenario_map_variant.h
typedef char c_map_variant[0xB8D0];

class c_game_variant
{
public:
	enum
	{
		k_vtable_pointer_size = 4,
	};

//protected:
	e_game_engine_type m_game_engine_index; // 0x0
	unsigned char m_variant_storage[0x1B0]; // 0x4
};
static_assert(sizeof(c_game_variant) == 0x1B4);

struct s_game_matchmaking_options
{
	rex::be<unsigned short> hopper_identifier; // 0x0
	unsigned char xlast_index; // 0x2
	bool hopper_ranked; // 0x3
	bool team_game; // 0x4
	char pad[1]; // 0x5
	wchar_t hopper_name[32]; // 0x6
	rex::be<long> draw_probability; // 0x48
	rex::be<float> beta; // 0x4C
	rex::be<float> tau; // 0x50
	rex::be<long> experience_base_increment; // 0x54
	rex::be<long> experience_penalty_decrement; // 0x58
};
static_assert(sizeof(s_game_matchmaking_options) == 92);

struct game_player_options
{
	bool valid; // 0x0
	bool player_left_game; // 0x1
	rex::be<short> user_index; // 0x2
	rex::be<e_controller_index> controller_index; // 0x4
	s_machine_identifier machine_identifier; // 0x8
	s_player_identifier player_identifier; // 0xE
	s_player_configuration configuration_data; // 0x18

	char __data[0x40]; // 0x128
};
static_assert(sizeof(game_player_options) == 0x168);

struct game_machine_options
{
	unsigned long machine_valid_mask; // 0x0
	s_machine_identifier machine_identifiers[16]; // 0x4
	bool local_machine_exists; // 0x64
	s_machine_identifier local_machine_identifier; // 0x65
};
static_assert(sizeof(game_machine_options) == 0x6C);

struct game_options
{
	rex::be<long> game_mode; // 0x0
	char game_simulation; // 0x4
	char game_network_type; // 0x5
	rex::be<short> game_tick_rate; // 0x6
	rex::be<unsigned long long> game_instance; // 0x8
	rex::be<unsigned long> random_seed; // 0x10
	rex::be<e_campaign_id> campaign_id; // 0x14
	rex::be<e_map_id> map_id; // 0x18
	char scenario_path[260]; // 0x1C
	rex::be<short> initial_zone_set_index; // 0x120
	bool load_level_only; // 0x122
	char dump_machine_index; // 0x123
	bool dump_object_log; // 0x124
	bool dump_random_seeds; // 0x125
	bool debug_random_seeds; // 0x126
	bool playtest_mode; // 0x127
	rex::be<short> game_playback; // 0x128
	bool record_saved_film; // 0x12A
	rex::be<short> campaign_difficulty; // 0x12C
	bool campaign_allow_persistent_storage; // 0x12E
	bool matchmade_game; // 0x12F
	char pad1[4]; // 0x130
	s_game_matchmaking_options matchmaking_options; // 0x134
	c_game_variant multiplayer_variant; // 0x190
	c_map_variant map_variant; // 0x344
	game_machine_options machines; // 0xBC14
	game_player_options players[16]; // 0xBC80
};
static_assert(sizeof(game_options) == 0xD300);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __GAME_OPTIONS_H__
