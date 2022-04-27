// Developed by Kelin Lyu.
#ifndef AudioBuffer_hpp
#define AudioBuffer_hpp
#include "../Engine.hpp"
class AudioBuffer final {
private:
    vector<SoundBuffer> buffers;
    vector<unsigned int> previousBufferIndices;
public:
    AudioBuffer(string file);
    AudioBuffer(string file, string extension, unsigned int startIndex, unsigned int endIndex);
    ~AudioBuffer();
    SoundBuffer* engineGetAudioBuffer();
};
#endif
