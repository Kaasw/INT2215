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
    : _state(AppState::Menu)
    , m_bomber(32, 32, 32, 32)
    , m_lives(3)
    , m_heartSurface(nullptr)
    , _gameOverSelection(0)
   
{
    // SDL init
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // Create window
    m_window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, 0);
    m_window_surface = SDL_GetWindowSurface(m_window);

    // Load HUD assets
    m_heartSurface = IMG_Load("assets/texture/heart.png");
    m_hudFont = TTF_OpenFont("assets/fonts/ARCADECLASSIC.ttf", 18);
    if (!m_hudFont) std::cerr << "HUD font error: " << TTF_GetError() << std::endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << "\n";
    }

    // Load background music (looped indefinitely)
    m_bgm = Mix_LoadMUS("assets/music/bg.mp3");
    if (!m_bgm) {
        std::cerr << "Failed to load BGM: " << Mix_GetError() << "\n";
    }
    else {
        if (m_bgm) {
            Mix_PlayMusic(m_bgm, -1);
            mute = false;
        }
    }

    m_sfxExplosion = Mix_LoadWAV("assets/music/bomb.wav");
    if (!m_sfxExplosion) {
        std::cerr << "Failed to load explosion SFX: " << Mix_GetError() << "\n";
    }
    // Setup menu
    _menu = new Menu(m_window_surface, "assets/texture/menu.png");

    _currentLevel = 1;
    loadLevel(_currentLevel);
}

void Application::loadLevel(int levelNumber) {
    // 1) Delete and clear all dynamic lists that OWN their pointers
    for (auto* bomb : m_bombs)      delete bomb;
    for (auto* exp : m_explosions) delete exp;
    m_bombs.clear();
    m_explosions.clear();

    // 2) Clear out all lists that reference map objects
    m_collidables.clear();
    m_baloons.clear();
    m_oneals.clear();
    m_AI.clear();

    // 3) Now let the Map itself delete its old Objects
    m_map.clearMap();   // only deletes things inside m_map.getObjects()

    // 4) Load the new map file
    std::string path = "src/levels/level" + std::to_string(levelNumber) + ".txt";
    m_map.loadMap(path);

    // 5) Repopulate your reference lists FROM THE NEW map.getObjects()
    for (auto* obj : m_map.getObjects()) {
        if (auto* b = dynamic_cast<Brick*>(obj)) {
            m_collidables.push_back(b);
        }
        else if (auto* w = dynamic_cast<Wall*>(obj)) {
            m_collidables.push_back(w);
        }
        else if (auto* bal = dynamic_cast<Baloon*>(obj)) {
            m_baloons.push_back(bal);
        }
        else if (auto* one = dynamic_cast<Oneal*>(obj)) {
            m_oneals.push_back(one);
        }
        else if (auto* ai = dynamic_cast<AI*>(obj)) {
            m_AI.push_back(ai);
        }
    }

    // 6) Reset player & HUD
    //m_bomber = Bomber(32, 32, 32, 32);
	//m_bomber.setPosition(32, 32);
    m_lives = m_bomber.getHealth();
    // m_score = 0;   // if you’d like

}


