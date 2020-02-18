#include "gameplay/entity.h"

bool entity::is_garbage() const
{
	if (m_valid) {
		return false;
	}
	for(auto& each : m_dependents) {
		if (!each->is_garbage()) {
			return false;
		}
	}
	return true;
}

