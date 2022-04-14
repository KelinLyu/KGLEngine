// Developed by Kelin Lyu.
#ifndef Font_hpp
#define Font_hpp
#include "../Engine.hpp"
class Texture;
struct FontCharacter final {
public:
    Texture* texture;
    ivec2 size;
    ivec2 bearing;
    unsigned int advance;
};
class Font final {
private:
    map<char, FontCharacter*> characters;
public:
    Font(FT_Face font);
    ~Font();
    FontCharacter* engineGetFontCharacter(char character);
};
#endif
