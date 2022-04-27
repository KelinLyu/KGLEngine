// Developed by Kelin Lyu.
#include "AudioBuffer.hpp"
AudioBuffer::AudioBuffer(string file) {
    SoundBuffer buffer;
    if(!buffer.loadFromFile(Engine::main->workingDirectory + file)) {
        cout << "\nFailed to load the audio file: "
        << Engine::main->workingDirectory + file << "!\n" << endl;
        exit(1);
    }
    this->buffers.push_back(buffer);
}
AudioBuffer::AudioBuffer(string file, string extension, unsigned int startIndex, unsigned int endIndex) {
    for(unsigned int i = startIndex; i <= endIndex; i += 1) {
        SoundBuffer buffer;
        if(!buffer.loadFromFile(Engine::main->workingDirectory + file + to_string(i) + "." + extension)) {
            cout << "\nFailed to load the audio file: "
            << Engine::main->workingDirectory + file + to_string(i) + "." + extension << "!\n" << endl;
            exit(1);
        }
        this->buffers.push_back(buffer);
    }
}
AudioBuffer::~AudioBuffer() {
    this->buffers.clear();
}
SoundBuffer* AudioBuffer::engineGetAudioBuffer() {
    if(this->buffers.size() == 1) {
        return(&this->buffers[0]);
    }
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < this->buffers.size(); i += 1) {
        bool found = false;
        for(unsigned int j = 0; j < this->previousBufferIndices.size(); j += 1) {
            if(i == this->previousBufferIndices[j]) {
                found = true;
                break;
            }
        }
        if(!found) {
            indices.push_back(i);
        }
    }
    unsigned int i = glm::linearRand<unsigned int>(0, (unsigned int)indices.size() - 1);
    this->previousBufferIndices.insert(this->previousBufferIndices.begin(), indices[i]);
    while(this->previousBufferIndices.size() > this->buffers.size() / 2) {
        this->previousBufferIndices.pop_back();
    }
    indices.clear();
    return(&this->buffers[indices[i]]);
}
