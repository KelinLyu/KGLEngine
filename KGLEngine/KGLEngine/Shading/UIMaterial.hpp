// Developed by Kelin Lyu.
#ifndef UIMaterial_hpp
#define UIMaterial_hpp
#include "../Engine.hpp"
class Shader;
class UIMaterial final {
private:
    Shader* shader;
    float currentAlpha;
    vec4 currentDefaultColor;
    Texture* currentTextureMap;
    vec3 currentDefaultMultiplyColor;
    Texture* currentMultiplyTextureMap;
    float currentMultiplyIntensity;
    vec3 currentDefaultEmissionColor;
    Texture* currentEmissionTextureMap;
    float currentEmissionIntensity;
public:
    float alpha;
    vec4 defaultColor;
    Texture* textureMap;
    vec3 defaultMultiplyColor;
    Texture* multiplyTextureMap;
    float multiplyIntensity;
    vec3 defaultEmissionColor;
    Texture* emissionTextureMap;
    float emissionIntensity;
    UIMaterial();
    void setAlpha();
    void setAdditive();
    ~UIMaterial();
    Shader* engineGetUIMaterialShader();
    void engineRenderUIMaterial();
};
#endif
