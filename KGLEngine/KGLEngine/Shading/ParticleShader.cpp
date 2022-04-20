// Developed by Kelin Lyu.
#include "Shader.hpp"
ParticleShader::ParticleShader(ParticleNode* particleNode) {
    string vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec2 birthTimeAndDuration;
layout (location = 3) in vec3 initialPosition;
layout (location = 4) in vec3 initialSpeed;
layout (location = 5) in vec4 accelerationData;
layout (location = 6) in vec4 rotationData;
layout (location = 7) in vec4 scaleData;
layout (location = 8) in vec2 spriteSheetAnimationData;
out fragment_data {
    float progress;
    vec2 UV;
    vec2 spriteSheetAnimationData;
} fragment;
uniform float time;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelViewTransform;
uniform bool useLocalSpace;
void main() {
    float progress = time - birthTimeAndDuration.x;
    if(progress > birthTimeAndDuration.y) {
        gl_Position = vec4(0.0f);
        return;
    }
    float accelerationFactor = progress * progress * 0.5f;
    vec4 particlePosition = vec4(initialPosition, 1.0f);
    particlePosition.xyz += initialSpeed * progress;
    particlePosition.xyz += initialSpeed * accelerationData.w * accelerationFactor;
    particlePosition.xyz += accelerationData.xyz * accelerationFactor;
    vec4 localVertexPosition = vec4(0.0f);
    float rotation = rotationData.x + rotationData.y * progress;
    localVertexPosition.x = cos(rotation) * vertexPosition.x - sin(rotation) * vertexPosition.y;
    localVertexPosition.y = sin(rotation) * vertexPosition.x + cos(rotation) * vertexPosition.y;
    localVertexPosition.xy *= max(0.0f, scaleData.x + scaleData.y * progress);
    if(useLocalSpace) {
        gl_Position = projectionTransform * (modelViewTransform * particlePosition + localVertexPosition);
    }else{
        gl_Position = projectionTransform * (viewTransform * particlePosition + localVertexPosition);
    }
    fragment.progress = progress / birthTimeAndDuration.y;
    fragment.UV = vertexUV;
    fragment.spriteSheetAnimationData = spriteSheetAnimationData;
}
)"""";
    string fragmentShaderCode = R""""(
