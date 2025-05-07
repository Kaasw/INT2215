#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map()
    : m_brickWidth(32)  
    , m_brickHeight(32) 
{
}

Map::~Map() {
    clearMap();
}

bool Map::loadMap(const std::string& filePath) {
    clearMap();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        std::vector<Object*> row;

        for (size_t x = 0; x < line.length(); x++) {
            Object* object = nullptr;

            if (line[x] == 'B') {
                object = new Brick(
                    x * m_brickWidth,     
                    y * m_brickHeight,     
                    m_brickWidth,         
					m_brickHeight
                );
				std::cout << "Brick at: " << x * m_brickWidth << ", " << y * m_brickHeight << std::endl;
                m_objects.push_back(object);
            }

			else if (line[x] == 'W') {
	
				object = new Wall(
					x * m_brickWidth,   
					y * m_brickHeight,     
					m_brickWidth,
					m_brickHeight
				);
                m_objects.push_back(object);
                std::cout << "Wall at: " << x * m_brickWidth << ", " << y * m_brickHeight << std::endl;
			}

            else if (line[x] == 'Q') {

                object = new Baloon(
                    x * m_brickWidth,
                    y * m_brickHeight,
                    m_brickWidth,
                    m_brickHeight
                );
                m_objects.push_back(object);
                std::cout << "Baloon at: " << x * m_brickWidth << ", " << y * m_brickHeight << std::endl;
            }

            else if (line[x] == 'O') {
                object = new Oneal(
                    x * m_brickWidth,
                    y * m_brickHeight,
                    m_brickWidth,
                    m_brickHeight
                );
                m_objects.push_back(object);
                std::cout << "Oneal at: " << x * m_brickWidth << ", " << y * m_brickHeight << std::endl;
            }
			else if (line[x] == 'A') {
				object = new AI(
					x * m_brickWidth,
					y * m_brickHeight,
					m_brickWidth,
					m_brickHeight
				);
				m_objects.push_back(object);
				std::cout << "AI at: " << x * m_brickWidth << ", " << y * m_brickHeight << std::endl;
			}


            row.push_back(object);
        }

        m_mapData.push_back(row);


        y++;
    }
    return true;
}

void Map::draw(SDL_Surface* window_surface) {
    Uint32 green = SDL_MapRGB(window_surface->format, 0, 153, 51);
    SDL_FillRect(window_surface, nullptr, green);
    for (size_t y = 0; y < m_mapData.size(); y++) {
        for (size_t x = 0; x < m_mapData[y].size(); x++) {
            Object* object = m_mapData[y][x];
            Brick* brick = dynamic_cast<Brick*>(object);
			Wall* wall = dynamic_cast<Wall*>(object);
			Baloon* baloon = dynamic_cast<Baloon*>(object);
			Oneal* oneal = dynamic_cast<Oneal*>(object);

            if (brick) { 
                brick->draw(window_surface);
            }
            else if (wall) {
                wall->draw(window_surface);
            }
			else if (baloon) {
				baloon->draw(window_surface);
			}
            else if (oneal) {
                oneal->draw(window_surface);
            }

        }
    }
}

void Map::removeObject(Object* obj) {
    // 1) erase from the flat list
    auto it = std::remove(m_objects.begin(), m_objects.end(), obj);
    m_objects.erase(it, m_objects.end());

    // 2) null out any grid cell that pointed at it
    for (auto& row : m_mapData) {
        for (auto& cell : row) {
            if (cell == obj)
                cell = nullptr;
        }
    }
}


void Map::clearMap() {

    for (Object* objects : m_objects) {
        delete objects;
    }

    m_objects.clear();
    m_mapData.clear();
}

const std::vector<Object*>& Map::getObjects() const {
    return m_objects;
}

void Map::clearObjects() {
    for (auto* obj : m_objects) {
        delete obj;
    }
    m_objects.clear();
}