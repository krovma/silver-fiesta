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
class map;
class map_step;

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
	EVENT_PROC(use_bomb);
	void on_keyboard(bool keydown, byte keycode) override;
	
	void _update_map();
	void _handle_map_step(map_step* step);
	void _spawn_enemy(const string& enemy_def_id, const vec2& position, bool last=false);
	void _load_protos();
	
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
	std::vector<bullet*> m_new_bullets;
	std::vector<enemy*>	m_enemies;

	size_t	m_num_maps = 0;
	map*	m_current_map = nullptr;
	
	float debug_next_enemy = 1.f;
};
