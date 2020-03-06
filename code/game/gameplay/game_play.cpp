#include "gameplay/game_play.h"
#include "framework/app.h"
#include "glare/render/renderer.h"
#include "glare/render/shader.h"
#include "glare/core/color.h"
#include "glare/core/input.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/vertex.h"
#include "glare/render/sprite.h"
#include "editor/editor.h"
#include "imgui/imgui.h"
#include "gameplay/bullet.h"
#include "gameplay/player.h"
#include "glare/math/utilities.h"
#include "gameplay/enemy.h"

extern renderer* g_renderer;

game_play::game_play(app* app)
	:m_app(app)
{
	g_renderer = m_app->m_renderer;
}
game_play::~game_play()
{
	delete m_unlit_shader;
}

void game_play::start()
{
	event::subscribe("game-spawn-bullet", {this, &game_play::spawn_bullet});
	
	m_unlit_shader = shader::load_from_xml("data/shader/unlit.shader", m_app->m_renderer);
	g_renderer->m_buffer_vbo->set_buffer_layout(buffer_layout::acquire_layout_of<vertex_pcu>());
	m_unlit_shader->create_dx_vbo_layout(g_renderer->m_buffer_vbo->get_buffer_layout());
	m_test_mesh = new mesh(buffer_layout::acquire_layout_of<vertex_pcu>());
	const aabb2 zone{{-375, -660}, {375, 180}};
	mesh_builder::add_aabb2(*m_test_mesh, zone);
	m_test_mesh->update_gpu_mesh(g_renderer);
	m_root = new empty_entity();

	bullet_proto::load_bullet_proto("data/game/bullet_player0.xml");
	bullet_proto::load_bullet_proto("data/game/bullet_TestBullet0.xml");
	
	m_player = new player();
	m_player->start();
}

void game_play::begin_frame()
{
	m_player->begin_frame();
	if (m_test_bullet) {
		m_test_bullet->begin_frame();
	}
	for(auto& each_bullet : m_player_bullets) {
		each_bullet->begin_frame();
	}
}

void game_play::update(float32 delta_sec)
{
	dict args;
	m_player_moving_input = vec2::ZERO;
	if (keyboard::get_key_state(KEY_UP_ARROW)) {
		m_player_moving_input.y += 1;
	}
	if (keyboard::get_key_state(KEY_DOWN_ARROW)) {
		m_player_moving_input.y -= 1;
		
	}
	if (keyboard::get_key_state(KEY_LEFT_ARROW)) {
		m_player_moving_input.x -= 1;	
	}
	if (keyboard::get_key_state(KEY_RIGHT_ARROW)) {
		m_player_moving_input.x += 1;
	}

	if (keyboard::get_key_state('Z')) {
		event::fire("player-shoot", args);
	} else {
		m_player->m_is_shoot_requested = false;
	}
	
	args.set("x-axis", m_player_moving_input.x);
	args.set("y-axis", m_player_moving_input.y);
	event::fire("player-move-right", args);
	event::fire("player-move-up", args);


	//////// ENTITES
	for(auto& each_bullet : m_player_bullets) {
		if (!each_bullet->is_garbage()) {
			each_bullet->update(delta_sec);
		}
		if(fabsf(each_bullet->m_position.x) > 375.f || each_bullet->m_position.y < -660.f || each_bullet->m_position.y > 180.f) {
			each_bullet->stop();
		}
	}
	for(auto& each_bullet : m_enemy_bullets) {
		if (!each_bullet->is_garbage()) {
			each_bullet->update(delta_sec);
		}
		if(fabsf(each_bullet->m_position.x) > 375.f || each_bullet->m_position.y < -660.f || each_bullet->m_position.y > 180.f) {
			each_bullet->stop();
		}
	}

	for(auto& each_enemy : m_enemies) {
		each_enemy->update(delta_sec);
	}
	{
		debug_next_enemy -= delta_sec;
		if(debug_next_enemy <= 0.f) {
			enemy* new_enemy = new enemy(nullptr);
			new_enemy->start();
			m_enemies.push_back(new_enemy);
			debug_next_enemy  = 1.f;
		}
	}

	
	m_player->update(delta_sec);
	if (m_player->is_shooting()) {
		m_player->m_shoot_cool_down = m_player->m_next_cool_down;
		bullet* new_bullet = new bullet(m_player, *bullet_proto::get_bullet_proto("player0"));
		new_bullet->start();
		new_bullet->m_position = m_player->m_position;
		new_bullet->m_velocity = {0, 1};
		m_player_bullets.emplace_back(new_bullet);
	}

	for (auto& each_enemy : m_enemies) {
		for(auto& each_player_bullet : m_player_bullets) {
			if (each_enemy->m_health.is_dead()) {
				each_enemy->m_valid = false;
				break;
			}
			if (!each_player_bullet->m_valid) {
				continue;
			}
			if (each_enemy->m_collider->is_collide_with(each_player_bullet->m_collider)) {
				each_enemy->m_health.damage(1);
				each_player_bullet->m_valid = false;
			}
		}
	}
	for(auto& each_enemy_bullet: m_enemy_bullets) {
		if (each_enemy_bullet->m_valid) {
			if (m_player->m_collider->is_collide_with(each_enemy_bullet->m_collider)) {
				m_player->m_is_respawning = true;
				m_player->m_respawn_cool_down = PLAYER_RESPAWN_TIME;
				each_enemy_bullet->m_valid = false;
				break;
			}
		}
	}
	for (auto i = std::begin(m_player_bullets); i != std::end(m_player_bullets);) {
		if ((*i)->is_garbage()) {
			i = m_player_bullets.erase(i);
			continue;
		}
		++i;
	}

	for (auto i = std::begin(m_enemy_bullets); i != std::end(m_enemy_bullets);) {
		if ((*i)->is_garbage()) {
			i = m_enemy_bullets.erase(i);
			continue;
		}
		++i;
	}

	for (auto i = std::begin(m_enemies); i != std::end(m_enemies);) {
		if ((*i)->is_garbage()) {
			i = m_enemies.erase(i);
			continue;
		}
		++i;
	}
}

