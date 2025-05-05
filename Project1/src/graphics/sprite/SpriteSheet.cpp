#include "spritesheet.h"
#include <iostream>

Spritesheet::Spritesheet(const char* path, int rows, int columns)
    : m_rows(rows), m_columns(columns)
{
    m_spritesheet_image = IMG_Load(path);
    if (!m_spritesheet_image) {
        std::cerr << "Failed to load spritesheet: " << IMG_GetError() << std::endl;
    }
}

Spritesheet::~Spritesheet()
{
    if (m_spritesheet_image) {
        SDL_FreeSurface(m_spritesheet_image);
    }
}

void Spritesheet::select_sprite(int x, int y)
{
    m_clip.x = x * (m_spritesheet_image->w / m_columns);
    m_clip.y = y * (m_spritesheet_image->h / m_rows);
    m_clip.w = m_spritesheet_image->w / m_columns;
    m_clip.h = m_spritesheet_image->h / m_rows;
}

void Spritesheet::draw_selected_sprite(SDL_Surface* window_surface, SDL_Rect* position, float scale)
{
    if (scale == 1.0f) {
        SDL_BlitSurface(m_spritesheet_image, &m_clip, window_surface, position);
    }
    else {

        SDL_Rect scaled_position = *position;
        

        int scaled_width = static_cast<int>(m_clip.w * scale);
        int scaled_height = static_cast<int>(m_clip.h * scale);
        

        scaled_position.x -= (scaled_width - position->w) / 2;
        scaled_position.y -= (scaled_height - position->h) / 2;
        
        scaled_position.w = scaled_width;
        scaled_position.h = scaled_height;
        
        SDL_BlitScaled(m_spritesheet_image, &m_clip, window_surface, &scaled_position);
    }
}