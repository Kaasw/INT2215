#include "ai.h"
#include "src/entities/brick/brick.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

int previousDirection = 0;
float previousDistance = 0.0f;
float firstDistance = 0.0f;
float bestDist = std::numeric_limits<float>::infinity();

float distanceRecalcTimer = 0.0f;

const float DISTANCE_RECALC_INTERVAL = 1.0f; // Recalculate every 1 second

AI::AI(float x, float y, int w, int h)
    : Object(x, y, w, h, Type::AI)
    , m_AI_columns(0)
    , AI_spritesheet("assets/texture/ai.png", 4, 4)
{
    AI_spritesheet.select_sprite(0, 0);
    SDL_Rect firstai = getRect();
    SDL_Rect playerRect;
    firstDistance = distance(playerRect, firstai);
}

void AI::update(float delta_time,
    std::list<Object*>& collidables,
    std::list<Bomb*>& m_bombs,
    SDL_Rect& playerRect)
{
    float vx = 0.0f, vy = 0.0f;
    const float speed = 5.0f;

   
    distanceRecalcTimer += delta_time;

   
    if (distanceRecalcTimer >= DISTANCE_RECALC_INTERVAL) {
        distanceRecalcTimer = 0.0f;

        bestDist = std::numeric_limits<float>::infinity();

        AI_Direction bestDir = direction;
        for (auto testDir : { AI_Direction::LEFT, AI_Direction::RIGHT, AI_Direction::UP, AI_Direction::DOWN }) {
            // skip if flagged blocked
            if ((testDir == AI_Direction::LEFT && leftBlocked) ||
                (testDir == AI_Direction::RIGHT && rightBlocked) ||
                (testDir == AI_Direction::UP && upBlocked) ||
                (testDir == AI_Direction::DOWN && downBlocked))
                continue;

            SDL_Rect testPos = getRect();
            if (testDir == AI_Direction::LEFT)  testPos.x -= speed * delta_time;
            if (testDir == AI_Direction::RIGHT) testPos.x += speed * delta_time;
            if (testDir == AI_Direction::UP)    testPos.y -= speed * delta_time;
            if (testDir == AI_Direction::DOWN)  testPos.y += speed * delta_time;

            float d = distance(playerRect, testPos);
            if (d < bestDist) {
                bestDist = d;
                bestDir = testDir;
            }
        }
        direction = bestDir;
        std::cout << "Recalculated direction: " << static_cast<int>(bestDir) << " at distance: " << bestDist << std::endl;
    }

    switch (direction) {
    case AI_Direction::LEFT:  vx = -1.0f; break;
    case AI_Direction::RIGHT: vx = +1.0f; break;
    case AI_Direction::UP:    vy = -1.0f; break;
    case AI_Direction::DOWN:  vy = +1.0f; break;
    }

    clearLeftBlocked();
    clearRightBlocked();
    clearUpBlocked();
    clearDownBlocked();

    moveX(vx * speed * delta_time, collidables, direction);
    moveY(vy * speed * delta_time, collidables, direction);

    timer += delta_time;
    if (timer >= FRAME_DURATION)
    {
        timer -= FRAME_DURATION;
        ++m_AI_columns;

        if (m_AI_columns >= FRAME_COUNT)
        {
            m_AI_columns = 0;
        }
    }
}

void AI::draw(SDL_Surface* window_surface)
{
    SDL_Rect dst = getRect();
    AI_spritesheet.draw_selected_sprite(window_surface, &dst, 2.0f);
}


void AI::moveX(float dx, std::list<Object*>& collidables, AI_Direction bestDir)
{
    m_x += dx;
    SDL_Rect rect = getRect();
    //collide with walls
    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&rect, &other))
        {
            if (direction == AI_Direction::LEFT) {
                setLeftBlocked(true);
                direction = bestDir;
            }
            else if (direction == AI_Direction::RIGHT) {
                setRightBlocked(true);
                direction = bestDir;
            }
            if (dx > 0)
                m_x = other.x - rect.w;
            else
                m_x = other.x + other.w;
        }
    }
}

void AI::moveY(float dy, std::list<Object*>& collidables, AI_Direction bestDir)
{
    m_y += dy;
    SDL_Rect me = getRect();

    for (auto* obj : collidables)
    {
        if (obj == this) continue;
        SDL_Rect other = obj->getRect();
        if (SDL_HasIntersection(&me, &other))
        {
            std::cout << "AI blocked! Y" << std::endl;
            if (direction == AI_Direction::UP) {
                setUpBlocked(true);
                direction = bestDir;
            }
            else if (direction == AI_Direction::DOWN) {
                setDownBlocked(true);
                direction = bestDir;
            }

            if (dy > 0)
                m_y = other.y - me.h;
            else
                m_y = other.y + other.h;
            break;
        }
    }
}

float AI::distance(SDL_Rect& playerRect, SDL_Rect& ai) {
    float dx = abs(playerRect.x - ai.x);
    float dy = abs(playerRect.y - ai.y);
    return sqrt(dx * dx + dy * dy);
}