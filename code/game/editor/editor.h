#pragma once
#include "glare/core/common.h"
#include "glare/data/xml_utils.h"

#include <vector>
#include "glare/math/vector.h"
using namespace glare;
constexpr size_t NAME_BUFFER_SIZE = 64;

class editor
{
public:
	editor() = default;
	virtual ~editor() { delete m_xml_doc; }
	virtual void start(const char* xml_path) = 0;
	virtual void update() = 0;
	virtual void end() = 0;
public:
	xml::document*	m_xml_doc = nullptr;
};

class editor_frame
{
public:
	editor_frame() = default;
	virtual ~editor_frame() = default;
	virtual void create(const xml::node& node)
	{
		m_xml_node = node;
	}
	virtual void update() = 0;
	virtual void destroy() = 0;
public:
	xml::node	m_xml_node;
	bool		m_active = true;
};

class bullet_proto_start_frame : public editor_frame
{
public:
	void create(const xml::node& node) override;
	void update() override;
	void destroy() override;

	char m_next_stage[NAME_BUFFER_SIZE] {"NONE"};
	vec2	m_position;
	float32 m_rotation;
	bool	m_local_move = false;
};

class bullet_proto_custom_frame : public editor_frame
{
public:
	static inline constexpr size_t NUM_MOVEMENTS = 1;
public:
	void create(const xml::node& node) override;
	void update() override;
	void destroy() override;

public:
	char	m_stage_id[NAME_BUFFER_SIZE] {""};
	char	m_tmp_buf[NAME_BUFFER_SIZE] {""};
	bool	m_tmp_popup;
	// Movements list
	// 1. Go self-forward
	float32		m_1_forward_speed = 1.f;
	float32		m_1_forward_time  = 1.f;

	// transit
	bool	m_use[NUM_MOVEMENTS];
	char	m_next_stage[NAME_BUFFER_SIZE] {"NONE"};
};

class bullet_proto_end_frame : public editor_frame
{
public:
	void create(const xml::node& node) override;
	void update() override;
	void destroy() override;

	bool m_effect = true;
};


class bullet_proto_editor : public editor
{
public:
	bullet_proto_editor();
	~bullet_proto_editor() override;
	void start(const char* xml_path) override;
	void update() override;
	void end() override;

	void add_new_stage();
	//void remove_stage();
public:
	bullet_proto_start_frame*	m_start_frame = nullptr;
	bullet_proto_end_frame*		m_end_frame = nullptr;

	xml::node	m_custom_frame_node;
	std::vector<bullet_proto_custom_frame*>	m_custom_frames;
	string	m_xml_path;
	char	m_bullet_id[NAME_BUFFER_SIZE] {""};
	
};

