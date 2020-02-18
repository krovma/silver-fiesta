#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include <unordered_map>
#include "glare/math/vector.h"

constexpr size_t GAME_MAX_ID_LENGTH = 64;

enum e_bullet_movement
{
	BLMV_FORWARD = 0,
	BLMV_TO_PLAYER,
	BLMV_TO_SPAWNER,
	BLMV_SINUSODIAL,
	BLMV_CIRCLE,
	BLMV_RANDOM,
	NUM_BULLET_MOVEMENT
};

class bullet_proto
{
public:
	struct custom
	{
		char m_custom_id[GAME_MAX_ID_LENGTH] {0};
		char m_next_stage[GAME_MAX_ID_LENGTH] {"NONE"};

		// BLMV_FORWARD
		float32 m_blmv_forward_speed = 10.f;
		float32 m_blmv_forward_time =  10.f;
		// BLMV_TO_PLAYER
		float32 m_blmv_to_player_speed = 10.f;
		float32 m_blmv_to_player_time =  -1.f;
		// BLMV_TO_SPAWNER
		float32 m_blmv_to_spawner_speed = 10.f;
		float32 m_blmv_to_spawner_time =  -1.f;
		// BLMV_SINUSODIAL
		// BLMV_RANDOM
		
		bool m_use[NUM_BULLET_MOVEMENT] {false};
	};

public:
	bullet_proto()
	{
		m_custom_stages.reserve(64);
	}
	const custom* get_custom_stage(const string& id) const;
public:
	char m_proto_id[GAME_MAX_ID_LENGTH] {""};

	// start stage
	vec2	m_start_position;
	float32 m_start_rotation = 0.f;
	bool	m_start_local_move = false;
	char	m_start_next_stage[GAME_MAX_ID_LENGTH] {"NONE"};

	bool	m_end_effect = true;
	
	std::vector<custom> m_custom_stages;
	
public:
	static std::unordered_map<string, bullet_proto*> s_bullet_protos;
};