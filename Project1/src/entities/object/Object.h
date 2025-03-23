#include <SDL.h>

class Object {
public:
    enum class Type {
        PLAYER,
        ENEMY,
        BRICK,
        GRASS,
        BOMB,
        POWERUP
    };

    Object(int x, int y, int width, int height, Type type);
    virtual ~Object();

    // getters
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    Type getType() const { return m_type; }
    SDL_Rect getRect() const;

    // setters
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    virtual void update(float deltaTime) {}
  
 

protected:
    int m_x, m_y;           
    int m_width, m_height; 
    Type m_type;               
};