#include "wall.h"
#include <iostream>

using namespace std;

Wall::Wall(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::WALL)
    , wall_spritesheet("assets/texture/Grass.png", 4, 4)
    , m_wall_columns(0)
{
    wall_spritesheet.select_sprite(0, 0);
}


void Wall::draw(SDL_Surface* window_surface)
{
    SDL_Rect wall_position = getRect();
    wall_spritesheet.draw_selected_sprite(window_surface, &wall_position, 2.0f);
}