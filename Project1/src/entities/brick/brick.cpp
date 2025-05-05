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


void Brick::draw(SDL_Surface* window_surface)
{
    SDL_Rect brick_position = getRect();

    brick_spritesheet.draw_selected_sprite(window_surface, &brick_position, 2.0f);
}


