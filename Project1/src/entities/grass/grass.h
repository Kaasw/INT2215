#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  

class Grass : public Object {
public:
    Grass(int x, int y, int w, int h);
    ~Grass() = default;
    void draw(SDL_Surface* window_surface);


private:
    Spritesheet grass_spritesheet;
    int m_grass_columns;
};
