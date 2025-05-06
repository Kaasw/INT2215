#include "brick.h"
#include <iostream>

using namespace std;

Brick::Brick(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::BRICK)
    , brick_spritesheet("assets/texture/brick.png", 4, 4)
    , m_brick_columns(0)
{
    brick_spritesheet.select_sprite(0, 0);
}


bool Brick::updateBrick(float delta_time)
{
    if (!isDestroyed)
        return false;

    m_destroy_timer += delta_time;
    if (m_destroy_timer >= FRAME_DURATION && m_brick_columns < DESTROY_FRAMES)
    {
        m_destroy_timer -= FRAME_DURATION;
        ++m_brick_columns;  // 1?2?3
        brick_spritesheet.select_sprite(0, m_brick_columns);
    }

    return (m_brick_columns >= DESTROY_FRAMES);
}

void Brick::draw(SDL_Surface* surface)
{
    //if (m_brick_columns <= DESTROY_FRAMES)
    //{
        SDL_Rect dst = getRect();
        brick_spritesheet.draw_selected_sprite(surface, &dst, 2.0f);
    //}
}
