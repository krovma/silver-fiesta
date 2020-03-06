#pragma once
#include "glare/render/sprite.h"
#include "glare/core/event.h"
#include "gameplay/entity.h"

constexpr float32 PLAYER_DEFAULT_SPEED = 300.f;
constexpr float32 PLAYER_SLOW_SPEED = 100.f;
constexpr float32 PLAYER_RESPAWN_TIME = 2.f;

class player : public entity
{
public:
	player();
	~player();
	void start() override;
	void begin_frame() override;
	void update(float32 delta_sec) override;
	void render(mesh& collection) const override;
	void end_frame() override;
	void stop() override;

	
	
	NODISCARD bool is_shooting() const
	{
		return m_is_shoot_requested && m_shoot_cool_down <= 0.f;
	}

	NODISCARD bool is_slow_mode() const
	{
		return m_slow_mode;
	}

	EVENT_PROC(shoot)
	{
		if (m_is_respawning) {
			return true;
		}
		m_is_shoot_requested = true;
		m_next_cool_down = args.get("cooldown", 0.05f);
		return true;
	}

	EVENT_PROC(move_right)
	{
		if (m_is_respawning) {
			return true;
		}
		m_move_input.x += args.get("x-axis", 0.f);
		if (m_move_input.length() != 0.f) {
			m_move_input.normalize();
		}
		return true;
	}

	EVENT_PROC(move_up)
	{
		if (m_is_respawning) {
			return true;
		}
		m_move_input.y += args.get("y-axis", 0.f);
		if (m_move_input.length() != 0.f) {
			m_move_input.normalize();
		}
		return true;
	}

	EVENT_PROC(slow)
	{
		if (m_is_respawning) {
			return true;
		}
		m_slow_mode = args.get("slow", false);
		return true;
	}
public:
	i_sprite_anim_base*	m_anim = nullptr;
	collider*			m_collider = nullptr;
	float32	m_shoot_cool_down = 0.f;
	float32 m_next_cool_down = 0.f;
	float32 m_respawn_cool_down = 0.f;
	bool	m_is_shoot_requested = false;
	vec2	m_move_input;
	bool	m_slow_mode = false;
	bool	m_is_respawning = false;

	float32	m_speed = PLAYER_DEFAULT_SPEED;
};