// Developed by Kelin.Lyu.
#ifndef texture_hpp
#define texture_hpp
#include "../core.hpp"
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
