#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include "glare/render/mesh.h"
#include "gameplay/entity.h"
#include "gameplay/game_mode.h"
#include "glare/core/event.h"
#include <vector>


namespace glare
{
	class shader;
	class sprite_sheet;
};

class app;
class bullet;
class enemy;
class game_play : public game_mode
{
public:
	game_play(app* app);
	virtual ~game_play();
	void start() override;
	void begin_frame() override;
	void update(float32 delta_sec) override;
	void render() const override;
	void end_frame() override;
	void stop() override;


	NODISCARD player* get_player() const override {return m_player;}

	// event handler
	EVENT_PROC(spawn_bullet);
	void on_keyboard(bool keydown, byte keycode) override;
	
	app*	m_app = nullptr;
	shader* m_unlit_shader = nullptr;
	sprite_sheet* m_test_sprite = nullptr;
	mesh*	m_test_mesh = nullptr;

	entity* m_test_bullet = nullptr;
	entity* m_root = nullptr;
	player*	m_player = nullptr;

	vec2	m_player_moving_input;
	std::vector<bullet*> m_player_bullets;
	std::vector<bullet*> m_enemy_bullets;
	std::vector<enemy*>	m_enemies;

	float debug_next_enemy = 1.f;
};
