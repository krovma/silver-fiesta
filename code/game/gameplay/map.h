#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include "glare/core/event.h"
#include <vector>
#include "glare/data/xml_utils.h"

enum e_map_step_t
{
	STEP_DELAY,
	STEP_WAIT,
	STEP_SPAWN,
	STEP_EVENT
};

class map_step
{
public:
	map_step() = default;
	map_step(e_map_step_t type);
	EVENT_PROC(continue_callback);
public:
	e_map_step_t m_type = STEP_DELAY;
	uint32	m_unique_mapstep_id = 0;
	float32	m_delay_time = 0.f;
	vec2	m_position = vec2::ZERO;
	string	m_event_enemy_id = "";
	bool m_proceed = true;
	bool m_last = false;
};

class map
{
public:
	map() = default;
	map(const xml::node& xml_node);
	map_step* get_current_map_step() const;
	void advance_step();
public:
	std::vector<map_step> m_map_steps;
	size_t m_current_step_index = 0;
};