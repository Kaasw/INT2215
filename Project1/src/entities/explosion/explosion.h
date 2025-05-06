#pragma once  
#include <SDL.h>  
#include <iostream>
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include "src/entities/brick/brick.h"
#include "src/entities/baloon/baloon.h"
#include <list>

enum class ExplosionDir : int {
    Center,
    Up,
    Down,
    Left,
    Right
};
class Explosion : public Object {

public:
    Explosion(float x, float y, int w, int h, ExplosionDir dir);
    ~Explosion() = default;
    void draw(SDL_Surface* window_surface);

	bool updateExplosion(float delta_time, const std::list<Object*>& collidables);
	bool blocked = false; // for collision detection

private:
    Spritesheet explosion_spritesheet;


	int m_explosion_columns;
	int m_explosion_rows;
	ExplosionDir m_explosion_dir;
    float          timer = 0.0f; // accumulation of delta-time
    int            frameIndex = 0;    // current frame (0 or 1)
    int            loopCount = 0;

    static constexpr float FRAME_DURATION = 5.0f;  // seconds per frame
    static constexpr int   FRAMES_PER_DIR = 2;     // two frames per direction
    static constexpr int   MAX_LOOPS = 2;     // complete cycles before removal

    // Selects _sheet sprite based on _dir and _frameIndex
    void selectCurrentFrame();


};
