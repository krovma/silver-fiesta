#include "gameplay/player.h"
#include "glare/render/mesh.h"
#include "glare/render/renderer.h"
#include "glare/render/mesh_builder.h"
#include "glare/core/log.h"

static vec2	PLAYER_EXTEND { 16.f, 24.f };
//static sprite_sheet* g_player_sprite_sheet = nullptr;

player::player()
{
	m_position = {0, -300};
	//delete g_player_sprite_sheet;
	//g_player_sprite_sheet = new sprite_sheet(g_renderer->load_texture2d_from_file("player_texture", "data/texture/d_marisa_tmp.png"), {8, 3});
	m_anim = sprite_anim::create_sprite_anim_copy_by_id("marisa_test");
	m_collider = new disk_collider(this, 12.f);
}

player::~player()
{
	delete m_anim;
}

void player::start()
{
	event::subscribe("player-shoot", {this, &player::shoot});
	event::subscribe("player-move-right", {this, &player::move_right});
	event::subscribe("player-move-up", {this, &player::move_up});
	event::subscribe("player-slow", {this, &player::slow});
	m_anim->set_current_clip("idle");
}

void player::begin_frame()
{
	m_move_input = vec2::ZERO;
	m_speed = is_slow_mode() ? PLAYER_SLOW_SPEED : PLAYER_DEFAULT_SPEED;
}

void player::update(float32 delta_sec)
{
	if (m_is_respawning) {
		m_respawn_cool_down -= delta_sec;
		if (m_respawn_cool_down <= 0.f) {
			m_position = {0, -300};
			m_is_respawning = false;
		}
		return;
	}
	
	m_rotation = 0.f;
	m_velocity = m_move_input * m_speed;
	m_position += m_velocity * delta_sec;
	m_anim->advance_time(delta_sec);
	m_shoot_cool_down -= delta_sec;

	if (m_position.x < -375.f + PLAYER_EXTEND.x) {
		m_position.x = -375.f + PLAYER_EXTEND.x;
	}
	if (m_position.x > 375.f - PLAYER_EXTEND.x) {
		m_position.x = 375.f - PLAYER_EXTEND.x;
	}
	if (m_position.y < -660.f + PLAYER_EXTEND.y) {
		m_position.y = -660.f + PLAYER_EXTEND.y;
	}
	if (m_position.y > 180.f - PLAYER_EXTEND.y) {
		m_position.y = 180.f - PLAYER_EXTEND.y;
	}
}

void player::render(mesh& collection) const
{
	if(m_is_respawning) {
		return;
	}
	aabb2 box;
	box.min = m_position - PLAYER_EXTEND;
	box.max = m_position + PLAYER_EXTEND;
	vec2 uvs[4];
	m_anim->get_current_frame_uvs(uvs);
	mesh_builder::add_aabb2(collection, box, uvs);
}

void player::end_frame()
{
}

void player::stop()
{
	event::unsubscribe("player-shoot", {this, &player::shoot});
	event::unsubscribe("player-move-right", {this, &player::move_right});
	event::unsubscribe("player-move-up", {this, &player::move_up});
	event::unsubscribe("player-slow", {this, &player::slow});
}
