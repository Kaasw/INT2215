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



private:
    Spritesheet brick_spritesheet;
    int m_brick_columns;
 
};
