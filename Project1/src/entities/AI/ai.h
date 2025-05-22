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


class AI : public Object {
	enum class AI_Direction {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
	};
public:
    AI(float x, float y, int w, int h);
    ~AI() = default;
    void draw(SDL_Surface* window_surface);
    bool isDestroyed = false;
    void setDestroy() {
        isDestroyed = true;
    };
    bool getDestruct() {
        return isDestroyed;
    }

	bool isBlocked = false; 
	bool rightBlocked = false;
	bool leftBlocked = false;
	bool upBlocked = false;
	bool downBlocked = false;

	void setRightBlocked(bool blocked) {
		rightBlocked = blocked;
	}
	void setLeftBlocked(bool blocked) {
		leftBlocked = blocked;
	}
	void setUpBlocked(bool blocked) {
		upBlocked = blocked;
	}
	void setDownBlocked(bool blocked) {
		downBlocked = blocked;
	}
	void clearRightBlocked() {
		rightBlocked = false;
	}
	void clearLeftBlocked() {
		leftBlocked = false;
	}
	void clearUpBlocked() {
		upBlocked = false;
	}
	void clearDownBlocked() {
		downBlocked = false;
	}

    void update(float delta_time,
        std::list<Object*>& collidables,
        std::list<Bomb*>& m_bombs,
         SDL_Rect& playerRect);
    static constexpr int FRAME_COUNT = 3;

private:
    Spritesheet AI_spritesheet;
    int m_AI_columns;
    float timer = 0.0f;                   
    static constexpr float FRAME_DURATION = 5.0f;
    static constexpr float SIGHT_RADIUS = 50.0f;
    static constexpr float ALIGN_THRESH = 4.0f;
    void moveX(float dx, std::list<Object*>& collidables, AI_Direction bestDir);
    void moveY(float dx, std::list<Object*>& collidables, AI_Direction bestDir);
	AI_Direction direction = AI_Direction::LEFT; 
	float distance( SDL_Rect& playerRect,  SDL_Rect& testPos);



};
