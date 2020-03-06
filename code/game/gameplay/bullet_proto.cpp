#include "gameplay/bullet_proto.h"

STATIC std::unordered_map<string, bullet_proto*> bullet_proto::s_bullet_protos;

bullet_proto::bullet_proto(const xml::node& xml_node)
{
	init_proto_from_xml_node(xml_node);
}

void bullet_proto::init_proto_from_xml_node(const xml::node& xml_node)
{
	strcpy_s(m_proto_id, xml_node.attribute("id").value());
	xml::node start = xml_node.child("start");
	strcpy_s(m_start_next_stage, xml::get_attr(start, "next", "NONE").c_str());
	m_start_position = xml::get_attr(start, "position", vec2::ZERO);
	m_start_rotation = xml::get_attr(start, "rotation", 0.f);
	m_start_rotation = xml::get_attr(start, "local", false);
	xml::node end    = xml_node.child("end");
	m_end_effect     = xml::get_attr(end, "effect", false);

	xml::node custom  = xml_node.child("custom");
	auto      xml_customs = custom.children("frame");
	for (auto& each_frame : xml_customs) {
		auto& custom_frame = m_custom_stages.emplace_back(bullet_proto::custom());

		strcpy_s(custom_frame.m_custom_id, xml::get_attr(each_frame, "id", "NONE").c_str());
		strcpy_s(custom_frame.m_next_stage, xml::get_attr(each_frame, "next", "NONE").c_str());
		auto moves        = each_frame.child("movements");
		auto blmv_forward = moves.child("move_forward");
		if (!blmv_forward.empty()) {
			custom_frame.m_use[BLMV_FORWARD]  = true;
			custom_frame.m_blmv_forward_speed = xml::get_attr(blmv_forward, "speed", 1.0f);
			custom_frame.m_blmv_forward_time  = xml::get_attr(blmv_forward, "time", 1.0f);
		}
		auto blmv_toplayer = moves.child("move_toplayer");
		if (!blmv_toplayer.empty()) {
			custom_frame.m_use[BLMV_TO_PLAYER]  = true;
			custom_frame.m_blmv_to_player_speed = xml::get_attr(blmv_toplayer, "speed", 1.0f);
			custom_frame.m_blmv_to_player_time  = xml::get_attr(blmv_toplayer, "time", 1.0f);

		}
		auto blmv_tospawner = moves.child("move_tospawner");
		if (!blmv_tospawner.empty()) {
			custom_frame.m_use[BLMV_TO_SPAWNER]  = true;
			custom_frame.m_blmv_to_spawner_speed = xml::get_attr(blmv_tospawner, "speed", 1.0f);
			custom_frame.m_blmv_to_spawner_time  = xml::get_attr(blmv_tospawner, "time", 1.0f);

		}
	}
}

const bullet_proto::custom* bullet_proto::get_custom_stage(const string& id) const
{
	for(auto& each : m_custom_stages) {
		if (id == each.m_custom_id) {
			return &each;
		}
	}
	return nullptr;
}

void bullet_proto::load_bullet_proto(const char* xml_path)
{
	xml::document* doc = xml::load_file(xml_path);
	string id = xml::get_attr(doc->child("bullet_proto"), "id", "default");
	auto found = s_bullet_protos.find(id);
	if (found != std::end(s_bullet_protos)) {
		delete found->second;
	}
	bullet_proto* created = new bullet_proto(doc->child("bullet_proto"));
	s_bullet_protos[id] = created;
	xml::unload_file(doc);
}

bullet_proto* bullet_proto::get_bullet_proto(const string& id)
{
	auto found = s_bullet_protos.find(id);
	if (found != std::end(s_bullet_protos)) {
		return found->second;
	}
	return nullptr;
}
