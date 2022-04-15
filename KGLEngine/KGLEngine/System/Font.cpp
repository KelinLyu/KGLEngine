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
            this->characters.insert(pair<char, FontCharacter*>(c, character));
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
            this->characters.insert(pair<char, FontCharacter*>(c, character));
        }
    }
    map<char, FontCharacter*>::iterator iterator;
    for(iterator = this->characters.begin(); iterator != this->characters.end(); iterator++) {
        iterator->second->size = iterator->second->size / maxHeight;
        iterator->second->bearing = iterator->second->bearing / maxHeight;
        iterator->second->advance = iterator->second->advance / maxHeight;
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
