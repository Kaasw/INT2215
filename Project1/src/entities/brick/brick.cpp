#include "brick.h"
#include <iostream>

using namespace std;

Brick::Brick(int x, int y, int w, int h)
    : Object(x, y, w, h, Type::WALL)
    , brick_spritesheet("assets/texture/wall.png", 4, 4)
    , m_brick_columns(0)
{
    brick_spritesheet.select_sprite(0, 0);
}

void Brick::update(float deltaTime)
{
    // Update logic if any
}

void Brick::draw(SDL_Surface* window_surface)
{
    SDL_Rect brick_position = getRect();

    // Fill the background with a solid color before drawing the sprite
    SDL_FillRect(window_surface, &brick_position, SDL_MapRGB(window_surface->format, 255, 255, 255));

    // Draw the sprite without transparency
    brick_spritesheet.draw_selected_sprite(window_surface, &brick_position, 4.0f);
}