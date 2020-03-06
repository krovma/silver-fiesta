#pragma once
#include "glare/core/common.h"

class player;

class game_mode
{
public:
	virtual ~game_mode() = default;
	NODISCARD virtual player* get_player() const = 0;

	virtual void start() = 0;
	virtual void begin_frame() = 0;
	virtual void update(float32 delta_sec) = 0;
	virtual void render() const = 0;
	virtual void end_frame() = 0;
	virtual void stop() = 0;

	virtual void on_keyboard(bool keydown, byte keycode)
	{
	}
};