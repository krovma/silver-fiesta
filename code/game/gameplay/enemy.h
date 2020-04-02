#pragma once
#include "gameplay/entity.h"
#include "glare/render/sprite.h"
#include "gameplay/enemy_proto.h"

class enemy : public entity
{
public:
	enemy() = default;
	enemy(enemy_proto* proto);
	~enemy();

	void start() override;
	void begin_frame() override;
	void update(float32 delta_sec) override;
	void render(mesh& collection) const override;
	void end_frame() override;
	void stop() override;

public:
	//bool m_shoot;
	i_sprite_anim_base* m_anim = nullptr;

	bool m_finish[NUM_BULLET_MOVEMENT] {false};
	const movement::stage*	m_current_custom_movement_stage = nullptr;
	//const entity*	m_spawner = nullptr;
	float32 m_current_stage_time = 0.f;
	vec2 m_last_goto_player_velocity {10000000.f, 10000000.f};
	
	enemy_proto*	m_proto = nullptr;
	collider*		m_collider = nullptr;
	health			m_health;
	float m_next_bullet = 0.3f;
};