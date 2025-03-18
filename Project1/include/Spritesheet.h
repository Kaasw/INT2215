#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>

/**
 * Tất cả sprite (hình ảnh game) được lưu trữ vào một ảnh duy nhất
 * Class này giúp lấy ra các sprite riêng từ 1 ảnh chung duy nhất đó
 */
class SpriteSheet {
private:
    std::string _path;
    SDL_Surface* _surface;
    Uint32* _pixels;
    
    void load();

public:
    static SpriteSheet* tiles;
    
    const int SIZE;
    
    SpriteSheet(const std::string& path, int size);
    ~SpriteSheet();
    
    // Hàm để lấy một sprite cụ thể từ vị trí x, y trên sprite sheet
    SDL_Surface* getSprite(int x, int y, int width, int height);
    
    // Truy cập trực tiếp vào pixel
    Uint32 getPixel(int x, int y) const;
};

#endif // SPRITE_SHEET_H