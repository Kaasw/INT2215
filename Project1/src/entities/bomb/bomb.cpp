#include "bomb.h"
#include <iostream>

Bomb::Bomb(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::BOMB)            
    , bomb_spritesheet("assets/texture/bomb.png", 4, 4)
    , m_bomb_columns(0)
    , m_destroy_timer(0.0f)
    , isDestroyed(false)
{
    bomb_spritesheet.select_sprite(0, 0);
}

bool Bomb::updateBomb(float delta_time)
{
    m_destroy_timer += delta_time;
    if (m_destroy_timer >= FRAME_DURATION)
    {
        m_destroy_timer -= FRAME_DURATION;
        ++m_bomb_columns;

        if (m_bomb_columns >= FRAME_COUNT)
        {
            m_bomb_columns = 0;
            ++m_loop_count;
            if (m_loop_count >= MAX_LOOPS)
                return true;             // done all loops ? remove bomb
        }

        bomb_spritesheet.select_sprite(0, m_bomb_columns);
    }
    return false;   // keep alive
}


void Bomb::draw(SDL_Surface* surface)
{
    SDL_Rect dst = getRect();
    bomb_spritesheet.draw_selected_sprite(surface, &dst, 2.0f);
}