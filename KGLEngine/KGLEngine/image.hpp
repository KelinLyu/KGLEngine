// Developed by Kelin.Lyu.
#ifndef image_hpp
#define image_hpp
#include "Toolkit/common.h"
#include "core.hpp"
class Image final {
public:
    unsigned char* data;
    int width, height;
    int channelCount;
    Image(string file);
    ~Image();
};
#endif
