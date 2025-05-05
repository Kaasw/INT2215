#pragma once

#include <SDL.h>
#include "../entities/player/Bomber.h"
#include "../graphics/map/map.h"
#include "../entities/brick/brick.h"
#include "../entities/wall/wall.h"
#include "../entities/object/Object.h"
#include <algorithm>  
#include <iostream>
#include <list>

class Application
{
public:
    Application();
    ~Application();

    void loop();
    void update(float delta_time);
    void draw();
private:
    Bomber m_bomber;
	Map m_map;
    SDL_Window* m_window;
    SDL_Surface* m_window_surface;
    SDL_Event    m_window_event;
    std::list<Object*> m_collidables;
};