#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  

class Brick : public Object {
public:
    Brick(int x, int y, int w, int h);
    ~Brick() = default;
    void draw(SDL_Surface* window_surface);


private:
    Spritesheet brick_spritesheet;
    int m_brick_columns;
};
