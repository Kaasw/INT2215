#pragma once

#include <SDL.h>
#include "../entities/player/Bomber.h"
//#include "../graphics/map/Map.h"
#include "../entities/brick/brick.h"
#include <iostream>

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
	Brick m_brick;

    SDL_Window* m_window;
    SDL_Surface* m_window_surface;
    SDL_Event    m_window_event;
};