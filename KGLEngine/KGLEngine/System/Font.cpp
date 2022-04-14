// Developed by Kelin Lyu.
#include "Font.hpp"
Font::Character::Character(Texture* texture, ivec2 size, ivec2 bearing, unsigned int advance) {
    this->texture = texture;
    this->size = size;
    this->bearing = bearing;
    this->advance = advance;
}
Font::Font(FT_Face font) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(unsigned char c = 0; c < 128; c+= 1) {
        if(FT_Load_Char(font, c, FT_LOAD_RENDER)) {
            Character* character = new Character(NULL, ivec2(0), ivec2(0), 0);
            this->characters.insert(pair<char, Character*>(c, character));
        }else{
            Texture* glyphTexture = new Texture(font->glyph->bitmap.width,
                                                font->glyph->bitmap.rows,
                                                font->glyph->bitmap.buffer);
            ivec2 size = ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows);
            ivec2 bearing = ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top);
            unsigned int advance = (unsigned int)font->glyph->advance.x;
            Character* character = new Character(glyphTexture, size, bearing, advance);
            this->characters.insert(pair<char, Character*>(c, character));
        }
    }
}
Font::~Font() {
    map<char, Character*>::iterator iterator;
    for(iterator = this->characters.begin(); iterator != this->characters.end(); iterator++) {
        delete(iterator->second->texture);
    }
    this->characters.clear();
}