void game_play::render() const
{
	static vec2 fraction = vec2{750.f, 840.f} / vec2{1200.f, 900.f};
	
	g_renderer->set_ortho({-375, -660}, {375, 180}, -1, 1);
	g_renderer->set_viewport(fraction * g_renderer->m_resolution, {30, 30});
	
	g_renderer->bind_shader(m_unlit_shader);
	g_renderer->bind_constant_buffer(CONSTANT_CAMERA_BUFFER, g_renderer->m_buffer_project);
	texture2d* bgtex = g_renderer->get_texture2d_by_id("bg_tmp");
	g_renderer->bind_texture(bgtex, TEXTURE_SLOT_DIFFUSE, MIN_LINEAR_MAG_LINEAR);
	g_renderer->draw_mesh(m_test_mesh);

	mesh* tmp_mesh = new mesh(buffer_layout::acquire_layout_of<vertex_pcu>());
	m_player->render(*tmp_mesh);
	for(auto& each_enemy : m_enemies) {
		each_enemy->render(*tmp_mesh);
	}
	tmp_mesh->update_gpu_mesh(g_renderer);
	g_renderer->bind_texture(m_player->m_anim->get_texture2d());
	g_renderer->draw_mesh(tmp_mesh);

	tmp_mesh->clear();
	for(auto& each : m_player_bullets) {
		each->render(*tmp_mesh);
	}
	for(auto& each : m_enemy_bullets) {
		each->render(*tmp_mesh);
	}
	tmp_mesh->update_gpu_mesh(g_renderer);
	g_renderer->bind_texture(g_renderer->get_texture2d_by_id("test44"));
	g_renderer->draw_mesh(tmp_mesh);
	delete tmp_mesh;
	
}

void game_play::end_frame()
{
	m_player->end_frame();
	if (m_test_bullet) {
		m_test_bullet->end_frame();
	}

	for(auto& each_enemy : m_enemies) {
		each_enemy->end_frame();
	}
	
	for(auto& each_bullet : m_player_bullets) {
		each_bullet->end_frame();
	}
	for(auto& each_bullet : m_enemy_bullets) {
		each_bullet->end_frame();
	}
	if (m_test_bullet && m_test_bullet->is_garbage()) {
		delete m_test_bullet;
		m_test_bullet = nullptr;
	}
}

void game_play::stop()
{
	m_player->stop();
	delete m_player;
	delete m_test_bullet;
	delete m_root;
	event::unsubscribe("game-spawn-bullet", {this, &game_play::spawn_bullet});
}

bool game_play::spawn_bullet(glare::dict& args)
{
	bullet_proto* proto = args.get<bullet_proto*>("proto", nullptr);
	entity*		spawner = args.get<entity*>("spawner", m_root);
	bullet* new_bullet = new bullet(spawner, *proto);
	new_bullet->start();
	new_bullet->m_position = spawner->m_position;
	m_enemy_bullets.push_back(new_bullet);
	return true;
}

void game_play::on_keyboard(bool keydown, byte keycode)
{
	if (keycode == KEY_SHIFT) {
		if (keydown) {
			dict args;
			args.set("slow", true);
			event::fire("player-slow", args);
		} else {
			dict args;
			event::fire("player-slow", args);
		}
	}
}
