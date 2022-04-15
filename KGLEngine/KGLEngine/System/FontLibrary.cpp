// Developed by Kelin Lyu.
#include "FontLibrary.hpp"
FontLibrary::FontLibrary() {
    this->fontLibrary = new FT_Library();
    if(FT_Init_FreeType(this->fontLibrary)) {
        cout << "\nFailed to initialize the font library!\n" << endl;
        exit(0);
    }
}
Font* FontLibrary::loadFontFile(string file, int heightResolution) {
    FT_Face font;
    string path = Engine::main->workingDirectory + file;
    if(FT_New_Face(*this->fontLibrary, path.c_str(), 0, &font)) {
        cout << "\nFailed to load the font file: " << path << "!\n" << endl;
        exit(0);
    }
    FT_Set_Pixel_Sizes(font, 0, heightResolution);
    Font* result = new Font(font);
    FT_Done_Face(font);
    return(result);
}
FontLibrary::~FontLibrary() {
    FT_Done_FreeType(*this->fontLibrary);
}
