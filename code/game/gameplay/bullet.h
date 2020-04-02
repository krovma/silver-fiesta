#pragma once
#include "framework/common.h"
#include "gameplay/entity.h"
#include "gameplay/bullet_proto.h"
#include "glare/render/sprite.h"

class collider;

class bullet : public entity
{
public:
	~bullet() override
	{
		delete m_anim;
		delete m_collider;
	}
	bullet(entity* spawner, const bullet_proto& proto);

	void start() override ;
	void begin_frame() override;
	void update(float32 delta_sec) override;
	void render(mesh& collection) const override;
	void end_frame() override;
	void stop() override;

	void try_transition();
public:
	i_sprite_anim_base*	m_anim = nullptr;
public:
	bool m_finish[NUM_BULLET_MOVEMENT] {false};
	const bullet_proto*	m_proto = nullptr;
	const movement::stage*	m_current_custom_movement_stage = nullptr;
	const entity*	m_spawner = nullptr;
	float32 m_current_stage_time = 0.f;
	vec2 m_last_goto_player_velocity {10000000.f, 10000000.f};
	collider* m_collider = nullptr;
};