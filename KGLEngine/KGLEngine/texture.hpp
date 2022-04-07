// Developed by Kelin.Lyu.
#ifndef texture_hpp
#define texture_hpp
#include "Toolkit/common.h"
#include "core.hpp"
class Texture final {
public:
    unsigned int data;
    Texture(string file, GLint wrapMode, float maxAnisotropy);
    ~Texture();
};
#endif
