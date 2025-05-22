#include "oneal.h"
#include "src/entities/brick/brick.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

int currentOnealSprite = 0;
Oneal::Oneal(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::ONEAL)
    , m_oneal_columns(0)
    , oneal_spritesheet("assets/texture/oneal.png", 4, 4)
{
    oneal_spritesheet.select_sprite(0, 0);
}

void Oneal::update(float delta_time,
    std::list<Object*>& collidables,
    std::list<Bomb*>& m_bombs,
    const SDL_Rect& playerRect)
{
    SDL_Rect oneal = getRect();
    float ox = float(oneal.x), oy = float(oneal.y);
    float px = float(playerRect.x), py = float(playerRect.y);
    float dx = px - ox, dy = py - oy;
    if ((dx * dx + dy * dy) <= SIGHT_RADIUS * SIGHT_RADIUS) {

        if (std::fabs(ox - px) <= ALIGN_THRESH) {
            direction = (dy > 0 ? OnealDirection::DOWN : OnealDirection::UP);
        }
      
        else if (std::fabs(oy - py) <= ALIGN_THRESH) {
            direction = (dx > 0 ? OnealDirection::RIGHT : OnealDirection::LEFT);
        }
      

    }
    float vx = 0.0f, vy = 0.0f;
    const float speed = 5.0f;
    switch (direction) {
    case OnealDirection::LEFT:  
        vx = -1.0f; 
		currentOnealSprite = 0;
        break;
    case OnealDirection::RIGHT: 
        vx = +1.0f;
		currentOnealSprite = 1;
        break;
    case OnealDirection::UP:    
		currentOnealSprite = 0;
        vy = -1.0f; break;
    case OnealDirection::DOWN:  
        vy = +1.0f;
		currentOnealSprite = 1;
        break;
    }

    moveX(vx * speed * delta_time, collidables);
	moveY(vy * speed * delta_time, collidables);


    timer += delta_time;
    if (timer >= FRAME_DURATION)
    {
        timer -= FRAME_DURATION;
        ++m_oneal_columns;

        if (m_oneal_columns >= FRAME_COUNT)
        {
            m_oneal_columns = 0;

        }

		oneal_spritesheet.select_sprite(currentOnealSprite, m_oneal_columns);
    }

}

void Oneal::draw(SDL_Surface* window_surface)
{
    SDL_Rect dst = getRect();
    oneal_spritesheet.draw_selected_sprite(window_surface, &dst, 2.0f);
}


void Oneal::moveX(float dx, std::list<Object*>& collidables)
{

    m_x += dx;
    SDL_Rect rect = getRect();
    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&rect, &other))
        {        
            if (direction == OnealDirection::LEFT) {
				direction = OnealDirection::RIGHT;
			}
			else if (direction == OnealDirection::RIGHT) {
				direction = OnealDirection::LEFT;
			}

            if (dx > 0)
                m_x = other.x - rect.w;
            else
                m_x = other.x + other.w;
        }
    }
}

void Oneal::moveY(float dy, std::list<Object*>& collidables)
{
    m_y += dy;
    SDL_Rect me = getRect();

    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&me, &other))
        {
            if (direction == OnealDirection::UP) {
                direction = OnealDirection::DOWN;
            }
            else if (direction == OnealDirection::DOWN) {
                direction = OnealDirection::UP;
            }
  
            if (dy > 0)
                m_y = other.y - me.h;
            else
                m_y = other.y + other.h;
            break;
        }
    }
}





