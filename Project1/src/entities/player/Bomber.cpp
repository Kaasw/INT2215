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
    if (m_invulnerable) {
        m_invulTimer -= delta_time;
        if (m_invulTimer <= 0.0f) {
            m_invulnerable = false;
        }
    }

	if (isDead()) {
        m_spritesheet.select_sprite(0, 3);
		return;
	}
    
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

	}
	else if (vx > 0) {
		m_spritesheet.select_sprite(SPRITESHEET_RIGHT, m_spritesheet_column);
		previousSprite = SPRITESHEET_RIGHT;

	}
	else if (vy < 0) {
		m_spritesheet.select_sprite(SPRITESHEET_UP, m_spritesheet_column);
		previousSprite = SPRITESHEET_UP;

	}
	else if (vy > 0) {
		m_spritesheet.select_sprite(SPRITESHEET_DOWN, m_spritesheet_column);
		previousSprite = SPRITESHEET_DOWN;

	}
    else {
        m_spritesheet.select_sprite(previousSprite, 0);
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
    if (m_invulnerable) {
        static int flicker = 0;
        if ((flicker++ / 5) % 2 == 0) return;
    }
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
           
            // push bomber back outside
            if (dy > 0)
                m_y = other.y - me.h;
            else
                m_y = other.y + other.h;
            break;
        }
    }
}

void Bomber::takeHit()
{
    if (m_invulnerable) return;       // ignore if still invulnerable

    // start invuln
    m_invulnerable = true;
    m_invulTimer = INVUL_DURATION;

    // actual damage
    if (m_health > 0) {
        --m_health;
        std::cout << "Ouch! Lives left: " << m_health << "\n";
        // optional: respawn or knockback
        //m_x = m_y = 0;
    }
}

bool Bomber::isDead() const {

    return (m_health <= 0);
}

