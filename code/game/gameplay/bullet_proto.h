#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include <unordered_map>
#include "glare/math/vector.h"
#include "glare/data/xml_utils.h"
#include "gameplay/movement.h"

class bullet_proto
{
public:

public:
	bullet_proto() = default;
	bullet_proto(const xml::node& xml_node);
	bullet_proto(const char* xml_path);
	void init_proto_from_xml_node(const xml::node& xml_node);
	NODISCARD movement::stage* get_movement_stage_by_id(const string& id) const;

	static void load_bullet_proto(const char* xml_path);
	static bullet_proto* get_bullet_proto(const string& id);
public:
	char m_proto_id[GAME_MAX_ID_LENGTH] {""};

	// start stage
	vec2	m_start_position;
	float32 m_start_rotation = 0.f;
	bool	m_start_local_move = false;
	char	m_start_next_stage[GAME_MAX_ID_LENGTH] {"NONE"};
	bool	m_end_effect = true;
	movement	m_custom_movement;
	
public:
	static std::unordered_map<string, bullet_proto*> s_bullet_protos;
};