#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SDL.h>
#include <SDL_image.h>

class Spritesheet {
public:
    Spritesheet(const char* path, int rows, int columns);
    ~Spritesheet();
    void select_sprite(int x, int y);
    void draw_selected_sprite(SDL_Surface* window_surface, SDL_Rect* position, float scale);

private:
    SDL_Surface* m_spritesheet_image;
    SDL_Rect m_clip;
    int m_rows;
    int m_columns;
};

#endif // SPRITESHEET_H