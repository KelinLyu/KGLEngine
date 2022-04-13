// Developed by Kelin Lyu.
#ifndef Material_hpp
#define Material_hpp
#include "../Engine.hpp"
class Shader;
class Material final {
private:
    Shader* shader;
    float currentOpacity;
    vec4 currentDiffuseColor;
    float currentDiffuseIntensity;
    float currentAlphaCutThreshold;
    float currentNormalIntensity;
    float currentHeightIntensity;
    vec2 currentHeightLayerRange;
    float currentMetallic;
    float currentMetallicIntensity;
    int currentReverseMetallic;
    float currentRoughness;
    float currentRoughnessIntensity;
    int currentReverseRoughness;
    float currentReflectionIntensity;
    float currentAmbientOcclusionIntensity;
    vec3 currentMultiplyColor;
    float currentMultiplyIntensity;
    vec3 currentEmissionColor;
    float currentEmissionIntensity;
public:
    float opacity;
    vec4 diffuseColor;
    float diffuseIntensity;
    float alphaCutThreshold;
    float normalIntensity;
    float heightIntensity;
    vec2 heightLayerRange;
    float metallic;
    float metallicIntensity;
    bool reverseMetallic;
    float roughness;
    float roughnessIntensity;
    bool reverseRoughness;
    float reflectionIntensity;
    float ambientOcclusionIntensity;
    vec3 multiplyColor;
    float multiplyIntensity;
    vec3 emissionColor;
    float emissionIntensity;
    Material(float metallic, float roughness);
    void setOpaque();
    void setAdditive();
    void setSemitransparent();
    void setDiffuseMap(Texture* texture);
    void setNormalMap(Texture* texture);
    void setHeightMap(Texture* texture);
    void setMetallicMap(Texture* texture);
    void setRoughnessMap(Texture* texture);
    void setReflectionMap(Texture* texture);
    void setAmbientOcclusionMap(Texture* texture);
    void setMultiplyMap(Texture* texture);
    void setEmissionMap(Texture* texture);
    ~Material();
    Shader* engineGetMaterialShader();
    void engineRenderMaterial();
};
#endif
