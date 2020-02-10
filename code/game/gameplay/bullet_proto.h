#pragma once
#include "glare/core/common.h"
#include "framework/common.h"
#include <unordered_map>
class bullet_proto
{
public:
	string m_proto_id;
public:
	static std::unordered_map<string, bullet_proto*> s_bullet_protos;
};