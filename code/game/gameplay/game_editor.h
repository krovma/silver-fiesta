#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include "glare/render/mesh.h"
#include "gameplay/entity.h"
#include "gameplay/game_mode.h"
class bullet_proto;

namespace glare
{
	class shader;
	class sprite_sheet;
};

class app;
class bullet_proto_editor;

class game_editor : public game_mode
{
public:
	game_editor(app* app);
	~game_editor();
	void start() override;
	void begin_frame() override;
	void update(float32 delta_sec) override;
	void render() const override;
	void end_frame() override;
	void stop() override;

	NODISCARD player* get_player() const override { return nullptr; }
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
