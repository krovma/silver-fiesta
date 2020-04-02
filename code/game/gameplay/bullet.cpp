#include "gameplay/bullet.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/renderer.h"
#include "gameplay/collider.h"
#include "gameplay/game.h"
//extern renderer* g_renderer;

bullet::bullet(entity* spawner, const bullet_proto& proto)
	: entity()
	, m_proto(&proto)
	, m_spawner(spawner)
{
	auto sprites = sprite_sheet::get_sprite_sheet("test4x4");
	m_anim = sprites->create_sprite_copy(0);
	spawner->m_dependents.push_back(this);
	m_collider = new disk_collider(this, 10.f);
}

void bullet::start()
{
	m_velocity = {0, -1};
	//m_position	= m_proto->m_start_position;
	//m_rotation	= m_proto->m_start_rotation;
	m_velocity.rotate(dtr(m_rotation));
	m_current_custom_movement_stage = m_proto->get_movement_stage_by_id(m_proto->m_start_next_stage);
}

void bullet::begin_frame()
{
}

void bullet::update(float32 delta_sec)
{
	if (!m_valid) {
		return;
	}
	const vec2 old_position = m_position;
	m_current_stage_time += delta_sec;
	if (m_current_custom_movement_stage->m_use[MV_FORWARD] && !m_finish[MV_FORWARD]) {
		vec2 velocity = m_velocity;
		velocity.set_length(m_current_custom_movement_stage->m_blmv_forward_speed);
		m_position += velocity * delta_sec;
		if (m_current_stage_time >= m_current_custom_movement_stage->m_blmv_forward_time) {
			m_finish[MV_FORWARD] = true;
		}
	}
	if (m_current_custom_movement_stage->m_use[MV_TO_PLAYER] && !m_finish[MV_TO_PLAYER]) {
		if (m_last_goto_player_velocity.x > 10000.f) {
			vec2 new_velocity = g_current_game->get_current_player_position() - m_position;
			new_velocity.set_length(m_current_custom_movement_stage->m_blmv_to_player_speed);
			m_last_goto_player_velocity = new_velocity;
		}
		m_position += m_last_goto_player_velocity * delta_sec;
		if (m_current_stage_time >= m_current_custom_movement_stage->m_blmv_to_player_time) {
			m_finish[MV_TO_PLAYER] = true;
		}
	}
	if (m_current_custom_movement_stage->m_use[MV_TO_SPAWNER] && !m_finish[MV_TO_SPAWNER]) {
		vec2 new_velocity = m_spawner->m_position - m_position;
		new_velocity.set_length(m_current_custom_movement_stage->m_blmv_to_spawner_speed);
		m_position += new_velocity * delta_sec;
		if (m_current_stage_time >= m_current_custom_movement_stage->m_blmv_to_spawner_time) {
			m_finish[MV_TO_SPAWNER] = true;
		}
	}
	if (m_current_custom_movement_stage->m_use[MV_MOVEBY] && !m_finish[MV_MOVEBY]) {
		m_position +=  m_current_custom_movement_stage->m_blmv_moveby_direction * delta_sec;
		if (m_current_stage_time >= m_current_custom_movement_stage->m_blmv_moveby_time) {
			m_finish[MV_MOVEBY] = true;
		}
	}
	if (m_current_custom_movement_stage->m_use[MV_SPAWN] && !m_finish[MV_SPAWN]) {
		const vec2 spawn_pos = m_position + m_current_custom_movement_stage->m_blmv_spawn_offset;
		dict args;
		bullet_proto* proto = bullet_proto::get_bullet_proto(m_current_custom_movement_stage->m_blmv_spawn_id);
		args.set("proto", proto);
		args.set("spawner", static_cast<entity*>(this));
		args.set("position", spawn_pos);
		args.set("rotation", m_current_custom_movement_stage->m_blmv_spawn_rotation);
		event::fire("game-spawn-bullet", args);
		m_finish[MV_SPAWN] = true;
	}
	m_velocity = (m_position - old_position) / delta_sec;
	m_anim->advance_time(delta_sec);
}

void bullet::render(mesh& collection) const
{
	if (m_valid) {
		vec2 uvs[4];
		m_anim->get_current_frame_uvs(uvs[0], uvs[1], uvs[2], uvs[3]);
		mesh_builder::add_disk(collection, m_position, 10.f, 16, {uvs[2], uvs[1]});
	}
}

void bullet::end_frame()
{
	if (m_valid) {
		try_transition();
	}
}

void bullet::stop()
{
	if (m_proto->m_end_effect) {
		// Spawn some effect;
	}
	m_valid = false;
}


void bullet::try_transition()
{
	for(int32 i = 0; i < NUM_BULLET_MOVEMENT; ++i) {
		if (m_current_custom_movement_stage->m_use[i] && !m_finish[i]) {
			return;
		}
	}
	m_current_custom_movement_stage = m_proto->get_movement_stage_by_id(m_current_custom_movement_stage->m_next_movement_stage);
	m_last_goto_player_velocity = {10000000.f, 10000000.f};
	if (!m_current_custom_movement_stage) {
		stop();
	}
	for (int32 i = 0; i < NUM_BULLET_MOVEMENT; ++i) {
		m_finish[i] = false;
	}
	m_current_stage_time = 0.f;
}
