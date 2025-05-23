#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include "src/graphics/sprite/SpriteSheet.h"  
#include <vector>

class Object {
public:
    enum class Type {
        PLAYER,
        BALOON,
        BRICK,
        GRASS,
        BOMB,
        AI,
        WALL,
        EXPLOSION,
        ONEAL
    };


    Object(float x, float y, int width, int height, Type type);
    virtual ~Object();

    float getX() const { return m_x; }
    float getY() const { return m_y; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    Type getType() const { return m_type; }
    SDL_Rect getRect();

    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setPosition(float x, float y) { m_x = x; m_y = y; }

    virtual void update(float deltaTime) {}

protected:
    float m_x, m_y;
    int m_width, m_height;
    Type m_type;

   
};

#endif // OBJECT_H