#include "Baloon.h"
#include "src/entities/brick/brick.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

int currentSprite = 0;


Baloon::Baloon(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::BALOON)
    , m_baloon_columns(0)
    , baloon_spritesheet("assets/texture/baloon.png", 4, 4)
{
	baloon_spritesheet.select_sprite(0, 0);
}

void Baloon::update(float delta_time,
    std::list<Object*>& collidables,
    std::list<Bomb*>& m_bombs)
{
    float vx = 0.0f, vy = 0.0f;
    const float speed = 5.0f;
	if (direction) {
		vx += 1.0f;
		currentSprite = 1;
	}
	else {
		vx -= 1.0f;
		currentSprite = 0;
	}
    moveX(vx * speed * delta_time, collidables);
	

    timer += delta_time;
    if (timer >= FRAME_DURATION)
    {
        timer -= FRAME_DURATION;
        ++m_baloon_columns;

        if (m_baloon_columns >= FRAME_COUNT)
        {
            m_baloon_columns = 0;

        }

        baloon_spritesheet.select_sprite(currentSprite, m_baloon_columns);
    }

}

void Baloon::draw(SDL_Surface* window_surface)
{
    SDL_Rect dst = getRect();
    baloon_spritesheet.draw_selected_sprite(window_surface, &dst, 2.0f);
}


void Baloon::moveX(float dx, std::list<Object*>& collidables)
{
    m_x += dx;
    SDL_Rect rect = getRect();
    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&rect, &other))
        {
			direction = !direction;

            if (dx > 0)
                m_x = other.x - rect.w;
            else
                m_x = other.x + other.w;
        }
    }
}




