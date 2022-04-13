// Developed by Kelin Lyu.
#ifndef skybox_hpp
#define skybox_hpp
#include "../Engine.hpp"
class Skybox final {
private:
    unsigned int vertexArrays;
    unsigned int vertexBuffers;
    Texture* texture;
    Shader* shader;
public:
    Skybox(string right, string left,
           string top, string bottom,
           string front, string back,
           float maxAnisotropy);
    void render();
    ~Skybox();
};
#endif
