#pragma once
#include <SDL.h>

class Bomber {
public:
	enum class Direction {
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Bomber();
	~Bomber() = default;
	void update(double delta_time);
	void draw(SDL_Surface* window_surface);
	void handleInput(SDL_Event& event);

private:
private:
	Direction	m_direction;
	SDL_Surface* m_image;
	SDL_Rect     m_position;
	double       m_x;
	double       m_y;

};