#version 330 core
const int ANIMATION_KEY_LIMIT = 10;
in fragment_data {
    float progress;
    vec2 UV;
    vec2 spriteSheetAnimationData;
} fragment;
out vec4 color;
struct animation_key {
    vec4 color;
    float progress;
};
uniform animation_key keys[ANIMATION_KEY_LIMIT];
uniform int keyCount;
uniform float time;
uniform bool useTextureMap;
uniform sampler2D textureMap;
uniform vec4 particleColor;
uniform bool hasSpriteSheetAnimation;
uniform int spriteSheetAnimationRows;
uniform int spriteSheetAnimationColumns;
void main() {
    if(useTextureMap) {
        if(hasSpriteSheetAnimation) {
            float data = fragment.spriteSheetAnimationData.x + fragment.progress * fragment.spriteSheetAnimationData.y;
            int frame = int(floor(data));
            if(frame >= (spriteSheetAnimationRows * spriteSheetAnimationColumns)) {
                color = vec4(0.0f);
            }else{
                float x = float(frame % spriteSheetAnimationColumns);
                float y = float(frame / spriteSheetAnimationColumns);
                vec2 UV;
                UV.x = x / spriteSheetAnimationColumns + fragment.UV.x / spriteSheetAnimationColumns;
                UV.y = y / spriteSheetAnimationRows + fragment.UV.y / spriteSheetAnimationRows;
                color = texture(textureMap, UV);
            }
        }else{
            color = texture(textureMap, fragment.UV);
        }
    }else{
        color = vec4(1.0f);
    }
    color *= particleColor;
    if(keyCount == 1) {
        color *= keys[0].color;
    }else if(keyCount > 1) {
        int i = 0;
        while(i < keyCount) {
            if(i == 0 && fragment.progress <= keys[i].progress) {
                color *= keys[i].color;
                break;
            }else if(i == (keyCount - 1) && keys[i].progress <= fragment.progress) {
                color *= keys[i].color;
                break;
            }
            if(i + 1 < keyCount) {
                if(keys[i].progress <= fragment.progress && fragment.progress <= keys[i + 1].progress) {
                    float factor = fragment.progress - keys[i].progress;
                    factor = factor / (keys[i + 1].progress - keys[i].progress);
                    color *= mix(keys[i].color, keys[i + 1].color, factor);
                    break;
                }
            }
            i += 1;
        }
    }
}
)"""";
    this->engineInitializeShader(vertexShaderCode, fragmentShaderCode);
    this->isParticleShader = true;
    this->blendMode = 2;
    this->writeToDepthBuffer = false;
    this->particleNode = particleNode;
    this->currentUseLocalSpace = -1;
    this->currentIsAdditive = -1;
    this->currentTexture = NULL;
    this->currentColor = vec4(-1.0f);
    this->colorAnimationVectorsChanged = true;
    this->spriteSheetAnimationChanged = true;
    this->hasSpriteSheetAnimation = false;
    this->spriteSheetAnimationRows = 1;
    this->spriteSheetAnimationColumns = 1;
    this->spriteSheetAnimationInitialFrameRange = 1;
    this->spriteSheetAnimationFPS = 1.0f;
    this->spriteSheetAnimationFPSVariation = 0.0f;
}
ParticleShader::~ParticleShader() {
    this->particleNode = NULL;
    this->currentTexture = NULL;
    this->colorKeys.clear();
    this->progressKeys.clear();
}
void ParticleShader::engineSetParticleShaderColorAnimation(vector<vec4> colorKeys, vector<float> progressKeys) {
    this->colorAnimationVectorsChanged = true;
    this->colorKeys = colorKeys;
    this->progressKeys = progressKeys;
}
void ParticleShader::engineSetSpriteSheetAnimation(unsigned int rows, unsigned int columns,
                                                   unsigned int initialFrameRange,
                                                   unsigned int FPS, unsigned int FPSVariation) {
    this->hasSpriteSheetAnimation = true;
    this->spriteSheetAnimationRows = rows;
    this->spriteSheetAnimationColumns = columns;
    this->spriteSheetAnimationInitialFrameRange = initialFrameRange;
    this->spriteSheetAnimationFPS = FPS;
    this->spriteSheetAnimationFPSVariation = FPSVariation;
}
void ParticleShader::engineRenderShader(Geometry* geometry) {
    if(this->currentUseLocalSpace != this->particleNode->useLocalSpace) {
        this->currentUseLocalSpace = this->particleNode->useLocalSpace;
        this->setBool("useLocalTransform", this->particleNode->useLocalSpace);
    }
    if(this->currentIsAdditive != this->particleNode->isAdditive) {
        this->currentIsAdditive = this->particleNode->isAdditive;
        if(this->particleNode->isAdditive) {
            this->blendMode = 1;
        }else{
            this->blendMode = 2;
        }
    }
    if(this->currentTexture != this->particleNode->texture) {
        this->currentTexture = this->particleNode->texture;
        if(this->particleNode->texture == NULL) {
            this->setBool("useTextureMap", false);
        }else{
            this->setBool("useTextureMap", true);
            this->setTexture("textureMap", this->particleNode->texture);
        }
    }
    if(this->currentColor != this->particleNode->color) {
        this->currentColor = this->particleNode->color;
        this->setVec4("particleColor", this->particleNode->color);
    }
    if(this->colorAnimationVectorsChanged) {
        this->colorAnimationVectorsChanged = false;
        this->setInt("keyCount", (unsigned int)this->colorKeys.size());
        for(unsigned int i = 0; i < this->colorKeys.size(); i += 1) {
            this->setVec4("keys[" + to_string(i) + "].color", this->colorKeys[i]);
            this->setFloat("keys[" + to_string(i) + "].progress", this->progressKeys[i]);
        }
    }
    if(this->spriteSheetAnimationChanged) {
        this->spriteSheetAnimationChanged = false;
        this->setBool("hasSpriteSheetAnimation", this->hasSpriteSheetAnimation);
        this->setInt("spriteSheetAnimationRows", this->spriteSheetAnimationRows);
        this->setInt("spriteSheetAnimationColumns", this->spriteSheetAnimationColumns);
    }
    this->Shader::engineRenderShader(geometry);
}
