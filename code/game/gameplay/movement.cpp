#include "gameplay/movement.h"

movement::stage::stage(const stage& copy)
{
	memcpy(m_movement_stage_id, copy.m_movement_stage_id, sizeof(char) * GAME_MAX_ID_LENGTH);
	memcpy(m_next_movement_stage, copy.m_next_movement_stage, sizeof(char) * GAME_MAX_ID_LENGTH);
	m_blmv_forward_speed	= copy.m_blmv_forward_speed;
	m_blmv_forward_time		= copy.m_blmv_forward_time;
	m_blmv_to_player_speed	= copy.m_blmv_to_player_speed;
	m_blmv_to_player_time	= copy.m_blmv_to_player_time;
	m_blmv_to_spawner_speed	= copy.m_blmv_to_spawner_speed;
	m_blmv_to_spawner_time	= copy.m_blmv_to_spawner_time;
	m_blmv_spawn_offset		= copy.m_blmv_spawn_offset;
	m_blmv_spawn_id			= copy.m_blmv_spawn_id;
	m_blmv_spawn_rotation	= copy.m_blmv_spawn_rotation;
	m_blmv_moveby_direction = copy.m_blmv_moveby_direction;
	m_blmv_moveby_time		= copy.m_blmv_moveby_time;
	memcpy(m_use, copy.m_use, sizeof(bool) * NUM_BULLET_MOVEMENT);
}

movement::movement()
{
	m_stages.reserve(64);
}

movement::movement(const xml::node& xml_node, bool reserve_for_edit)
{
	if (reserve_for_edit) {
		m_stages.reserve(64);
	}
	auto stages_xml = xml_node.children("stage");
	for (auto& each_stage: stages_xml) {
		auto& new_stage = m_stages.emplace_back();
		strcpy_s(new_stage.m_movement_stage_id, xml::get_attr(each_stage, "id", "NONE").c_str());
		strcpy_s(new_stage.m_next_movement_stage, xml::get_attr(each_stage, "next", "NONE").c_str());
		auto moves        = each_stage.child("movements");
		auto blmv_forward = moves.child("move_forward");
		if (!blmv_forward.empty()) {
			new_stage.m_use[MV_FORWARD]  = true;
			new_stage.m_blmv_forward_speed = xml::get_attr(blmv_forward, "speed", 1.0f);
			new_stage.m_blmv_forward_time  = xml::get_attr(blmv_forward, "time", 1.0f);
		}
		auto blmv_toplayer = moves.child("move_toplayer");
		if (!blmv_toplayer.empty()) {
			new_stage.m_use[MV_TO_PLAYER]  = true;
			new_stage.m_blmv_to_player_speed = xml::get_attr(blmv_toplayer, "speed", 1.0f);
			new_stage.m_blmv_to_player_time  = xml::get_attr(blmv_toplayer, "time", 1.0f);

		}
		auto blmv_tospawner = moves.child("move_tospawner");
		if (!blmv_tospawner.empty()) {
			new_stage.m_use[MV_TO_SPAWNER]  = true;
			new_stage.m_blmv_to_spawner_speed = xml::get_attr(blmv_tospawner, "speed", 1.0f);
			new_stage.m_blmv_to_spawner_time  = xml::get_attr(blmv_tospawner, "time", 1.0f);
		}
		auto blmv_moveby = moves.child("moveby");
		if (!blmv_moveby.empty()) {
			new_stage.m_use[MV_MOVEBY]  = true;
			new_stage.m_blmv_moveby_direction = xml::get_attr(blmv_moveby, "dir", vec2::ZERO);
			new_stage.m_blmv_moveby_time  = xml::get_attr(blmv_moveby, "time", 1.0f);
		}
		auto blmv_spawn = moves.child("shoot");
		if (!blmv_spawn.empty()) {
			new_stage.m_use[MV_SPAWN]  = true;
			new_stage.m_blmv_spawn_id = xml::get_attr(blmv_spawn, "id", "default");
			new_stage.m_blmv_spawn_offset = xml::get_attr(blmv_spawn, "offset", vec2::ZERO);
			new_stage.m_blmv_spawn_rotation = xml::get_attr(blmv_spawn, "rotation", 0.f);
		}
	}
	
}

NODISCARD movement::stage* movement::get_stage_by_id(const string& id) const
{
	auto& stages = const_cast<std::vector<stage>&>(m_stages);
	for(auto& each : stages) {
		if (each.m_movement_stage_id == id) {
			return &each;
		}
	}
	return nullptr;
}
