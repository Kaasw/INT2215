#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <SDL_image.h>


class Menu {
public:
    enum Option { StartGame = 0, ExitApp = 1 };
    Menu(SDL_Surface* surface, const std::string& bgImagePath);
    ~Menu();

    bool handleEvent(const SDL_Event& e);

    void draw(SDL_Surface* surface);

    Option getSelection() const;
    bool isFinished() const;

private:
    std::vector<std::string> _items;
    int _selectedIndex;
    bool _finished;
    TTF_Font* _font;
    SDL_Color _normalColor;
    SDL_Color _highlightColor;
    SDL_Surface* _background;  // background image
};