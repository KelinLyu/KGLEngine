// Developed by Kelin Lyu.
#ifndef Image_hpp
#define Image_hpp
#include "../Engine.hpp"
class Image final {
public:
    unsigned char* data;
    int width;
    int height;
    int channelCount;
    Image(string file);
    ~Image();
};
#endif
