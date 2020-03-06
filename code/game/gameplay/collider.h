#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include "glare/math/obb2.h"

enum e_collider_t
{
	COLLIDER_OBB,
	COLLIDER_DISK
};

class entity;

class collider
{
public:
	collider(entity* entity, e_collider_t type)
		:m_type(type), m_entity(entity)
	{}
	virtual ~collider() = default;
	bool is_collide_with(collider* other);
public:
	e_collider_t m_type;
	entity* m_entity = nullptr;
};

class disk_collider : public collider
{
public:
	disk_collider(entity* entity, float32 radius)
		: collider(entity, COLLIDER_DISK)
		, m_radius(radius)
	{}
public:
	float32 m_radius = 1.f;
};