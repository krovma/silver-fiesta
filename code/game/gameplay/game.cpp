#include "gameplay/game.h"
#include "gameplay/game_editor.h"
#include "gameplay/game_play.h"
#include "glare/core/event.h"
#include "glare/core/log.h"
#include "glare/core/input.h"

game::game(app* app)
	:m_app(app)
{
	m_current_game_mode = new game_play(app);
}

game::~game()
{
	delete m_current_game_mode;
	//delete m_game_editor;
	//delete m_game_playing;
}

void game::start()
{
	m_current_game_mode->start();
	//m_game_editor = new game_editor(m_app);
	//m_game_editor->start();
}

void game::begin_frame()
{
	m_current_game_mode->begin_frame();
}

void game::update(float32 delta_sec)
{
	if (m_transition_to_playing_requested) {
		m_current_game_mode->stop();
		delete m_current_game_mode;
		m_current_game_mode = new game_play(m_app);
		m_current_game_mode->start();
		m_transition_to_playing_requested = false;
		return;
	}
	if (m_transition_to_editor_requested) {
		m_current_game_mode->stop();
		delete m_current_game_mode;
		m_current_game_mode = new game_editor(m_app);
		m_current_game_mode->start();
		m_transition_to_editor_requested = false;
		return;
	}
	
	m_current_game_mode->update(delta_sec);
	// dict empty;
	// event::fire("test", empty);
	// dict with_y;
	// with_y.set("y", 56.f);
	// event::fire("test2", with_y);
}

void game::render() const
{
	m_current_game_mode->render();
}

void game::end_frame()
{
	m_current_game_mode->end_frame();
}

void game::stop()
{
	m_current_game_mode->stop();
}

void game::on_keyboard(bool keydown, byte keycode)
{
	m_current_game_mode->on_keyboard(keydown, keycode);
	if (keydown && keycode == KEY_F1) {
		m_transition_to_playing_requested = true;
		m_transition_to_editor_requested = false;
	}
	if (keydown && keycode == KEY_F2) {
		m_transition_to_playing_requested = false;
		m_transition_to_editor_requested = true;
	}
}
