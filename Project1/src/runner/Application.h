#pragma once

#include <SDL.h>
#include "../entities/player/Bomber.h"
#include "../graphics/map/map.h"
#include "../entities/brick/brick.h"
#include "../entities/wall/wall.h"
#include "../entities/object/Object.h"
#include "../entities/bomb/bomb.h"
#include "../entities/explosion/explosion.h"
#include "../entities/baloon/baloon.h"
#include "../entities/oneal/oneal.h"
#include "../entities/AI/ai.h"
#include "../graphics/menu/menu.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <algorithm>  
#include <iostream>
#include <list>

class Application {
public:
    struct Transition {
        float timer = 0.0f;
        float duration = 1.0f;       // total seconds fade‐out + fade‐in
        bool  loading = false;      // have we already swapped levels?
    };
    Application();
    ~Application();
    void loop();

private:
    enum class AppState { Menu, Playing, GameOver, Exiting, Win };

    // SDL
    SDL_Window* m_window;
    SDL_Surface* m_window_surface;
    SDL_Event     m_window_event;

    // State & Menu
    AppState      _state;
    Menu* _menu;
    Transition  _transition;

    // Game entities & collections
    Bomber        m_bomber;
    Map           m_map;
    std::list<Object*>    m_collidables;
    std::list<Bomb*>      m_bombs;
    std::list<Explosion*> m_explosions;
    std::list<Baloon*>    m_baloons;
    std::list<Oneal*>     m_oneals;
    std::list<AI*>        m_AI;

    // HUD and GameOver
    int           m_score;
    int           m_lives;
    SDL_Surface* m_heartSurface;
    TTF_Font* m_hudFont;

    // GameOver selection
    int           _gameOverSelection; // 0 = Retry, 1 = Quit
    int _currentLevel = 1;
    int        _nextLevel = 1;
    int 	   _maxLevel = 2;
    int _winSelection;

    //Sound
    Mix_Music* m_bgm = nullptr;
    Mix_Chunk* m_sfxExplosion = nullptr;
	bool mute = false;
    void toggleMute();

    // Internal
    void update(float dt);
    void draw();
    void handleGameOverInput(const SDL_Event& e, bool& running);
    void handleWinInput(const SDL_Event& e, bool& running);
    void drawGameOver();
	void drawWinScreen();
    void resetGame();
	void loadLevel(int level);
};