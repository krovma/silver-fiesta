#pragma once
#include "glare/core/common.h"
#include "framework/common.h"

namespace glare{
	class window;
	class renderer;
};

class game;
class app
{
public:
	app();
	void start();
	void run_frame();
	void stop();

	void event_close();
	void event_keyboard(bool keydown, byte keycode) const;

	void _load_resources() const;
public:
	bool m_run = true;
	window* m_window = nullptr;
	renderer* m_renderer = nullptr;

	game*	m_game = nullptr;
};

