// Developed by Kelin Lyu.
#include "Node.hpp"
UINode::UINode() {
    this->engineInitializeUINode();
    this->currentUINode = this;
}
Node* UINode::copy() {
    UINode* node = new UINode();
    node->screenPosition = this->screenPosition;
    node->position = this->position;
    node->rotation = this->rotation;
    node->scale = this->scale;
    node->size = this->size;
    node->alpha = this->alpha;
    node->renderingOrder = this->renderingOrder;
    node->name = this->name;
    node->isDisabled = this->isDisabled;
    node->renderingBitMask = this->renderingBitMask;
    node->Node::position = this->Node::position;
    node->Node::eulerAngles = this->Node::eulerAngles;
    node->Node::scale = this->Node::scale;
    node->orientationTargetNode = this->orientationTargetNode;
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        node->geometries.push_back(this->geometries[i]->copy(&node->animators));
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        node->addChildNode(this->childNodes[i]->copy());
    }
    return(node);
}
Node* UINode::clone() {
    return(this->copy());
}
void UINode::loadSprite(Sprite* sprite) {
    this->geometries.push_back(sprite);
}
vec2 UINode::convertScreenPositionToLocal(vec2 screenPosition) {
    vec2 resolution = Engine::main->getScreenResolution();
    float minLength = glm::min(resolution.x, resolution.y);
    mat4 transform = glm::translate(glm::mat4(1.0f), vec3(screenPosition * resolution, 0.0f));
    vec3 location = glm_helper::getPosition(glm::inverse(this->getWorldTransform()) * transform);
    return(vec2(location.x / minLength, location.y / minLength));
}
bool UINode::checkSizeIncludesScreenPosition(vec2 screenPosition) {
    vec2 localPosition = this->convertScreenPositionToLocal(screenPosition);
    if(-this->size.x * 0.5f < localPosition.x && localPosition.x < this->size.x * 0.5f) {
        if(-this->size.y * 0.5f < localPosition.y && localPosition.y < this->size.y * 0.5f) {
            return(true);
        }
    }
    return(false);
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
void UINode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data, unsigned int renderingMode) {
    if(this->isDisabled) {
        return;
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
    if((this->renderingBitMask & Engine::main->mainCameraNode->renderingBitMask) > 0 && renderingMode == 0) {
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            this->geometries[i]->renderingOrder = data.y + this->renderingOrder;
            this->geometries[i]->enginePrepareGeometryForRendering(this->renderingTransform);
        }
    }
    vec2 newData = vec2(data.x * this->alpha, data.y + this->renderingOrder);
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->enginePrepareNodeForRendering(this->worldTransform, newData, renderingMode);
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
