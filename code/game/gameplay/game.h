#pragma once
#include "framework/app.h"
#include "glare/math/vector.h"
#include "gameplay/map.h"
#include <map>
class game_mode;
class game_editor;
class game_play;
//class vec2;

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

	NODISCARD vec2 get_current_player_position() const;

	void on_keyboard(bool keydown, byte keycode);
	void _load_map_defs();

public:
	app*			m_app = nullptr;

	game_mode*		m_current_game_mode;
	//game_editor*	m_game_editor = nullptr;
	//game_play*		m_game_playing = nullptr;
	
	bool			m_transition_to_editor_requested = false;
	bool			m_transition_to_playing_requested = false;
	std::unordered_map<int32, map*>		m_map_defs;
};

extern game* g_current_game;