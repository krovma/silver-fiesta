#pragma once
#include "glare/data/xml_utils.h"
#include "gameplay/movement.h"
#include <unordered_map>
using namespace glare;

class enemy_proto
{
public:
	enemy_proto() = default;
	enemy_proto(const xml::node& xml_node);
	//bullet_proto(const char* xml_path);
	void init_proto_from_xml_node(const xml::node& xml_node);
	NODISCARD movement::stage* get_movement_stage_by_id(const string& id) const;

	static void load_enemy_proto(const string& id, const char* xml_path);
	static enemy_proto* get_enemy_proto(const string& id);
public:
	char		m_first_stage[GAME_MAX_ID_LENGTH] {"NONE"};
	movement	m_custom_movement;
	vec2	m_size = vec2::ZERO;
	float	m_radius = 0.f;
	bool	m_is_static_frame = false;
	string	m_anim_id;
public:
	static std::unordered_map<string, enemy_proto*> s_enemy_protos;
};


