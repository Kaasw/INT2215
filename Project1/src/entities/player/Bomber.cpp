#include "Bomber.h"
#include "src/entities/brick/brick.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

int const SPRITESHEET_UP = 0;
int const SPRITESHEET_LEFT = 3;
int const SPRITESHEET_RIGHT = 1;
int const SPRITESHEET_DOWN = 2;
int previousSprite = SPRITESHEET_RIGHT;
int previousColumn = 0;

Bomber::Bomber(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::PLAYER)
    , m_spritesheet_column(0)
    , m_requestBomb(false)
    , m_spritesheet("assets/texture/Player.png", 4, 4)
{
}

void Bomber::update(float delta_time,
    std::list<Object*>& collidables,
    std::list<Bomb*>& m_bombs)
{
    
    // 1) Spawn bomb if requested
    if (m_requestBomb) {
        float tx = std::floor(m_x / m_width) * m_width;
        float ty = std::floor(m_y / m_height) * m_height;
        m_bombs.push_back(new Bomb(tx, ty, m_width, m_height));
        m_requestBomb = false;
    }

    // 2) Poll keyboard for continuous multi-key movement
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    float vx = 0.0f, vy = 0.0f;
    if (keys[SDL_SCANCODE_W]) vy -= 1.0f;
    if (keys[SDL_SCANCODE_S]) vy += 1.0f;
    if (keys[SDL_SCANCODE_A]) vx -= 1.0f;
    if (keys[SDL_SCANCODE_D]) vx += 1.0f;

    // 3) Choose sprite row based on primary axis of movement
    if (vx < 0) {
        m_spritesheet.select_sprite(SPRITESHEET_LEFT, m_spritesheet_column);
		previousSprite = SPRITESHEET_LEFT;
		previousColumn = 0;
	}
	else if (vx > 0) {
		m_spritesheet.select_sprite(SPRITESHEET_RIGHT, m_spritesheet_column);
		previousSprite = SPRITESHEET_RIGHT;
		previousColumn = 0;
	}
	else if (vy < 0) {
		m_spritesheet.select_sprite(SPRITESHEET_UP, m_spritesheet_column);
		previousSprite = SPRITESHEET_UP;
		previousColumn = 0;
	}
	else if (vy > 0) {
		m_spritesheet.select_sprite(SPRITESHEET_DOWN, m_spritesheet_column);
		previousSprite = SPRITESHEET_DOWN;
		previousColumn = 0;
	}
    else {
        m_spritesheet.select_sprite(previousSprite, previousColumn);
    }
       // idle frame

    // 4) Movement & collision
    const float speed = 5.0f;
    moveX(vx * speed * delta_time, collidables);
    moveY(vy * speed * delta_time, collidables);

    // 5) Advance animation column
    m_spritesheet_column++;
    if (m_spritesheet_column > 2)
        m_spritesheet_column = 0;
}

void Bomber::draw(SDL_Surface* window_surface)
{
    SDL_Rect dst = getRect();
    m_spritesheet.draw_selected_sprite(window_surface, &dst, 2.0f);
}

void Bomber::handleInput(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.scancode == SDL_SCANCODE_SPACE)
    {
        m_requestBomb = true;
    }
}

void Bomber::moveX(float dx, std::list<Object*>& collidables)
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

void Bomber::moveY(float dy, std::list<Object*>& collidables)
{
    m_y += dy;
    SDL_Rect me = getRect();

    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&me, &other))
        {
            // if it's an explosion, mark brick for destruction
            if (obj->getType() == Type::EXPLOSION)
            {
                std::cout << "Explosion hit – marking for removal\n";
                dynamic_cast<Brick*>(obj)->setDestroy();
            }
            // push bomber back outside
            if (dy > 0)
                m_y = other.y - me.h;
            else
                m_y = other.y + other.h;
            break;
        }
    }
}
