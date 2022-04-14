// Developed by Kelin Lyu.
#include "Font.hpp"
Font::Font(FT_Face font) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(unsigned char c = 0; c < 128; c+= 1) {
        if(FT_Load_Char(font, c, FT_LOAD_RENDER)) {
            FontCharacter* character = new FontCharacter();
            character->texture = NULL;
            character->size = ivec2(0);
            character->bearing = ivec2(0);
            character->advance = 0;
            this->characters.insert(pair<char, FontCharacter*>(c, character));
        }else{
            Texture* glyphTexture = new Texture(font->glyph->bitmap.width,
                                                font->glyph->bitmap.rows,
                                                font->glyph->bitmap.buffer);
            ivec2 size = ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows);
            ivec2 bearing = ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top);
            unsigned int advance = (unsigned int)font->glyph->advance.x;
            FontCharacter* character = new FontCharacter();
            character->texture = glyphTexture;
            character->size = size;
            character->bearing = bearing;
            character->advance = advance;
            this->characters.insert(pair<char, FontCharacter*>(c, character));
        }
    }
}
Font::~Font() {
    map<char, FontCharacter*>::iterator iterator;
    for(iterator = this->characters.begin(); iterator != this->characters.end(); iterator++) {
        delete(iterator->second->texture);
    }
    this->characters.clear();
}
FontCharacter* Font::engineGetFontCharacter(char character) {
    return(this->characters[character]);
}
