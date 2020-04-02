#include "gameplay/enemy.h"
#include "glare/core/event.h"
#include "gameplay/bullet_proto.h"
#include "glare/math/aabb2.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/renderer.h"
#include "game.h"

static vec2	PLAYER_EXTEND { 16.f, 24.f };
//static sprite_sheet* g_enemy_sprite_sheet = nullptr;

enemy::enemy(enemy_proto* proto)
	:m_proto(proto)
{
	m_collider = new disk_collider(this, m_proto->m_radius);

	if (m_proto->m_is_static_frame) {
		sprite_sheet* enemy_sheet = sprite_sheet::get_sprite_sheet(m_proto->m_anim_id);
		m_anim = enemy_sheet->create_sprite_copy(0);
	} else {
		//m_anim = sprite_anim
	}
}

enemy::~enemy()
{
	delete m_anim;
	delete m_collider;
}

void enemy::start()
{
	m_current_custom_movement_stage = m_proto->get_movement_stage_by_id(m_proto->m_first_stage);
}

void enemy::begin_frame()
{
}

void enemy::update(float32 delta_sec)
{
	if (m_health.is_dead()) {
		m_valid = false;
		return;
	}
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
	// if (m_current_custom_movement_stage->m_use[MV_TO_SPAWNER] && !m_finish[MV_TO_SPAWNER]) {
	// 	vec2 new_velocity = m_spawner->m_position - m_position;
	// 	new_velocity.set_length(m_current_custom_movement_stage->m_blmv_to_spawner_speed);
	// 	m_position += new_velocity * delta_sec;
	// 	if (m_current_stage_time >= m_current_custom_movement_stage->m_blmv_to_spawner_time) {
	// 		m_finish[MV_TO_SPAWNER] = true;
	// 	}
	// }
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

void enemy::render(mesh& collection) const
{
	aabb2 box;
	box.min = m_position - PLAYER_EXTEND;
	box.max = m_position + PLAYER_EXTEND;
	vec2 uvs[4];
	m_anim->get_current_frame_uvs(uvs);
	mesh_builder::add_aabb2(collection, box, uvs);
}

void enemy::end_frame()
{
	if (m_position.x > 395 || m_position.x < - 395) {
		m_valid = false;
	}

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

void enemy::stop()
{
	m_valid = false;
}
