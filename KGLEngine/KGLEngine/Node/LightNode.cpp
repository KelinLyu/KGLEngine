// Developed by Kelin Lyu.
#include "Node.hpp"
LightNode::LightNode(vec3 color) {
    this->engineInitializeNode();
    this->engineNodeSetLightNode(this);
    this->lightType = -1;
    this->colorFactor = color;
    this->highlightFactor = color;
    this->attenuationExponent = 0.0f;
    this->range = 0.0f;
    this->innerAngle = 0.0f;
    this->outerAngle = 0.0f;
    this->lightMask = -1;
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
void LightNode::engineConfigurateShader(Shader* shader, int index) {
    shader->setInt("lights[" + to_string(index) + "].type", this->lightType);
    shader->setVec3("lights[" + to_string(index) + "].colorFactor", this->colorFactor);
    if(this->lightType == 1) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->highlightFactor);
        shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
    }else if(this->lightType == 2) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->highlightFactor);
        shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
        shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
        shader->setFloat("lights[" + to_string(index) + "].range", this->range);
    }else if(this->lightType == 3) {
        shader->setVec3("lights[" + to_string(index) + "].highlightFactor", this->highlightFactor);
        shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
        shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
        shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
        shader->setFloat("lights[" + to_string(index) + "].range", this->range);
        shader->setFloat("lights[" + to_string(index) + "].innerAngle", cos(radians(this->innerAngle)));
        shader->setFloat("lights[" + to_string(index) + "].outerAngle", cos(radians(this->outerAngle)));
    }
}
