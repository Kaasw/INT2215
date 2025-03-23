#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "src/entities/brick/brick.h"
#include "src/entities/grass/grass.h"

class Map {
public:
    Map();
    ~Map();

    bool loadMap(const std::string& filePath);

    void draw(SDL_Surface* window_surface);


private:
    std::vector<std::vector<Object*>> m_mapData;  
    std::vector<Object*> m_bricks; 
    int m_brickWidth;
    int m_brickHeight;
    void clearMap();
};