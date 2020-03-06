#pragma once
#include "framework/app.h"

class game_mode;
class game_editor;
class game_play;

class game
{
public:
	game(app* app);
	~game();
	void start();
	void begin_frame();
	void update(float32 delta_sec);
	void render() const;
	void end_frame();
	void stop();

	void on_keyboard(bool keydown, byte keycode);

public:
	app*			m_app = nullptr;

	game_mode*		m_current_game_mode;
	//game_editor*	m_game_editor = nullptr;
	//game_play*		m_game_playing = nullptr;
	
	bool			m_transition_to_editor_requested = false;
	bool			m_transition_to_playing_requested = false;
};