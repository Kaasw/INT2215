#pragma once  
#include <SDL.h>  
#include "../../graphics/sprite/SpriteSheet.h"  
#include "../object/Object.h"  

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
   void update(float deltaTime) override;  
   void draw(SDL_Surface* window_surface);  
   void handleInput(SDL_Event& event);  

private:  
   Direction m_direction;  
   int m_spritesheet_column;  
   Spritesheet m_spritesheet;  
};
