#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include <vector>
#include "glare/data/xml_utils.h"
#include "glare/math/vector.h"

enum e_movement_type_t
{
	MV_FORWARD = 0,
	MV_TO_PLAYER,
	MV_TO_SPAWNER,
	MV_SPAWN,
	MV_MOVEBY,
	NUM_BULLET_MOVEMENT
};

class movement
{
public:
	struct stage
	{
		stage() = default;
		stage(const stage& copy);
		
		char m_movement_stage_id[GAME_MAX_ID_LENGTH] {0};
		char m_next_movement_stage[GAME_MAX_ID_LENGTH] {"NONE"};
		// MV_FORWARD
		float32 m_blmv_forward_speed = 10.f;
		float32 m_blmv_forward_time =  10.f;
		// MV_TO_PLAYER
		float32 m_blmv_to_player_speed = 10.f;
		float32 m_blmv_to_player_time =  -1.f;
		// MV_TO_SPAWNER
		float32 m_blmv_to_spawner_speed = 10.f;
		float32 m_blmv_to_spawner_time =  -1.f;
		// MV_SPAWN
		vec2	m_blmv_spawn_offset = vec2::ZERO;
		string	m_blmv_spawn_id;
		float32 m_blmv_spawn_rotation = 0.f;
		// MV_MOVEBY
		vec2	m_blmv_moveby_direction = vec2::ZERO;
		float32 m_blmv_moveby_time = 0.f;
		bool m_use[NUM_BULLET_MOVEMENT] {false};
	};

	movement();
	movement(const xml::node& xml_node, bool reserve_for_edit=false);
	
	NODISCARD stage* get_stage_by_id(const string& id) const;
	
public:
	std::vector<stage> m_stages;
};