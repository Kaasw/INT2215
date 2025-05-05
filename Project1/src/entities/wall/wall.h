#pragma once
#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  

class Wall : public Object {
public:
    Wall(float x, float y, int w, int h);
    ~Wall() = default;
    void draw(SDL_Surface* window_surface);


private:
    Spritesheet wall_spritesheet;
    int m_wall_columns;
};
