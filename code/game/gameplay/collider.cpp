#include "gameplay/collider.h"
#include "gameplay/entity.h"
#include "glare/math/utilities.h"

static bool disk_disk_collide(disk_collider* a, disk_collider* b)
{
	const vec2 disp = a->m_entity->m_position - b->m_entity->m_position;
	return disp.length() <= a->m_radius + b->m_radius;
}

bool collider::is_collide_with(collider* other)
{
	if (this->m_type == COLLIDER_DISK && other->m_type == COLLIDER_DISK) {
		return disk_disk_collide(static_cast<disk_collider*>(this), static_cast<disk_collider*>(other));
	}
	return false;
}
