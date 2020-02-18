#include "framework/app.h"
#include "gameplay/game.h"
#include "glare/core/window.h"
#include "glare/render/renderer.h"
#include "glare/core/color.h"
#include "glare/dev/dev_ui.h"

app::app()
{
}

void app::start()
{
	m_window = new window();
	m_window->create(APP_NAME, 1.333f, 0.9f, DEFAULT_WINDOW_EVENT_PROC);
	m_renderer = new renderer(m_window);
	m_renderer->start();
	dev_ui::start(m_window, m_renderer);
	m_game = new game(this);
	m_game->start();
}

void app::run_frame()
{
	m_window->begin_frame();
	m_renderer->begin_frame();
	m_renderer->clear_render_target(color::BLACK);
	dev_ui::begin_frame();
	m_game->update(0.016f);
	dev_ui::update(0.016f);
	m_game->render();
	dev_ui::render();
	m_game->end_frame();
	m_renderer->end_frame();
	dev_ui::end_frame();
	m_window->end_frame();
}

void app::stop()
{
	m_game->stop();
	delete m_game;
	m_renderer->stop();
	delete m_renderer;
	m_window->destroy();
	delete m_window;
}

void app::event_close()
{
	m_run = false;
}
