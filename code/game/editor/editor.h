#pragma once
#include "glare/core/common.h"
#include "glare/data/xml_utils.h"
#include <vector>
#include "glare/math/vector.h"
#include "gameplay/bullet_proto.h"

using namespace glare;
class game_editor;
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
	virtual void create(const xml::node& node, void* object)
	{
		m_editor_xml_node = node;
		m_object = object;
	}
	virtual void update() = 0;
	virtual void destroy() = 0;
	virtual void update_xml_node() = 0;
public:
	xml::node	m_editor_xml_node;
	void*		m_object=nullptr;
	bool		m_active = true;
};

class bullet_proto_start_frame : public editor_frame
{
public:
	void create(const xml::node& node, void* proto) override;
	void update() override;
	void destroy() override;
	void update_xml_node() override;

	bullet_proto* m_proto = nullptr;
};

class bullet_proto_custom_frame : public editor_frame
{
public:
	static inline constexpr size_t NUM_MOVEMENTS = 1;
public:
	void create(const xml::node& node, void* proto) override;
	void update() override;
	void destroy() override;
	void update_xml_node() override;

	char	m_rename_buf[GAME_MAX_ID_LENGTH] {""};
	bool	m_rename_popup;

	movement::stage* m_proto_stage = nullptr;
};

class bullet_proto_end_frame : public editor_frame
{
public:
	void create(const xml::node& node, void* proto) override;
	void update() override;
	void destroy() override;
	void update_xml_node() override;

	bullet_proto*	m_proto=nullptr;
};


class bullet_proto_editor : public editor
{
public:
	bullet_proto_editor(game_editor* game);
	~bullet_proto_editor() override;
	void start(const char* xml_path) override;
	void update() override;
	void end() override;

	void add_new_stage();
	//void remove_stage();
	bool save_xml();
public:
	game_editor*						m_game = nullptr;
	bullet_proto_start_frame*	m_start_frame = nullptr;
	bullet_proto_end_frame*		m_end_frame = nullptr;

	xml::node	m_custom_frame_node;
	//xml::node   m_proto_root;
	std::vector<bullet_proto_custom_frame*>	m_custom_editor_frames;
	string	m_xml_path;
	bullet_proto	m_tmp_proto;
};

