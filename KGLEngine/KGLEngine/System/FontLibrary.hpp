// Developed by Kelin Lyu.
#ifndef FontLibrary_hpp
#define FontLibrary_hpp
#include "../Engine.hpp"
class Font;
class FontLibrary final {
private:
    FT_Library* fontLibrary;
public:
    FontLibrary();
    Font* loadFontFile(string file, int heightResolution);
    ~FontLibrary();
};
#endif
