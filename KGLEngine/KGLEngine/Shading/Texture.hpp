// Developed by Kelin Lyu.
#ifndef Texture_hpp
#define Texture_hpp
#include "../Engine.hpp"
class Texture final {
public:
    unsigned int data;
    Texture() = default;
    Texture(string file);
    Texture(string file, float maxAnisotropy, bool generateMipmaps);
    Texture(string right, string left,
            string top, string bottom,
            string front, string back,
            float maxAnisotropy);
    Texture(int width, int height, unsigned char* buffer);
    ~Texture();
};
#endif
