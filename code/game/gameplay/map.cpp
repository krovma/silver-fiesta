#include "gameplay/map.h"

static uint32 _unique_id = 1;

map_step::map_step(e_map_step_t type)
	: m_type(type)
	, m_unique_mapstep_id(++_unique_id)
{
}

bool map_step::continue_callback(glare::dict& args)
{
	if (m_type == STEP_WAIT || args.get<uint32>("id", 0u) == m_unique_mapstep_id) {
		m_proceed = true;
		return true;
	}
	return false;
}

map::map(const xml::node& xml_node)
{
	auto xml_step_list = xml_node.children();
	for (auto& xml_step : xml_step_list) {
		const string step_name = xml_step.name();
		if (step_name == "delay") { //wait for event "mapstep-delay"
			auto& new_step = m_map_steps.emplace_back(STEP_DELAY);
			new_step.m_delay_time = xml::get_attr(xml_step, "time", 0.f);
			new_step.m_proceed = false;
		} else if (step_name == "wait") {
			auto& new_step = m_map_steps.emplace_back(STEP_WAIT);
			new_step.m_event_enemy_id = xml::get_attr(xml_step, "event", "DEFAULT");
			new_step.m_proceed = false;
		} else if (step_name == "spawn") {
			auto& new_step = m_map_steps.emplace_back(STEP_SPAWN);
			new_step.m_event_enemy_id = xml::get_attr(xml_step, "id", "DEFAULT");
			new_step.m_position = xml::get_attr(xml_step, "position", vec2::ZERO);
			new_step.m_last = xml::get_attr(xml_step, "last", false);
			new_step.m_proceed = true;
		} else if (step_name == "event") {
			// NOT IMPLEMENTED
			//new_step.m_proceed = true;
		}
	}
}

map_step* map::get_current_map_step() const
{
	if (m_current_step_index >= m_map_steps.size()) {
		return nullptr;
	}
	return const_cast<map_step*>(&m_map_steps[m_current_step_index]);
}

void map::advance_step()
{
	auto& current_step = m_map_steps[m_current_step_index];
	if (current_step.m_type == STEP_DELAY) {
		event::unsubscribe("mapstep-delay", {&current_step, &map_step::continue_callback});
	} else if (current_step.m_type == STEP_WAIT) {
		event::unsubscribe(current_step.m_event_enemy_id, {&current_step, &map_step::continue_callback});
	}
	++m_current_step_index;
}
