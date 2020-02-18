#pragma once
#include "framework/common.h"
#include "glare/math/vector.h"
#include <vector>

namespace glare {
class mesh;
}

class entity
{
public:
	entity() = default;
	virtual	~entity()
	{
		
	}
	virtual void start()		=0;
	virtual void begin_frame()	=0;
	virtual void update(float32 delta_sec)		=0;
	virtual void render(mesh& collection) const =0;
	virtual void end_frame()	=0;
	virtual void stop()			=0;

	NODISCARD virtual bool is_garbage() const;
public:
	vec2	m_position;
	vec2	m_velocity;
	vec2	m_accleration;
	float32 m_rotation	= 0.f;
	float32 m_ang_accleration;
	float32 m_scale		= 1.f;

	bool	m_valid = true;
	std::vector<entity*>	m_dependents;
};

class empty_entity : public entity
{
public:
	empty_entity() : entity() { }
	~empty_entity() override  = default;

	void start() override { }
	void begin_frame() override { }
	void update(float32 delta_sec) override { UNUSED(delta_sec); }
	void render(mesh& collection) const override { UNUSED(collection); }
	void end_frame() override { }
	void stop() override { }
};
