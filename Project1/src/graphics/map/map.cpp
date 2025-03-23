#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map()
    : m_brickWidth(32)  // Default brick width
    , m_brickHeight(32) // Default brick height
{
}

Map::~Map() {
    clearMap();
}

bool Map::loadMap(const std::string& filePath) {
    // Clear any existing map
    clearMap();

    // Open the map file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    int y = 0;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::vector<Object*> row;

        // Process each character in the line
        for (size_t x = 0; x < line.length(); x++) {
            Object* object = nullptr;

            // Check if the character represents a brick (assuming '1' means brick)
            if (line[x] == 'B') {
                // Create a new brick at the appropriate position
                object = new Brick(
                    x * m_brickWidth,      // x position
                    y * m_brickHeight,     // y position
                    m_brickWidth,          // width
                    m_brickHeight          // height
                );

                // Add the brick to our flat list for easy access
                m_bricks.push_back(object);
            }

			else if (line[x] == 'G') {
				// Create a new grass at the appropriate position
				object = new Grass(
					x * m_brickWidth,      // x position
					y * m_brickHeight,     // y position
					m_brickWidth,          // width
					m_brickHeight          // height
				);
                m_bricks.push_back(object);
			}

            // Add the brick (or nullptr for empty space) to the row
            row.push_back(object);
        }

        // Add the row to the map data
        m_mapData.push_back(row);

        // Move to next row
        y++;
    }

    std::cout << "Map loaded with " << m_bricks.size() << " bricks." << std::endl;
    std::cout << "Map dimensions: " << m_mapData.size() << " rows x "
        << (m_mapData.empty() ? 0 : m_mapData[0].size()) << " columns." << std::endl;
    return true;
}

void Map::draw(SDL_Surface* window_surface) {
    // Draw all bricks using the 2D grid structure to ensure correct positioning
    for (size_t y = 0; y < m_mapData.size(); y++) {
        for (size_t x = 0; x < m_mapData[y].size(); x++) {
            Object* object = m_mapData[y][x];
            Brick* brick = dynamic_cast<Brick*>(object);
			Grass* grass = dynamic_cast<Grass*>(object);
            if (brick) {  // If the cast is successful, it's a Brick object
                brick->draw(window_surface);
            }
            else if (grass) {
                grass->draw(window_surface);
            }
        }
    }
}



void Map::clearMap() {

    for (Object* brick : m_bricks) {
        delete brick;
    }

    // Clear both containers
    m_bricks.clear();
    m_mapData.clear();
}