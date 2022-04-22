// Developed by Kelin Lyu.
#ifndef Font_hpp
#define Font_hpp
#include "../Engine.hpp"
class Texture;
struct FontCharacter final {
public:
    Texture* texture;
    vec2 size;
    vec2 bearing;
    float advance;
};
class Font final {
private:
    vector<FontCharacter*> characters;
public:
    Font(FT_Face font);
    ~Font();
    FontCharacter* engineGetFontCharacter(unsigned char character);
};
#endif
