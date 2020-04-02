#include "framework/app.h"
#include "gameplay/game.h"
#include "glare/core/window.h"
#include "glare/render/renderer.h"
#include "glare/core/color.h"
#include "glare/core/input.h"
#include "glare/dev/dev_ui.h"
#include "glare/data/xml_utils.h"
#include "glare/render/sprite.h"
#include "glare/core/clock.h"

renderer* g_renderer = nullptr;

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

	_load_resources();
	
	m_game = new game(this);
	m_game->start();
}

void app::run_frame()
{
	m_window->begin_frame();
	m_renderer->begin_frame();
	m_renderer->clear_render_target(color::BLACK);
	m_game->begin_frame();
	dev_ui::begin_frame();

	const float32 dt = g_master_clock->get_frame_time();
	
	m_game->update(dt);
	dev_ui::update(dt);
	m_game->render();
	dev_ui::render();
	m_game->end_frame();
	m_renderer->end_frame();
	dev_ui::end_frame();
	m_window->end_frame();
}

void app::stop()
{
	sprite_sheet::clear_cache();
	
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

void app::event_keyboard(bool keydown, byte keycode) const
{
	// some thing like slow mode ...
	keyboard::set_key_state(keycode, keydown);
	m_game->on_keyboard(keydown, keycode);
}

void app::_load_resources() const
{
	xml::document* doc = xml::load_file("data/resources.xml");
	const xml::node root = doc->child("resources");

	// load texture
	const xml::node texture = root.child("texture");
	for (auto& each: texture.children("load")) {
		const string id = xml::get_attr(each, "id", "default");
		const string src = xml::get_attr(each, "src", "");
		m_renderer->load_texture2d_from_file(id, src.c_str());
	}

	// load sprites
	const xml::node sprite = root.child("sprite");
	for (auto& each: sprite.children("load")) {
		const string id = xml::get_attr(each, "id", "default");
		const ivec2 layout = xml::get_attr(each, "layout", ivec2::ZERO);
		if (layout.x > 0 && layout.y > 0) {
			const string texture_id = xml::get_attr(each, "texture", "default");
			texture2d* const ptexture = m_renderer->get_texture2d_by_id(texture_id);
			sprite_sheet::load_sprite_sheet_from_texture(id, layout, ptexture);
		} else {
			const string src = xml::get_attr(each, "src", "");
			sprite_sheet::load_sprite_sheet_from_xml(id, m_renderer, src.c_str());
		}
	}
	// load animations
	const xml::node anim = root.child("anim");
	for (auto& each: anim.children("load")) {
		const string id = xml::get_attr(each, "id", "no-anim-id");
		const string src = xml::get_attr(each, "src", "");
		sprite_anim::load_sprite_anim_from_xml(id, src.c_str());
	}
	// load audio
	
}
