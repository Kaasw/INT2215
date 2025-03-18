

#pragma once

#include <SDL.h>

#include <iostream>

class Application
{
public:
    Application();
    ~Application();

    void loop();
    void update(double delta_time);
    void draw();
private:
    SDL_Surface *m_image;
    SDL_Rect     m_image_position;
	double 	 m_image_x;
	double 	 m_image_y;


    SDL_Window  *m_window;
    SDL_Surface *m_window_surface;
    SDL_Event    m_window_event;
};
