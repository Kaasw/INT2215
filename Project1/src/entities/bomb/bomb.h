#pragma once  
#include <SDL.h>  
#include <iostream>
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  

class Bomb : public Object {
public:
    Bomb(float x, float y, int w, int h);
    ~Bomb() = default;
    void draw(SDL_Surface* window_surface);
    bool isDestroyed = false;
    void setDestroy() {
        isDestroyed = true;
    };
    bool getDestruct() {
        return isDestroyed;
    }
    static constexpr int FRAME_COUNT = 3;   
    static constexpr int MAX_LOOPS = 3;   
    bool updateBomb(float delta_time);
    int   m_loop_count = 0;
    bool  isPassable = true;

private:
    Spritesheet bomb_spritesheet;
    int m_bomb_columns;
    bool m_requestBomb = false;
    float m_destroy_timer = 0.f;       
    static constexpr float FRAME_DURATION = 5.0f; 
    static constexpr int   LOOP_FRAME = 9;  

};
