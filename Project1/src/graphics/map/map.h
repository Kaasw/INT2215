#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "../../entities/object/Object.h"

using namespace std;

class Map {
public:
    Map(const std::string& filePath);
    ~Map() = default;

    void loadMap();
    void draw(SDL_Surface* window_surface);

private:
    string m_filePath;
    vector<vector<Object>> m_mapData;
    SDL_Surface* m_tileSurface;
};