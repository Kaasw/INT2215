#include "grass.h"
#include <iostream>

using namespace std;

Grass::Grass(int x, int y, int w, int h)
    : Object(x, y, w, h, Type::WALL)
    , grass_spritesheet("assets/texture/Grass.png", 4, 4)
    , m_grass_columns(0)
{
    grass_spritesheet.select_sprite(0, 0);
}


void Grass::draw(SDL_Surface* window_surface)
{
    SDL_Rect grass_position = getRect();
    grass_spritesheet.draw_selected_sprite(window_surface, &grass_position, 2.0f);
}