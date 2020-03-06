#include "gameplay/bullet.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/renderer.h"
#include "gameplay/collider.h"
//extern renderer* g_renderer;

bullet::bullet(const entity* spawner, const bullet_proto& proto)
	: entity()
	, m_proto(&proto)
	, m_spawner(spawner)
{
	auto sprites = sprite_sheet::get_sprite_sheet("test4x4");
	m_anim = sprites->create_sprite_copy(8);

	m_collider = new disk_collider(this, 10.f);
}

void bullet::start()
{
	m_velocity = {0, -1};
	m_position	= m_proto->m_start_position;
	m_rotation	= m_proto->m_start_rotation;
	m_current_custom_stage = m_proto->get_custom_stage(m_proto->m_start_next_stage);
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
	if (m_current_custom_stage->m_use[BLMV_FORWARD] && !m_finish[BLMV_FORWARD]) {
		vec2 velocity = m_velocity;
		velocity.set_length(m_current_custom_stage->m_blmv_forward_speed);
		m_position += velocity * delta_sec;
		if (m_current_stage_time >= m_current_custom_stage->m_blmv_forward_time) {
			m_finish[BLMV_FORWARD] = true;
		}
	}
	if (m_current_custom_stage->m_use[BLMV_TO_PLAYER] && !m_finish[BLMV_TO_SPAWNER]) {
		// redirect to player
	}
	if (m_current_custom_stage->m_use[BLMV_TO_SPAWNER] && !m_finish[BLMV_TO_SPAWNER]) {
		vec2 new_velocity = m_spawner->m_position - m_position;
		new_velocity.set_length(m_current_custom_stage->m_blmv_to_spawner_speed);
		m_position += new_velocity * delta_sec;
		if (m_current_stage_time >= m_current_custom_stage->m_blmv_to_spawner_time) {
			m_finish[BLMV_TO_SPAWNER] = true;
		}
	}
	m_velocity = (m_position - old_position) / delta_sec;
	m_anim->advance_time(delta_sec);
}

void bullet::render(mesh& collection) const
{
	vec2 uvs[4];
	m_anim->get_current_frame_uvs(uvs[0], uvs[1], uvs[2], uvs[3]);
	mesh_builder::add_disk(collection, m_position, 10.f, 16, {uvs[2], uvs[1]});
}

void bullet::end_frame()
{
	try_transition();
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
		if (m_current_custom_stage->m_use[i] && !m_finish[i]) {
			return;
		}
	}
	m_current_custom_stage = m_proto->get_custom_stage(m_current_custom_stage->m_next_stage);
	if (!m_current_custom_stage) {
		stop();
	}
	for (int32 i = 0; i < NUM_BULLET_MOVEMENT; ++i) {
		m_finish[i] = false;
	}
	m_current_stage_time = 0.f;
}
