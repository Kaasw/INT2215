#include "explosion.h"
#include <iostream>

Explosion::Explosion(float x, float y, int w, int h, ExplosionDir dir)
    : Object(x, y, w, h, Type::EXPLOSION)
    , explosion_spritesheet("assets/texture/explosion.png", 4, 4)
    , m_explosion_columns(0)
	, m_explosion_dir(dir)


{
    frameIndex = 0;
    loopCount = 0;
    timer = 0.0f;
    selectCurrentFrame();
}


void Explosion::draw(SDL_Surface* surface)
{
    SDL_Rect dst = getRect();
    explosion_spritesheet.draw_selected_sprite(surface, &dst, 2.0f);
}

bool Explosion::updateExplosion(float delta_time)
{
    timer += delta_time;
    if (timer >= FRAME_DURATION) {
        timer -= FRAME_DURATION;
        ++frameIndex;
        if (frameIndex >= FRAMES_PER_DIR) {
            frameIndex = 0;
            ++loopCount;
            if (loopCount >= MAX_LOOPS)
                return true;  // done all loops, signal removal
        }
        selectCurrentFrame();
    }
    return false;
}

void Explosion::selectCurrentFrame()
{
    int col = 0;
    int row = 0;
    switch (m_explosion_dir) {
    case ExplosionDir::Center:
        col = 0;
        row = (frameIndex == 0 ? 0 : 1);
        break;
    case ExplosionDir::Up:
        col = 1;
        row = (frameIndex == 0 ? 0 : 2);
        break;
    case ExplosionDir::Down:
        col = 1;
        row = (frameIndex == 0 ? 1 : 3);
        break;
    case ExplosionDir::Left:
        col = 3;
        row = (frameIndex == 0 ? 0 : 1);
        break;
    case ExplosionDir::Right:
        col = 2;
        row = (frameIndex == 0 ? 0 : 1);
        break;
    }
    explosion_spritesheet.select_sprite(col, row);
}
