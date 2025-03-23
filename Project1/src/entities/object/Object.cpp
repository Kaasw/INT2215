#include "Object.h"

Object::Object(int x, int y, int width, int height, Type type)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_type(type)

{
}

Object::~Object()
{

}

SDL_Rect Object::getRect() const
{
    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;
    rect.w = m_width;
    rect.h = m_height;
    return rect;
}


void Object::draw(SDL_Surface* window_surface)
{
    SDL_Rect object_position = getRect();

    object_spritesheet.draw_selected_sprite(window_surface, &object_position, 2.0f);
}