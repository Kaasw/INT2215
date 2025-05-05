#include "Bomber.h"
#include "src/entities/brick/brick.h"
#include <SDL_image.h>
#include <iostream>
#include <algorithm> // For std::remove

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

void Bomber::update(float delta_time, std::list<Object*>& collidables)
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


void Bomber::moveX(float dx, std::list<Object*>& collidables)
{
	m_x += dx;
	SDL_Rect rect = getRect();
	for (auto* obj : collidables)
	{
		if (obj == this) continue;
		SDL_Rect other = obj->getRect();
		if (SDL_HasIntersection(&rect, &other))
		{
			std::cout << "Collision detected X!" << std::endl;
			if (dx > 0)
				m_x = other.x - rect.w;
			else
				m_x = other.x + other.w;
		}
	}
}

void Bomber::moveY(float dy, std::list<Object*>& collidables)
{
    m_y += dy;
    SDL_Rect me = getRect();

    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();

        if (SDL_HasIntersection(&me, &other))
        {
            // if it's a brick, mark it for destruction
            if (obj->getType() == Type::BRICK)
            {
                std::cout << "Brick hit – marking for removal\n";
                dynamic_cast<Brick*>(obj)->setDestroy();
            }

            // push bomber back outside
            if (dy > 0)    m_y = other.y - me.h;
            else           m_y = other.y + other.h;

            break;  // resolve only one collision per axis
        }
    }
}
