// Developed by Kelin Lyu.
#include "Shader.hpp"
SpriteShader::SpriteShader() {
    string vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec4 vertexData;
out vec2 UV;
uniform mat4 modelProjectionTransform;
void main() {
    UV = vertexData.zw;
    gl_Position = modelProjectionTransform * vec4(vertexData.xy, 0.0f, 1.0f);
}
)"""";
    string fragmentShaderCode = R""""(
#version 330 core
in vec2 UV;
out vec4 color;
uniform float alpha;
uniform vec4 defaultColor;
uniform bool useTextureMap;
uniform sampler2D textureMap;
uniform vec3 defaultMultiplyColor;
uniform bool useMultiplyTextureMap;
uniform sampler2D multiplyTextureMap;
uniform float multiplyIntensity;
uniform vec3 defaultEmissionColor;
uniform bool useEmissionTextureMap;
uniform sampler2D emissionTextureMap;
uniform float emissionIntensity;
void main() {
    color = defaultColor;
    if(useTextureMap) {
        color = texture(textureMap, UV);
    }
    vec3 multiplyColor = defaultMultiplyColor;
    if(useMultiplyTextureMap) {
        multiplyColor = texture(multiplyTextureMap, UV).rgb;
    }
    vec3 inverseMultiplyColor = vec3(1.0f) - multiplyColor;
    multiplyColor = vec3(1.0f) - inverseMultiplyColor * multiplyIntensity;
    color.rgb *= multiplyColor;
    vec3 emissionColor = defaultEmissionColor;
    if(useEmissionTextureMap) {
        emissionColor = texture(emissionTextureMap, UV).rgb;
    }
    emissionColor *= emissionIntensity;
    color.rgb += emissionColor;
    color.a *= alpha;
}
)"""";
    this->engineInitializeShader(vertexShaderCode, fragmentShaderCode);
    this->setUIShader();
    this->currentAlpha = -1.0f;
    this->currentDefaultColor = vec4(-1.0f);
    this->currentTextureMap = NULL;
    this->currentDefaultMultiplyColor = vec3(-1.0f);
    this->currentMultiplyTextureMap = NULL;
    this->currentMultiplyIntensity = -1.0f;
    this->currentDefaultEmissionColor = vec3(-1.0f);
    this->currentEmissionTextureMap = NULL;
    this->currentEmissionIntensity = -1.0f;
    this->setBool("useTextureMap", false);
    this->setBool("useMultiplyTextureMap", false);
    this->setBool("useEmissionTextureMap", false);
    this->alpha = 1.0f;
    this->defaultColor = vec4(1.0f);
    this->textureMap = NULL;
    this->defaultMultiplyColor = vec3(1.0f);
    this->multiplyTextureMap = NULL;
    this->multiplyIntensity = 0.0f;
    this->defaultEmissionColor = vec3(0.0f);
    this->emissionTextureMap = NULL;
    this->emissionIntensity = 0.0f;
}
SpriteShader::~SpriteShader() {
    this->currentTextureMap = NULL;
    this->currentMultiplyTextureMap = NULL;
    this->currentEmissionTextureMap = NULL;
    this->textureMap = NULL;
    this->multiplyTextureMap = NULL;
    this->emissionTextureMap = NULL;
    delete(this);
}
void SpriteShader::engineRenderShader(Geometry *geometry, bool shadowMap) {
    if(this->currentAlpha != this->alpha) {
        this->currentAlpha = this->alpha;
        this->setFloat("alpha", this->alpha);
    }
    if(this->currentDefaultColor != this->defaultColor) {
        this->currentDefaultColor = this->defaultColor;
        this->setVec4("defaultColor", this->defaultColor);
    }
    if(this->currentTextureMap != this->textureMap) {
        this->currentTextureMap = this->textureMap;
        if(this->textureMap != NULL) {
            this->setBool("useTextureMap", true);
            this->setTexture("textureMap", this->textureMap);
        }else{
            this->setBool("useTextureMap", false);
        }
    }
    if(this->currentDefaultMultiplyColor != this->defaultMultiplyColor) {
        this->currentDefaultMultiplyColor = this->defaultMultiplyColor;
        this->setVec3("defaultMultiplyColor", this->defaultMultiplyColor);
    }
    if(this->currentMultiplyTextureMap != this->multiplyTextureMap) {
        this->currentMultiplyTextureMap = this->multiplyTextureMap;
        if(this->multiplyTextureMap != NULL) {
            this->setBool("useMultiplyTextureMap", true);
            this->setTexture("multiplyTextureMap", this->multiplyTextureMap);
        }else{
            this->setBool("useMultiplyTextureMap", false);
        }
    }
    if(this->currentMultiplyIntensity != this->multiplyIntensity) {
        this->currentMultiplyIntensity = this->multiplyIntensity;
        this->setFloat("multiplyIntensity", this->multiplyIntensity);
    }
    if(this->currentDefaultEmissionColor != this->defaultEmissionColor) {
        this->currentDefaultEmissionColor = this->defaultEmissionColor;
        this->setVec3("defaultEmissionColor", this->defaultEmissionColor);
    }
    if(this->currentEmissionTextureMap != this->emissionTextureMap) {
        this->currentEmissionTextureMap = this->emissionTextureMap;
        if(this->multiplyTextureMap != NULL) {
            this->setBool("useEmissionTextureMap", true);
            this->setTexture("emissionTextureMap", this->emissionTextureMap);
        }else{
            this->setBool("useEmissionTextureMap", false);
        }
    }
    if(this->currentEmissionIntensity != this->emissionIntensity) {
        this->currentEmissionIntensity = this->emissionIntensity;
        this->setFloat("emissionIntensity", this->emissionIntensity);
    }
    this->Shader::engineRenderShader(geometry, shadowMap);
}
