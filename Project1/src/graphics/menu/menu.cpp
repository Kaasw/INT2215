#include "Menu.h"
#include <iostream>

Menu::Menu(SDL_Surface* /*surface*/, const std::string& bgImagePath)
    : _items({ "Start Game", "Exit" })
    , _selectedIndex(0)
    , _finished(false)
    , _font(nullptr)
    , _normalColor{ 255,255,255,255 }
    , _highlightColor{ 255,215,0,255 }
    , _background(nullptr)
{
    // Initialize TTF if needed
    if (TTF_WasInit() == 0) {
        if (TTF_Init() < 0) {
            std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        }
    }
    // Load font
    _font = TTF_OpenFont("assets/fonts/ARCADECLASSIC.ttf", 24);
    if (!_font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    // Initialize SDL_image if needed
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
    }
    // Load background image
    _background = IMG_Load(bgImagePath.c_str());
    if (!_background) {
        std::cerr << "Failed to load background: " << IMG_GetError() << std::endl;
    }
}

Menu::~Menu() {
    if (_background) SDL_FreeSurface(_background);
    if (_font)      TTF_CloseFont(_font);
    IMG_Quit();
    TTF_Quit();
}

bool Menu::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.scancode) {
        case SDL_SCANCODE_UP:
            _selectedIndex = (_selectedIndex - 1 + _items.size()) % _items.size();
            break;
        case SDL_SCANCODE_DOWN:
            _selectedIndex = (_selectedIndex + 1) % _items.size();
            break;
        case SDL_SCANCODE_RETURN:
            _finished = true;
            break;
        default:
            break;
        }
    }
    return _finished;
}

void Menu::draw(SDL_Surface* surface) {
    // Draw background stretched to window
    if (_background) {
        SDL_Rect dst = { 0,0, surface->w, surface->h };
        SDL_BlitScaled(_background, nullptr, surface, &dst);
    }
    else {
        SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0, 0, 0));
    }


    const int startX = 250;  
    const int startY = 400;  
    const int spacing = 50;  

    for (size_t i = 0; i < _items.size(); ++i) {
        SDL_Color color = (i == _selectedIndex) ? _highlightColor : _normalColor;
        SDL_Surface* textSurf = TTF_RenderText_Solid(_font, _items[i].c_str(), color);
        if (textSurf) {
            SDL_Rect dstRect;
            dstRect.x = startX;
            dstRect.y = startY + static_cast<int>(i) * spacing;
            dstRect.w = textSurf->w;
            dstRect.h = textSurf->h;
            SDL_BlitSurface(textSurf, nullptr, surface, &dstRect);
            SDL_FreeSurface(textSurf);
        }
    }
}

Menu::Option Menu::getSelection() const {
    return static_cast<Option>(_selectedIndex);
}

bool Menu::isFinished() const {
    return _finished;
}


