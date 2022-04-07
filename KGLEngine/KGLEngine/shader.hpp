// Developed by Kelin.Lyu.
#ifndef shader_hpp
#define shader_hpp
#include "Toolkit/common.h"
#include "core.hpp"
class Texture;
class Geometry;
class Shader final {
private:
    void checkCompileErrors(unsigned int shader, string type);
public:
    unsigned int programID;
    vector<Texture*> textures;
    vector<string> uniformNames;
    Shader(string shaderFile);
    Shader(Geometry* geometry);
    void addTexture(Texture* texture, string uniformName);
    void render(mat4 modelTransform);
    void setActivate();
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    void setVec2(const std::string &name, const vec2 &value) const;
    void setVec3(const std::string &name, const vec3 &value) const;
    void setVec4(const std::string &name, const vec4 &value) const;
    void setMat2(const std::string &name, const mat2 &mat) const;
    void setMat3(const std::string &name, const mat3 &mat) const;
    void setMat4(const std::string &name, const mat4 &mat) const;
    ~Shader();
};
#endif
