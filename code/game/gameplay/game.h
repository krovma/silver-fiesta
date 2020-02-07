#pragma once
#include "glare/core/common.h"
#include "framework/common.h"

namespace glare
{
	class shader;

};

class app;
class game
{
public:
	game(app* app);
	~game();
	void start();
	void begin_frame();
	void update();
	void render() const;
	void end_frame();
	void stop();
public:
	app*	m_app = nullptr;
	shader* m_unlit_shader = nullptr;
};
