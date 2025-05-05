#include "Application.h"  
#include <SDL_image.h>  
#include "src/entities/object/Object.h" 

Application::Application()
    : m_bomber(32,32,32,32)
	
{
    m_window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        0);

    if (!m_window)
    {
        std::cout << "Failed to create window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }

    m_window_surface = SDL_GetWindowSurface(m_window);

    if (!m_window_surface)
    {
        std::cout << "Failed to get window's surface\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }

    m_map.loadMap("src/levels/level1.txt");
    auto& mapObjects = m_map.getObjects();
    for (auto* obj : mapObjects) {
        if (dynamic_cast<Brick*>(obj) || dynamic_cast<Wall*>(obj)) {
            m_collidables.push_back(obj);
        }
        //m_collidables.push_back(obj);
    }
}

Application::~Application()
{
    for (auto* obj : m_collidables) {
        delete obj;
    }
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::loop()
{
    bool keep_window_open = true;
    while (keep_window_open)
    {

        while (SDL_PollEvent(&m_window_event) > 0)
        {
            m_bomber.handleInput(m_window_event);
            switch (m_window_event.type)
            {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            }
        }

        update(1.0 / 60.0);
        draw();
    }
}

void Application::update(float delta_time)
{
    m_bomber.update(delta_time, m_collidables);
    for (auto it = m_collidables.begin(); it != m_collidables.end(); )
    {
        Brick* b = dynamic_cast<Brick*>(*it);
        if (b && b->getDestruct())
        {
            m_map.removeObject(b);
            delete b;
            it = m_collidables.erase(it);
        }
        else
        {
            ++it;
        }
    }
	
}

void Application::draw()
{
    SDL_FillRect(m_window_surface, NULL, SDL_MapRGB(m_window_surface->format, 255, 153, 204));
	
	m_map.draw(m_window_surface);
    
    m_bomber.draw(m_window_surface);

    SDL_UpdateWindowSurface(m_window);
}