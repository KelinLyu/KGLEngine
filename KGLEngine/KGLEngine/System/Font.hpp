// Developed by Kelin Lyu.
#ifndef Font_hpp
#define Font_hpp
#include "../Engine.hpp"
class Texture;
class Font final {
private:
    class Character final {
    public:
        Texture* texture;
        ivec2 size;
        ivec2 bearing;
        unsigned int advance;
        Character(Texture* texture, ivec2 size, ivec2 bearing, unsigned int advance);
    };
public:
    
    map<char, Character*> characters;
    
    Font(FT_Face font);
    ~Font();
};
#endif
