#pragma once  
#include <SDL.h>  
#include <iostream>
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  

class Brick : public Object {
public:
    Brick(float x, float y, int w, int h);
    ~Brick() = default;
    void draw(SDL_Surface* window_surface);
    bool isDestroyed = false;
    void setDestroy() { 
        isDestroyed = true; };
    bool getDestruct() {
        return isDestroyed;
    }

    bool updateBrick(float delta_time);


private:
    Spritesheet brick_spritesheet;
    int m_brick_columns;

    // timing
    float m_destroy_timer = 0.f;       // accumulates delta_time
    static constexpr float FRAME_DURATION = 5.0f;  // seconds per frame
    static constexpr int   DESTROY_FRAMES = 3;    // columns 1,2,3
 
};
