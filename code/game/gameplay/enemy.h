#pragma once
#include "gameplay/entity.h"
#include "glare/render/sprite.h"

class enemy_proto;

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
	enemy_proto*	m_proto = nullptr;
	collider*		m_collider = nullptr;
	health			m_health;
	float m_next_bullet = 0.3f;
};