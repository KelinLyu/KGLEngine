// Developed by Kelin.Lyu.
#define STB_IMAGE_IMPLEMENTATION
#include "Toolkit/stb_image.h"
#include "image.hpp"
Image::Image(string file) {
    this->data = stbi_load((Engine::main->programDirectory + file).c_str(),
                           &this->width, &this->height, &this->channelCount, 0);
    if(this->data == NULL) {
        cout << "\nFailed to find the image: " << file << "!" << endl;
        exit(0);
    }
}
Image::~Image() {
    stbi_image_free(this->data);
}
