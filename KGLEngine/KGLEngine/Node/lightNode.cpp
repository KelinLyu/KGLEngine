// Developed by Kelin.Lyu.
#include "node.hpp"
LightNode::LightNode(vec3 color) {
    this->classType = "LightNode";
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->parent = NULL;
    this->isHidden = false;
    this->type = -1;
    this->color = color;
    this->attenuationExponent = 0.0f;
    this->range = 0.0f;
    this->innerAngle = 0.0f;
    this->outerAngle = 0.0f;
}
void LightNode::setAmbientLight() {
    this->type = 0;
}
void LightNode::setDirectionalLight() {
    this->type = 1;
}
void LightNode::setPointLight(float attenuationExponent, float range) {
    this->type = 2;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
}
void LightNode::setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle) {
    this->type = 3;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
    this->innerAngle = innerAngle;
    this->outerAngle = outerAngle;
}
void LightNode::configurateShader(Shader* shader, int index, mat4 worldTransform) {
    shader->setInt("lights[" + to_string(index) + "].type", this->type);
    shader->setVec3("lights[" + to_string(index) + "].color", this->color);
    shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
    shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
    shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
    shader->setFloat("lights[" + to_string(index) + "].range", this->range);
    shader->setFloat("lights[" + to_string(index) + "].innerAngle", cos(radians(this->innerAngle)));
    shader->setFloat("lights[" + to_string(index) + "].outerAngle", cos(radians(this->outerAngle)));
}
