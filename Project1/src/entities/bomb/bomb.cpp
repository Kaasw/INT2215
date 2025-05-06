//#include "Bomb.h"
//#include <SDL_image.h>
//
//Bomb::Bomb(float x, float y, int w, int h)
//    : Object(x, y, w, h, Type::BOMB)
//{
//    m_sprite = IMG_Load("assets/texture/bomb.png");
//}
//
//Bomb::~Bomb()
//{
//    if (m_sprite) SDL_FreeSurface(m_sprite);
//}
//
//void Bomb::update(float delta_time)
//{
//    m_timer -= delta_time;
//    if (m_timer < 0.f) m_timer = 0.f;
//}
//
//void Bomb::draw(SDL_Surface* surface)
//{
//    if (m_timer <= 0.f) return;  // you might draw an “explosion” sprite instead
//    SDL_Rect dst = getRect();
//    SDL_BlitSurface(m_sprite, nullptr, surface, &dst);
//}
