// Developed by Kelin Lyu.
#include "Texture.hpp"
Texture::Texture(string file) {
    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_2D, this->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Image* image = new Image(file);
    GLenum format = GL_RED;
    if(image->channelCount == 3) {
        format = GL_RGB;
    }else if(image->channelCount == 4) {
        format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
    delete(image);
}
Texture::Texture(string file, float maxAnisotropy, bool generateMipmaps) {
    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_2D, this->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    if(generateMipmaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(maxAnisotropy > 0.0f) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }
    Image* image = new Image(file);
    GLenum format = GL_RED;
    if(image->channelCount == 3) {
        format = GL_RGB;
    }else if(image->channelCount == 4) {
        format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
    if(generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    delete(image);
}
Texture::Texture(string right, string left,
                 string top, string bottom,
                 string front, string back,
                 float maxAnisotropy) {
    vector<string> files;
    files.push_back(right);
    files.push_back(left);
    files.push_back(top);
    files.push_back(bottom);
    files.push_back(front);
    files.push_back(back);
    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->data);
    for(unsigned int i = 0; i < files.size(); i += 1) {
        Image* image = new Image(files[i]);
        GLenum format = GL_RED;
        if(image->channelCount == 3) {
            format = GL_RGB;
        }else if(image->channelCount == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
        delete(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(maxAnisotropy > 0.0f) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }
}
Texture::Texture(int width, int height, unsigned char* buffer) {
    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_2D, this->data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}
Texture::~Texture() {
    glDeleteTextures(1, &this->data);
}
