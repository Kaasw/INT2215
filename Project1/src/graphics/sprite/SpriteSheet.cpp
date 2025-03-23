#include "SpriteSheet.h"

Spritesheet::Spritesheet(char const* path, int row, int column)
{
    m_spritesheet_image = IMG_Load(path);

    m_clip.w = m_spritesheet_image->w / column;
    m_clip.h = m_spritesheet_image->h / row;
}

Spritesheet::~Spritesheet()
{
    SDL_FreeSurface(m_spritesheet_image);
}

void Spritesheet::select_sprite(int x, int y)
{
    m_clip.x = x * m_clip.h;
    m_clip.y = y * m_clip.w;
}

void Spritesheet::draw_selected_sprite(SDL_Surface* window_surface, SDL_Rect* position, float scale)
{
    if (scale == 1.0f) {
        SDL_BlitSurface(m_spritesheet_image, &m_clip, window_surface, position);
    }
    else {
     
        SDL_Rect scaled_position = *position;
        scaled_position.w = static_cast<int>(m_clip.w * scale);
        scaled_position.h = static_cast<int>(m_clip.h * scale);

        SDL_BlitScaled(m_spritesheet_image, &m_clip, window_surface, &scaled_position);
    }
}