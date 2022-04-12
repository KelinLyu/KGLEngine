// Developed by Kelin.Lyu.
#ifndef material_hpp
#define material_hpp
#include "../core.hpp"
class Shader;
class Material final {
public:
    Shader* shader;
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
    vec4 multiplyColor;
    float multiplyIntensity;
    vec4 emissionColor;
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
    void render();
    ~Material();
};
#endif
