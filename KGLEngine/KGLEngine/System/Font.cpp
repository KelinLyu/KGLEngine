// Developed by Kelin Lyu.
#include "Font.hpp"
Font::Font(FT_Face font) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    float maxHeight = 0.0f;
    for(unsigned char c = 0; c < 128; c+= 1) {
        if(FT_Load_Char(font, c, FT_LOAD_RENDER)) {
            FontCharacter* character = new FontCharacter();
            character->texture = NULL;
            character->size = vec2(0.0f);
            character->bearing = vec2(0.0f);
            character->advance = 0.0f;
            this->characters.push_back(character);
        }else{
            Texture* glyphTexture = new Texture(font->glyph->bitmap.width,
                                                font->glyph->bitmap.rows,
                                                font->glyph->bitmap.buffer);
            vec2 size = vec2(font->glyph->bitmap.width, font->glyph->bitmap.rows);
            vec2 bearing = vec2(font->glyph->bitmap_left, font->glyph->bitmap_top);
            if(size.y > maxHeight) {
                maxHeight = size.y;
            }
            unsigned int advance = (unsigned int)font->glyph->advance.x;
            FontCharacter* character = new FontCharacter();
            character->texture = glyphTexture;
            character->size = size;
            character->bearing = bearing;
            character->advance = (advance >> 6);
            this->characters.push_back(character);
        }
    }
    for(unsigned int i = 0; i < this->characters.size(); i += 1) {
        this->characters[i]->size = this->characters[i]->size / maxHeight;
        this->characters[i]->bearing = this->characters[i]->bearing / maxHeight;
        this->characters[i]->advance = this->characters[i]->advance / maxHeight;
    }
}
Font::~Font() {
    for(unsigned int i = 0; i < this->characters.size(); i += 1) {
        delete(this->characters[i]);
    }
    this->characters.clear();
}
FontCharacter* Font::engineGetFontCharacter(unsigned char character) {
    return(this->characters[character]);
}
