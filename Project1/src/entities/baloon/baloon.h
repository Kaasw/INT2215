#pragma once
#pragma once  
#include <SDL.h>  
#include <iostream>
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include "src/entities/brick/brick.h"
#include "src/entities/wall/wall.h"
#include "src/entities/bomb/bomb.h"
#include <list>

class Baloon : public Object {
public:
    Baloon(float x, float y, int w, int h);
    ~Baloon() = default;
    void draw(SDL_Surface* window_surface);
    bool isDestroyed = false;
    void setDestroy() {
        isDestroyed = true;
    };
    bool getDestruct() {
        return isDestroyed;
    }

    void update(float delta_time,
        std::list<Object*>& collidables,
        std::list<Bomb*>& m_bombs);
    static constexpr int FRAME_COUNT = 3;

private:
    Spritesheet baloon_spritesheet;
    int m_baloon_columns;
	bool direction = false; // true = right, false = left

   
    float timer = 0.0f;                   // NEW
    static constexpr float FRAME_DURATION = 5.0f;
    void moveX(float dx, std::list<Object*>& collidables);
    void moveY(float dy, std::list<Object*>& collidables);


};
