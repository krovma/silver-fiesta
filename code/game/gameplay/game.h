#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include "glare/render/mesh.h"
#include "gameplay/entity.h"

class bullet_proto;

namespace glare
{
	class shader;
	class sprite_sheet;
};

class app;
class bullet_proto_editor;
class game
{
public:
	game(app* app);
	~game();
	void start();
	void begin_frame();
	void update(float32 delta_sec);
	void render() const;
	void end_frame();
	void stop();

public:
	void create_test_bullet(bullet_proto& proto);
public:
	app*	m_app = nullptr;
	shader* m_unlit_shader = nullptr;
	sprite_sheet* m_test_sprite = nullptr;
	mesh*	m_test_mesh = nullptr;

	entity* m_test_bullet = nullptr;
	entity* m_root = nullptr;
	
	bullet_proto_editor*	m_proto_editor = nullptr;
};