Application::~Application()
{
    m_map.clearMap();
    m_collidables.clear();
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::loop() {
    bool running = true;
    const float dt = 1.0f / 60.0f;

    while (running) {
        while (SDL_PollEvent(&m_window_event) > 0) {
            if (_state == AppState::Menu) {
                if (_menu->handleEvent(m_window_event)) {
                    if (_menu->getSelection() == Menu::StartGame)
                        _state = AppState::Playing;
                    else
                        running = false;
                }
            }
            else if (_state == AppState::Playing) {
                         if (m_window_event.type == SDL_KEYDOWN &&
                    m_window_event.key.keysym.scancode == SDL_SCANCODE_M)
                {
                    toggleMute();
                    continue;  // skip the rest of this event
                }

                // existing gameplay input
                m_bomber.handleInput(m_window_event);
                if (m_window_event.type == SDL_QUIT)
                    running = false;
                m_bomber.handleInput(m_window_event);
                if (m_window_event.type == SDL_QUIT) running = false;
            }
            else if (_state == AppState::GameOver) {
                handleGameOverInput(m_window_event, running);
            }
  
			else if (_state == AppState::Win) {
				handleWinInput(m_window_event, running);
			}

        }

        if (_state == AppState::Menu) {
            _menu->draw(m_window_surface);
            SDL_UpdateWindowSurface(m_window);
        }
        else if (_state == AppState::Playing) {
            update(dt);
            draw();
            // Transition to GameOver if no lives left
            if (m_bomber.getHealth() <= 0) {
                _state = AppState::GameOver;
                _gameOverSelection = 0;
            }
        }
        else if (_state == AppState::GameOver) {
            drawGameOver();
            SDL_UpdateWindowSurface(m_window);
        }

        else if (_state == AppState::Win) {
            drawWinScreen();
            SDL_UpdateWindowSurface(m_window);
        }
    }
}


void Application::update(float delta_time)
{
    m_bomber.update(delta_time, m_collidables, m_bombs);
    // at the end of update, after you remove dead enemies:
    

    SDL_Rect playerRect = m_bomber.getRect();
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

            if (m_sfxExplosion) {
                Mix_PlayChannel(-1, m_sfxExplosion, 0);
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

    for (auto* e : m_explosions) {
        SDL_Rect er = e->getRect();
        if (SDL_HasIntersection(&playerRect, &er) &&
            !m_bomber.isInvulnerable())   // only hit if not invulnerable
        {
            m_bomber.takeHit();
            break;  // only one hit per frame
        }
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

    for (auto it = m_baloons.begin(); it != m_baloons.end(); /* no ++it */)
    {
        Baloon* b = *it;
		SDL_Rect bl = b->getRect();
        if (SDL_HasIntersection(&playerRect, &bl) &&
            !m_bomber.isInvulnerable())   // only hit if not invulnerable
        {
            m_bomber.takeHit();
            break;  
        }
        // 1) Move & animate the balloon as before
        b->update(delta_time, m_collidables, m_bombs);

        {
            SDL_Rect br = b->getRect();
            for (auto* e : m_explosions)
            {
                SDL_Rect er = e->getRect();
                if (SDL_HasIntersection(&br, &er))
                {
                    b->isDestroyed = true;
                    break;
                }
            }
        }

        // 3) If marked dead, remove from everything
        if (b->isDestroyed)
        {
            m_map.removeObject(b);
            m_collidables.remove(b);
            delete b;
            it = m_baloons.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = m_oneals.begin(); it != m_oneals.end(); /* no ++it */)
    {
        Oneal* o = *it;
        SDL_Rect oneal = o->getRect();
        if (SDL_HasIntersection(&playerRect, &oneal) &&
            !m_bomber.isInvulnerable())   // only hit if not invulnerable
        {
            m_bomber.takeHit();
            break;
        }
        // 1) Move & animate the balloon as before
        o->update(delta_time, m_collidables, m_bombs, playerRect);

        {
            SDL_Rect br = o->getRect();
            for (auto* e : m_explosions)
            {
                SDL_Rect er = e->getRect();
                if (SDL_HasIntersection(&br, &er))
                {
                    o->isDestroyed = true;
                    break;
                }
            }
        }

        // 3) If marked dead, remove from everything
        if (o->isDestroyed)
        {
            m_map.removeObject(o);
            m_collidables.remove(o);
            delete o;
            it = m_oneals.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_AI.begin(); it != m_AI.end(); /* no ++it */)
    {
        AI* o = *it;
        SDL_Rect oneal = o->getRect();
        if (SDL_HasIntersection(&playerRect, &oneal) &&
            !m_bomber.isInvulnerable())   // only hit if not invulnerable
        {
            m_bomber.takeHit();
            break;
        }
        // 1) Move & animate the balloon as before
        o->update(delta_time, m_collidables, m_bombs, playerRect);

        {
            SDL_Rect br = o->getRect();
            for (auto* e : m_explosions)
            {
                SDL_Rect er = e->getRect();
                if (SDL_HasIntersection(&br, &er))
                {
                    o->isDestroyed = true;
                    break;
                }
            }
        }

        // 3) If marked dead, remove from everything
        if (o->isDestroyed)
        {
            m_map.removeObject(o);
            m_collidables.remove(o);
            delete o;
            it = m_AI.erase(it);
        }
        else
        {
            ++it;
        }
    }

    bool noEnemies = m_baloons.empty()
        && m_oneals.empty()
        && m_AI.empty();
    bool noExplosions = m_explosions.empty();

    // only advance once both all enemies are gone and all explosions have finished
    if (noEnemies && noExplosions) {
		if (_currentLevel >= _maxLevel) {
			_state = AppState::Win;
			_winSelection = 0;
		}
		else {
            ++_currentLevel;
            loadLevel(_currentLevel);
		}
    
    }
}


void Application::draw()
{
    // 1) Clear the window to your background color (pink here)
    SDL_FillRect(
        m_window_surface,
        nullptr,
        SDL_MapRGB(m_window_surface->format, 255, 153, 204)
    );

    // 2) Blit everything else as you already do:
    m_map.draw(m_window_surface);
    for (auto* b : m_bombs)      b->draw(m_window_surface);
    for (auto* e : m_explosions) e->draw(m_window_surface);
    for (auto* bal : m_baloons)  bal->draw(m_window_surface);
    for (auto* one : m_oneals)   one->draw(m_window_surface);
    for (auto* ai : m_AI)        ai->draw(m_window_surface);
    m_bomber.draw(m_window_surface);

    // Draw hearts (lives)
    m_lives = m_bomber.getHealth();
    const int heartSize = 32, padding = 10;
    for (int i = 0; i < m_lives; ++i) {
        SDL_Rect dst = {
            m_window_surface->w - padding - heartSize * (i + 1),
            padding,
            heartSize, heartSize
        };
        SDL_BlitScaled(m_heartSurface, nullptr, m_window_surface, &dst);
    }

    // 3) Present the entire surface to screen
    SDL_UpdateWindowSurface(m_window);
}

void Application::handleGameOverInput(const SDL_Event& e, bool& running)
{
    if (e.type != SDL_KEYDOWN) return;
    switch (e.key.keysym.scancode) {
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_DOWN:
        _gameOverSelection = 1 - _gameOverSelection;
        break;
    case SDL_SCANCODE_RETURN:
        if (_gameOverSelection == 0) {
            resetGame();
            _state = AppState::Playing;
        }
        else {
            running = false;
        }
        break;
    default:
        break;
    }
}

void Application::drawGameOver()
{
    // Clear
    SDL_FillRect(m_window_surface, nullptr,
        SDL_MapRGB(m_window_surface->format, 0, 0, 0));
    // Draw "Game Over" text
    SDL_Color red = { 255,0,0,255 };
    SDL_Surface* goSurf = TTF_RenderText_Solid(m_hudFont, "Game Over", red);
    SDL_Rect goDst{ (m_window_surface->w - goSurf->w) / 2, 150, goSurf->w, goSurf->h };
    SDL_BlitSurface(goSurf, nullptr, m_window_surface, &goDst);
    SDL_FreeSurface(goSurf);

    // Draw options
    const char* opts[2] = { "Retry", "Quit" };
    for (int i = 0; i < 2; ++i) {
        SDL_Color color = (i == _gameOverSelection) ? red : SDL_Color{ 255,255,255,255 };
        SDL_Surface* optSurf = TTF_RenderText_Solid(m_hudFont, opts[i], color);
        SDL_Rect optDst{ (m_window_surface->w - optSurf->w) / 2,
                          250 + i * 50,
                          optSurf->w, optSurf->h };
        SDL_BlitSurface(optSurf, nullptr, m_window_surface, &optDst);
        SDL_FreeSurface(optSurf);
    }
}

void Application::handleWinInput(const SDL_Event& e, bool& run) {
    if (e.type != SDL_KEYDOWN) return;
    if (e.key.keysym.scancode == SDL_SCANCODE_UP ||
        e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
        _winSelection = 1 - _winSelection;
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
        if (_winSelection == 0) {
            loadLevel(1); m_bomber.setPosition(32, 32);
            _currentLevel = 1;
            _state = AppState::Playing;
        }
        else run = false;
    }
}

void Application::drawWinScreen() {
    SDL_FillRect(m_window_surface, nullptr, SDL_MapRGB(m_window_surface->format, 0, 0, 0));
    SDL_Color green{ 0,255,0,255 };
    auto* surf = TTF_RenderText_Solid(m_hudFont, "You Win!", green);
    SDL_Rect d{ (640 - surf->w) / 2,150,surf->w,surf->h }; SDL_BlitSurface(surf, nullptr, m_window_surface, &d); SDL_FreeSurface(surf);
    const char* opts[2] = { "Play Again","Quit" };
    for (int i = 0; i < 2; i++) {
        SDL_Color c = (i == _winSelection) ? green : SDL_Color{ 255,255,255,255 };
        surf = TTF_RenderText_Solid(m_hudFont, opts[i], c);
        SDL_Rect d2{ (640 - surf->w) / 2,250 + i * 50,surf->w,surf->h };
        SDL_BlitSurface(surf, nullptr, m_window_surface, &d2);
        SDL_FreeSurface(surf);
    }
    SDL_UpdateWindowSurface(m_window);
}


void Application::resetGame()
{
	loadLevel(1);
	m_bomber.setPosition(32, 32);
	m_bomber.setHealth(3);
	m_lives = m_bomber.getHealth();
	_state = AppState::Playing;
	_gameOverSelection = 0;


}

void Application::toggleMute() {
    mute = !mute;
    if (mute) {
        Mix_PauseMusic();
    }
    else {
        Mix_ResumeMusic();
    }
}