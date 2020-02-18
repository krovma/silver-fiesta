#include "gameplay/bullet_proto.h"

STATIC std::unordered_map<string, bullet_proto*> bullet_proto::s_bullet_protos;

const bullet_proto::custom* bullet_proto::get_custom_stage(const string& id) const
{
	for(auto& each : m_custom_stages) {
		if (id == each.m_custom_id) {
			return &each;
		}
	}
	return nullptr;
}
