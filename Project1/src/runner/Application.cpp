#include "Application.h"  
#include <SDL_image.h>  
#include "src/entities/object/Object.h" 

static const std::pair<ExplosionDir, SDL_Point> blastTable[] = {
    { ExplosionDir::Center, { 0,  0} },
    { ExplosionDir::Up,     { 0, -1} },
    { ExplosionDir::Down,   { 0,  1} },
    { ExplosionDir::Left,   {-1,  0} },
    { ExplosionDir::Right,  { 1,  0} }
};
static constexpr int BLAST_COUNT = sizeof(blastTable) / sizeof(blastTable[0]);

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
    m_bomber.update(delta_time, m_collidables, m_bombs);


    for (auto it = m_collidables.begin(); it != m_collidables.end(); )
    {
        Brick* b = dynamic_cast<Brick*>(*it);
        if (b && b->getDestruct())
        {
            if (b->updateBrick(delta_time))
            {
                m_map.removeObject(b);
                delete b;
                it = m_collidables.erase(it);
                continue;
            }

        }
        ++it;
    }

    SDL_Rect playerRect = m_bomber.getRect();
    for (auto it = m_bombs.begin(); it != m_bombs.end(); /*no ++it*/)
    {
        Bomb* bomb = *it;

        if (bomb->updateBomb(delta_time))
        {
			SDL_Rect br = bomb->getRect();
            for (int i = 0; i < BLAST_COUNT; ++i)
            {
                ExplosionDir dir = blastTable[i].first;
                SDL_Point    off = blastTable[i].second;
                float ex = float(br.x + off.x * br.w);
                float ey = float(br.y + off.y * br.h);

                m_explosions.push_back(
                    new Explosion(ex, ey, br.w, br.h, dir)
                );
            }
            m_map.removeObject(bomb);
            m_collidables.remove(bomb);
            delete bomb;
            it = m_bombs.erase(it);
            continue;
        }


        if (bomb->isPassable)
        {
            SDL_Rect br = bomb->getRect();
            if (!SDL_HasIntersection(&playerRect, &br))
            {
                bomb->isPassable = false;
                m_collidables.push_back(bomb);
            }
        }
        ++it;
    }



    for (auto eit = m_explosions.begin(); eit != m_explosions.end(); )
    {
        Explosion* e = *eit;
        if (e->updateExplosion(delta_time, m_collidables))
        {
            delete e;
            eit = m_explosions.erase(eit);
        }
        else
        {
            ++eit;
        }
    }
}


void Application::draw()
{
    SDL_FillRect(m_window_surface, nullptr,
        SDL_MapRGB(m_window_surface->format, 255, 153, 204));

    m_map.draw(m_window_surface);

    // draw bombs (even if they’re still passable)
    for (auto* bomb : m_bombs)
        bomb->draw(m_window_surface);
    for (auto* e : m_explosions)
    {
        SDL_Rect er = e->getRect();
        bool blocked = false;

        for (auto* obj : m_collidables)
        {
            if ((obj->getType() == Object::Type::WALL ||
                obj->getType() == Object::Type::BRICK))
            {
                SDL_Rect orct = obj->getRect();    // ? materialize into a variable
                if (SDL_HasIntersection(&er, &orct))
                {
                    blocked = true;
                    break;
                }
            }
        }

        if (!blocked)
            e->draw(m_window_surface);
    }

    m_bomber.draw(m_window_surface);
    SDL_UpdateWindowSurface(m_window);
}