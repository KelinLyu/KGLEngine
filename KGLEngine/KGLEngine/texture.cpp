// Developed by Kelin.Lyu.
#include "texture.hpp"
Texture::Texture(string file, GLint wrapMode) {
    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_2D, this->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Image* image = new Image(file);
    GLenum format = GL_RED;
    if(image->channelCount == 3) {
        format = GL_RGB;
    }else if(image->channelCount == 4) {
        format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete(image);
}
Texture::~Texture() {
    glDeleteTextures(1, &this->data);
}
