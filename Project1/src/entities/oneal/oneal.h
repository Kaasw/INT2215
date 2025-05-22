#pragma once
#include <SDL.h>  
#include <iostream>
#include "src/graphics/sprite/SpriteSheet.h"  
#include "src/entities/object/Object.h"  
#include "src/entities/brick/brick.h"
#include "src/entities/wall/wall.h"
#include "src/entities/bomb/bomb.h"
#include <list>

enum class OnealDirection {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

class Oneal : public Object {
public:
    Oneal(float x, float y, int w, int h);
    ~Oneal() = default;
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
        std::list<Bomb*>& m_bombs,
        const SDL_Rect& playerRect);
    static constexpr int FRAME_COUNT = 3;

private:
    Spritesheet oneal_spritesheet;
    int m_oneal_columns;
	OnealDirection direction = OnealDirection::LEFT; 
    float timer = 0.0f;               
    static constexpr float FRAME_DURATION = 5.0f;
    static constexpr float SIGHT_RADIUS = 50.0f;
    static constexpr float ALIGN_THRESH = 4.0f;
    void moveX(float dx, std::list<Object*>& collidables);
    void moveY(float dx, std::list<Object*>& collidables);



};
