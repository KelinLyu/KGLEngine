// Developed by Kelin Lyu.
#define STB_IMAGE_IMPLEMENTATION
#include "../Toolkit/stb_image.h"
#include "Image.hpp"
Image::Image(string file) {
    string path = Engine::main->workingDirectory + file;
    this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->channelCount, 0);
    if(this->data == NULL) {
        cout << "\nFailed to load the image file: " << path << "!\n" << endl;
        exit(0);
    }
}
Image::~Image() {
    stbi_image_free(this->data);
}
