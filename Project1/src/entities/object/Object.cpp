#include "Object.h"

Object::Object(float x, float y, int width, int height, Type type)
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

void Object::moveX(float dx, const std::vector<Object*>& collidables)
{
	m_x += dx;
	SDL_Rect rect = getRect();
	for (auto* obj : collidables)
	{
		if (obj == this) continue;
		SDL_Rect other = obj->getRect();
		if (SDL_HasIntersection(&rect, &other))
		{
			if (dx > 0)
				m_x = other.x - rect.w;
			else
				m_x = other.x + other.w;
		}
	}
}

void Object::moveY(float dy, const std::vector<Object*>& collidables)
{
	m_y += dy;
	SDL_Rect rect = getRect();
	for (auto* obj : collidables)
	{
		if (obj == this) continue;
		SDL_Rect other = obj->getRect();
		if (SDL_HasIntersection(&rect, &other))
		{
			if (dy > 0)
				m_y = other.y - rect.h;
			else
				m_y = other.y + other.h;
		}
	}
}


