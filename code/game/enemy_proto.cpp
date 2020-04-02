#include "gameplay/enemy_proto.h"

STATIC std::unordered_map<string, enemy_proto*> enemy_proto::s_enemy_protos;

enemy_proto::enemy_proto(const xml::node& xml_node)
{
	init_proto_from_xml_node(xml_node);
}

void enemy_proto::init_proto_from_xml_node(const xml::node& xml_node)
{
	strcpy_s(m_first_stage, xml_node.attribute("first-stage").value());
	xml::node shape = xml_node.child("shape");
	m_size.x = xml::get_attr(shape, "w", 1.f);
	m_size.y = xml::get_attr(shape, "h", 1.f);
	m_radius = xml::get_attr(shape, "r", 1.f);
	xml::node anim = xml_node.child("anim");
	m_is_static_frame = xml::get_attr(anim, "static", false);
	m_anim_id = xml::get_attr(anim, "id", "default");
	
	xml::node custom  = xml_node.child("custom");
	m_custom_movement = movement(custom);
}

movement::stage* enemy_proto::get_movement_stage_by_id(const string& id) const
{
	return m_custom_movement.get_stage_by_id(id);
}

void enemy_proto::load_enemy_proto(const string& id, const char* xml_path)
{
	xml::document* doc = xml::load_file(xml_path);
	auto found = s_enemy_protos.find(id);
	if (found != std::end(s_enemy_protos)) {
		delete found->second;
	}
	enemy_proto* created = new enemy_proto(doc->child("enemy"));
	s_enemy_protos[id] = created;
	xml::unload_file(doc);
}

enemy_proto* enemy_proto::get_enemy_proto(const string& id)
{
	auto found = s_enemy_protos.find(id);
	if (found != std::end(s_enemy_protos)) {
		return found->second;
	}
	return nullptr;
}