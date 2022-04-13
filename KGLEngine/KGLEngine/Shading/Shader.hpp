// Developed by Kelin Lyu.
#ifndef Shader_hpp
#define Shader_hpp
#include "../Engine.hpp"
class Texture;
class Geometry;
class Shader final {
private:
    static int currentProgramID;
    unsigned int programID;
    unsigned int blendMode;
    vector<Texture*> textures;
    mat4 currentModelTransform;
public:
    Shader();
    Shader(string shaderFile);
    Shader(string vertexShaderCode, string fragmentShaderCode);
    void setOpaque();
    void setAdditive();
    void setSemitransparent();
    void setBool(string name, bool value);
    void setInt(string name, int value);
    void setFloat(string name, float value);
    void setVec2(string name, vec2 value);
    void setVec3(string name, vec3 value);
    void setVec4(string name, vec4 value);
    void setMat2(string name, mat2 mat);
    void setMat3(string name, mat3 mat);
    void setMat4(string name, mat4 mat);
    void setTexture(string name, Texture* texture);
    ~Shader();
    void engineInitializeShader(string vertexShaderCode, string fragmentShaderCode);
    bool engineCheckCompileErrors(unsigned int shader, string type);
    void engineActiveShader();
    void engineRenderShader(Geometry* geometry);
};
#endif
