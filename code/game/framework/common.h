#pragma once
#include "glare/core/common.h"

namespace glare
{
	class rng;
	class renderer;
}
using namespace glare;

constexpr const char* APP_NAME = "Silver Fiesta";
extern rng g_rng;
extern renderer* g_renderer;