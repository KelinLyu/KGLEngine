// Developed by Kelin Lyu.
#include "Node.hpp"
LightNode::LightNode(vec3 color) {
    this->engineInitializeNode();
    this->currentLightNode = this;
    this->lightType = -1;
    this->cameraNodeDistance = 0.0f;
    this->colorFactor = color;
    this->highlightIntensity = 1.0f;
    this->attenuationExponent = 0.0f;
    this->range = 0.0f;
    this->penetrationRange = 0.0f;
    this->innerAngle = 0.0f;
    this->outerAngle = 0.0f;
    this->lightingBitMask = -1;
    this->hasDirectionalLightShadow = false;
}
Node* LightNode::copy() {
    LightNode* node = new LightNode(this->colorFactor);
    node->lightType = this->lightType;
    node->highlightIntensity = this->highlightIntensity;
    node->attenuationExponent = this->attenuationExponent;
    node->range = this->range;
    node->penetrationRange = this->penetrationRange;
    node->innerAngle = this->innerAngle;
    node->outerAngle = this->outerAngle;
    node->lightingBitMask = this->lightingBitMask;
    node->name = this->name;
    node->isDisabled = this->isDisabled;
    node->renderingBitMask = this->renderingBitMask;
    node->position = this->position;
    node->eulerAngles = this->eulerAngles;
    node->scale = this->scale;
    node->orientationTargetNode = this->orientationTargetNode;
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        node->animators.push_back(this->animators[i]->engineCopyAnimator());
    }
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        node->geometries.push_back(this->geometries[i]->copy(&node->animators));
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        Node* newNode = this->childNodes[i]->copy();
        node->addChildNode(newNode);
        map<string, Node*>::iterator iterator;
        for(iterator = this->boneNodes.begin(); iterator != this->boneNodes.end(); iterator++) {
            if(iterator->second == this->childNodes[i]) {
                node->boneNodes[iterator->first] = newNode;
                break;
            }
        }
    }
    return(node);
}
Node* LightNode::clone() {
    return(this->copy());
}
void LightNode::setAmbientLight() {
    this->lightType = 0;
}
void LightNode::setDirectionalLight() {
    this->lightType = 1;
}
void LightNode::setPointLight(float attenuationExponent, float range) {
    this->lightType = 2;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
}
void LightNode::setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle) {
    this->lightType = 3;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
    this->innerAngle = innerAngle;
    this->outerAngle = outerAngle;
}
void LightNode::activateDirectionalLightShadow(unsigned int mapSize, float projectionSize, float near, float far, float xOffset, float bias, int samples) {
    this->hasDirectionalLightShadow = true;
    this->shadowMapSize = mapSize;
    glGenFramebuffers(1, &this->shadowBuffer);
    unsigned int map;
    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_2D, map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mapSize, mapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->shadowMap = new Texture();
    this->shadowMap->data = map;
    this->directionalLightCameraNode = new CameraNode(projectionSize, projectionSize, near, far);
    this->directionalLightCameraNode->position.x = xOffset;
    this->addChildNode(this->directionalLightCameraNode);
    this->shadowBias = bias;
    this->shadowSamples = samples;
}
void LightNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data, unsigned int renderingMode) {
    if(this->isDisabled) {
        return;
    }
    this->Node::enginePrepareNodeForRendering(parentWorldTransform, data, renderingMode);
    if(renderingMode > 0) {
        return;
    }
    if((this->renderingBitMask & Engine::main->mainCameraNode->renderingBitMask) > 0) {
        if(this->lightType < 2) {
            this->cameraNodeDistance = 0;
        }else{
            this->cameraNodeDistance = glm::length(Engine::main->mainCameraNode->getWorldPosition() - this->getWorldPosition());
        }
        for(unsigned int i = 0; i < Engine::main->preparedLightNodes.size(); i += 1) {
            if(Engine::main->preparedLightNodes[i]->cameraNodeDistance > this->cameraNodeDistance) {
                Engine::main->preparedLightNodes.insert(Engine::main->preparedLightNodes.begin() + i, this);
                this->enginePrepareLightShadowForRendering();
                return;
            }
        }
        Engine::main->preparedLightNodes.push_back(this);
        this->enginePrepareLightShadowForRendering();
    }
}
void LightNode::enginePrepareLightShadowForRendering() {
    if(Engine::main->preparedLightNodeShadows.size() >= SHADOWS_LIMIT) {
        this->shadowIndex = -1;
        return;
    }
    if(this->lightType == 1 && this->hasDirectionalLightShadow) {
        this->shadowIndex = (unsigned int)Engine::main->preparedLightNodeShadows.size();
        Engine::main->preparedLightNodeShadows.push_back(this);
    }else{
        this->shadowIndex = -1;
    }
}
unsigned int LightNode::engineGetLightType() {
    return(this->lightType);
}
CameraNode* LightNode::engineGetDirectionalLightCameraNode() {
    return(this->directionalLightCameraNode);
}
unsigned int LightNode::engineLightNodeGetShadowMapSize() {
    return(this->shadowMapSize);
}
unsigned int LightNode::engineLightNodeGetShadowBuffer() {
    return(this->shadowBuffer);
}
void LightNode::engineConfigurateLightForShader(Shader* shader, int index) {
    shader->setInt("lights[" + to_string(index) + "].type", this->lightType);
    shader->setVec3("lights[" + to_string(index) + "].colorFactor", this->colorFactor);
    if(this->lightType == 1) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->colorFactor * this->highlightIntensity);
        shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
        shader->setInt("lights[" + to_string(index) + "].shadowIndex", this->shadowIndex);
    }else if(this->lightType == 2) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->colorFactor * this->highlightIntensity);
        shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
        shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
        shader->setFloat("lights[" + to_string(index) + "].range", this->range);
        shader->setFloat("lights[" + to_string(index) + "].penetrationRange", this->penetrationRange);
        shader->setInt("lights[" + to_string(index) + "].shadowIndex", this->shadowIndex);
    }else if(this->lightType == 3) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->colorFactor * this->highlightIntensity);
        shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
        shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
        shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
        shader->setFloat("lights[" + to_string(index) + "].range", this->range);
        shader->setFloat("lights[" + to_string(index) + "].penetrationRange", this->penetrationRange);
        shader->setFloat("lights[" + to_string(index) + "].innerAngle", glm::cos(glm::radians(this->innerAngle)));
        shader->setFloat("lights[" + to_string(index) + "].outerAngle", glm::cos(glm::radians(this->outerAngle)));
        shader->setInt("lights[" + to_string(index) + "].shadowIndex", this->shadowIndex);
    }
}
void LightNode::engineConfigurateShadowForShader(Shader* shader, int index) {
    if(this->lightType == 1) {
        shader->setInt("shadows[" + to_string(index) + "].type", 0);
    }
    shader->setTexture("shadows[" + to_string(index) + "].shadowMap", this->shadowMap);
    shader->setMat4("shadows[" + to_string(index) + "].lightSpaceMatrix", this->directionalLightCameraNode->getDirectionalLightSpaceMatrix());
    shader->setFloat("shadows[" + to_string(index) + "].bias", this->shadowBias);
    shader->setInt("shadows[" + to_string(index) + "].samples", this->shadowSamples);
}
