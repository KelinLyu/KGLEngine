// Developed by Kelin Lyu.
#ifndef Texture_hpp
#define Texture_hpp
#include "../Engine.hpp"
class Texture final {
public:
    unsigned int data;
    Texture(string file, float maxAnisotropy, bool generateMipmaps);
    Texture(string right, string left,
            string top, string bottom,
            string front, string back,
            float maxAnisotropy);
    ~Texture();
};
#endif