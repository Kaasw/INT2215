#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include <list>

class Bomber: public Object {  
public:  
   enum class Direction {  
       NONE,  
       UP,  
       DOWN,  
       LEFT,  
       RIGHT  
   };  
   Bomber(float x, float y, int w, int h);
   ~Bomber() = default;  

   void update(float delta_time, std::list<Object*>& collidables);
   void draw(SDL_Surface* window_surface);  
   void handleInput(SDL_Event& event);  

private:  
   void moveX(float dx, std::list<Object*>& collidables);
   void moveY(float dy, std::list<Object*>& collidables);
   Direction m_direction;  
   int m_spritesheet_column;  
   Spritesheet m_spritesheet;  
};
