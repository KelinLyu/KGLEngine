// Developed by Kelin Lyu.
#include "Node.hpp"
UINode::UINode() {
    this->engineInitializeUINode();
}
void UINode::loadSprite(Sprite* sprite) {
    this->geometries.push_back(sprite);
}
void UINode::engineInitializeUINode() {
    this->engineInitializeNode();
    this->screenPosition = vec2(0.0f);
    this->position = vec2(0.0f);
    this->rotation = 0.0f;
    this->scale = vec2(1.0f);
    this->size = vec2(0.0f);
    this->alpha = 1.0f;
    this->renderingOrder = 0.0f;
    this->renderingTransform = mat4(0.0f);
}
void UINode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    if(this->isDisabled) {
        return;
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->renderingOrder = data.y + this->renderingOrder;
        this->geometries[i]->enginePrepareGeometryForRendering(this->renderingTransform);
    }
    vec2 newData = vec2(data.x * this->alpha, data.y + this->renderingOrder);
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->enginePrepareNodeForRendering(this->worldTransform, newData);
    }
}
void UINode::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    vec2 resolution = Engine::main->getScreenResolution();
    float minLength = glm::min(resolution.x, resolution.y);
    vec2 uiSize = this->size * this->scale * minLength;
    vec2 uiPosition = this->screenPosition * resolution + this->position * minLength;
    glm::mat4 pointTransform = glm::mat4(1.0f);
    pointTransform = glm::translate(pointTransform, vec3(uiPosition, 0.0f));
    pointTransform = glm::rotate(pointTransform, glm::radians(this->rotation), vec3(0.0f, 0.0f, 1.0f));
    pointTransform = glm::scale(pointTransform, vec3(this->scale, 1.0f));
    this->worldTransform = parentWorldTransform * pointTransform;
    mat4 transform = mat4(1.0f);
    transform = glm::translate(transform, vec3(uiPosition - uiSize * 0.5f, 0.0f));
    transform = glm::translate(transform, vec3(uiSize * 0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(this->rotation), vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, vec3(-uiSize * 0.5f, 0.0f));
    transform = glm::scale(transform, vec3(uiSize, 1.0f));
    this->renderingTransform = parentWorldTransform * transform;
}
