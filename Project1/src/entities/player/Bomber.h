#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include <vector>

class Bomber: public Object {  
public:  
   enum class Direction {  
       NONE,  
       UP,  
       DOWN,  
       LEFT,  
       RIGHT  
   };  
   Bomber(int x, int y, int w, int h);  
   ~Bomber() = default;  
   // Bomber.h
   void update(float delta_time, std::vector<Object*>& collidables);
   void draw(SDL_Surface* window_surface);  
   void handleInput(SDL_Event& event);  

private:  
   Direction m_direction;  
   int m_spritesheet_column;  
   Spritesheet m_spritesheet;  
};
