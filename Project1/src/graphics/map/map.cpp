#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <SDL_image.h>


Map::Map(const string& filePath)
    : m_filePath(filePath), m_tileSurface(nullptr) {
}

void Map::loadMap() {
    ifstream file(m_filePath);
    if (!file.is_open()) {
        cerr << "Failed to open map file: " << m_filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<Object> row;
        istringstream stream(line);
        char tile;
        while (stream >> tile) {
            switch (tile) {
			case 'B':
				row.push_back(Object(0, 0, 64, 64, Object::Type::BRICK));
				break;

            }
        }
        m_mapData.push_back(row);
    }

    file.close();
}

void Map::draw(SDL_Surface* window_surface) {
    if (!m_tileSurface) {
        m_tileSurface = IMG_Load("assets/texture/wall.png");
        if (!m_tileSurface) {
            std::cerr << "Failed to load tile surface: " << SDL_GetError() << std::endl;
            return;
        }
    }

    for (size_t y = 0; y < m_mapData.size(); ++y) {
        for (size_t x = 0; x < m_mapData[y].size(); ++x) {
            SDL_Rect destRect = { static_cast<int>(x * 32), static_cast<int>(y * 32), 32, 32 };
            SDL_BlitSurface(m_tileSurface, nullptr, window_surface, &destRect);
        }
    }
}

