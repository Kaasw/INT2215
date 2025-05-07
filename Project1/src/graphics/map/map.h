#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "src/entities/brick/brick.h"
#include "src/entities/wall/wall.h"
#include "src/entities/baloon/baloon.h"
#include "src/entities/oneal/oneal.h"
#include "src/entities/AI/ai.h"


class Map {
public:
    Map();
    ~Map();

    bool loadMap(const std::string& filePath);

    void draw(SDL_Surface* window_surface);
    
    const std::vector<Object*>& getObjects() const;
    void removeObject(Object* obj);
	void clearObjects();
    void clearMap();

private:
    std::vector<std::vector<Object*>> m_mapData;  
    std::vector<Object*> m_objects; 
    int m_brickWidth;
    int m_brickHeight;
   

};