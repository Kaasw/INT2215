#include "Bomber.h"
#include <SDL_image.h>
#include <iostream>

int const SPRITESHEET_UP = 0;
int const SPRITESHEET_LEFT = 3;
int const SPRITESHEET_RIGHT = 1;
int const SPRITESHEET_DOWN = 2;

Bomber::Bomber(float x, float y, int w, int h)
	:Object(x, y, w, h, Type::PLAYER)
	, m_direction(Direction::NONE)
	, m_spritesheet_column(0)
	, m_spritesheet("assets/texture/Player.png", 4, 4)
{
}

void Bomber::update(float delta_time, std::vector<Object*>& collidables)
{
    float vx = 0.0f, vy = 0.0f;
    const float speed = 700.0f;

    switch (m_direction)
    {
    case Direction::NONE:
        m_spritesheet.select_sprite(1, 0);
        break;
    case Direction::UP:
        vy = -1.0f;
        m_spritesheet.select_sprite(SPRITESHEET_UP, m_spritesheet_column);
        break;
    case Direction::DOWN:
        vy = +1.0f;
        m_spritesheet.select_sprite(SPRITESHEET_DOWN, m_spritesheet_column);
        break;
    case Direction::LEFT:
        vx = -1.0f;
        m_spritesheet.select_sprite(SPRITESHEET_LEFT, m_spritesheet_column);
        break;
    case Direction::RIGHT:
        vx = +1.0f;
        m_spritesheet.select_sprite(SPRITESHEET_RIGHT, m_spritesheet_column);
        break;
    }

    moveX(vx * speed * delta_time, collidables);
    moveY(vy * speed * delta_time, collidables);
    m_spritesheet_column++;
    if (m_spritesheet_column > 2)
        m_spritesheet_column = 0;
}


void Bomber::draw(SDL_Surface* window_surface)
{
    SDL_Rect m_position = getRect();
	m_spritesheet.draw_selected_sprite(window_surface, &m_position, 2.0f);
	SDL_Delay(100);
}

void Bomber::handleInput(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		Uint8 const* keys = SDL_GetKeyboardState(nullptr);

		if (keys[SDL_SCANCODE_W] == 1)
			m_direction = Direction::UP;
		else if (keys[SDL_SCANCODE_S] == 1)
			m_direction = Direction::DOWN;
		else if (keys[SDL_SCANCODE_A] == 1)
			m_direction = Direction::LEFT;
		else if (keys[SDL_SCANCODE_D] == 1)
			m_direction = Direction::RIGHT;
		break;
	}
}