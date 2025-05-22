#pragma once  
#include <SDL.h>  
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include "src/entities/bomb/bomb.h"
#include <list>

class Bomber: public Object {  
public:  
 
   Bomber(float x, float y, int w, int h);
   ~Bomber() = default;  
   void takeHit();           
   bool  isDead() const;
   void update(float delta_time, std::list<Object*>& collidables, std::list<Bomb*>& m_bombs);
   void draw(SDL_Surface* window_surface);  
   void handleInput(SDL_Event& event);  
   int   m_health = 3;
   bool isInvulnerable() const { return m_invulnerable; }
   int getHealth() { return m_health; }
   void setHealth(int health) { m_health = health; }
   
private:  
   void moveX(float dx, std::list<Object*>& collidables);
   void moveY(float dy, std::list<Object*>& collidables);
   int m_spritesheet_column;  
   Spritesheet m_spritesheet;  
   bool m_requestBomb = false;
   bool   m_invulnerable = false;
   float  m_invulTimer = 0.0f;
   static constexpr float INVUL_DURATION = 20.0f;
   
};
