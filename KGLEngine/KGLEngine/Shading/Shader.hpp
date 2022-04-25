// Developed by Kelin Lyu.
#ifndef Shader_hpp
#define Shader_hpp
#include "../Engine.hpp"
class Texture;
class Geometry;
class ParticleNode;
class Shader {
protected:
    static int currentProgramID;
    static int currentBlendMode;
    static int currentWriteToDepthBuffer;
    static int currentDepthFunction;
    unsigned int programID;
    unsigned int blendMode;
    string vertexShaderSourceCode;
    string fragmentShaderSourceCode;
    vector<Texture*> textures;
    vector<string> uniformTextureNames;
    mat4 currentModelTransform;
    bool isUIShader;
    bool isParticleShader;
public:
    bool writeToDepthBuffer;
    bool clearDepthBuffer;
    Shader();
    Shader(string shaderFile);
    Shader(string vertexShaderCode, string fragmentShaderCode);
    Shader* copy();
    void activateShader();
    void setUIShader();
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
    virtual void engineRenderShader(Geometry* geometry, unsigned int renderingMode);
};
class PBRShader final: public Shader {
private:
    float currentOpacity;
    vec4 currentDiffuseColor;
    float currentDiffuseIntensity;
    float currentAlphaCutThreshold;
    float currentNormalIntensity;
    float currentHeightIntensity;
    vec2 currentHeightLayerRange;
    float currentMetallic;
    float currentMetallicIntensity;
    int currentinvertMetallic;
    float currentRoughness;
    float currentRoughnessIntensity;
    int currentinvertRoughness;
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
    bool invertMetallic;
    float roughness;
    float roughnessIntensity;
    bool invertRoughness;
    float reflectionIntensity;
    float ambientOcclusionIntensity;
    vec3 multiplyColor;
    float multiplyIntensity;
    vec3 emissionColor;
    float emissionIntensity;
    PBRShader(float metallic, float roughness);
    PBRShader(string shaderFile, float metallic, float roughness);
    PBRShader* copy();
    void setDiffuseMap(Texture* texture);
    void setNormalMap(Texture* texture);
    void setHeightMap(Texture* texture);
    void setMetallicMap(Texture* texture);
    void setRoughnessMap(Texture* texture);
    void setReflectionMap(Texture* texture);
    void setAmbientOcclusionMap(Texture* texture);
    void setMultiplyMap(Texture* texture);
    void setEmissionMap(Texture* texture);
    ~PBRShader() = default;
    void engineRenderShader(Geometry* geometry, unsigned int renderingMode) override;
};
class SpriteShader final: public Shader {
private:
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
    SpriteShader();
    ~SpriteShader();
    void engineRenderShader(Geometry* geometry, unsigned int renderingMode) override;
};
class ParticleShader final: public Shader {
private:
    ParticleNode* particleNode;
    int currentUseLocalSpace;
    int currentIsAdditive;
    Texture* currentTexture;
    vec4 currentColor;
    bool colorAnimationVectorsChanged;
    vector<vec4> colorKeys;
    vector<float> progressKeys;
    bool spriteSheetAnimationChanged;
    bool hasSpriteSheetAnimation;
    unsigned int spriteSheetAnimationRows;
    unsigned int spriteSheetAnimationColumns;
    unsigned int spriteSheetAnimationInitialFrameRange;
    float spriteSheetAnimationFPS;
    float spriteSheetAnimationFPSVariation;
public:
    ParticleShader(ParticleNode* particleNode);
    ~ParticleShader();
    void engineSetParticleShaderColorAnimation(vector<vec4> colorKeys, vector<float> progressKeys);
    void engineSetSpriteSheetAnimation(unsigned int rows, unsigned int columns,
                                       unsigned int initialFrameRange,
                                       unsigned int FPS, unsigned int FPSVariation);
    void engineRenderShader(Geometry* geometry, unsigned int renderingMode) override;
};
#endif
