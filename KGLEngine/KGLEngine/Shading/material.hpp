// Developed by Kelin.Lyu.
#ifndef material_hpp
#define material_hpp
#include "../core.hpp"
class Shader;
class Material final {
public:
    Shader* shader;
    Material();
};
#endif
