// Developed by Kelin Lyu.
#include "Node.hpp"
CameraNode::CameraNode(float field, float near, float far) {
    this->engineInitializeNode();
    this->currentCameraNode = this;
    this->width = 0.0f;
    this->height = 0.0f;
    this->field = glm::radians(field);
    this->near = near;
    this->far = far;
    this->renderingBitMask = -1;
}
CameraNode::CameraNode(float width, float height, float near, float far) {
    this->engineInitializeNode();
    this->currentCameraNode = this;
    this->width = width;
    this->height = height;
    this->field = 0.0f;
    this->near = near;
    this->far = far;
    this->renderingBitMask = -1;
}
Node* CameraNode::copy() {
    CameraNode* node = new CameraNode(this->field, this->near, this->far);
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
Node* CameraNode::clone() {
    return(this->copy());
}
mat4 CameraNode::getDirectionalLightSpaceMatrix() {
    mat4 lightProjection = glm::ortho(-this->width * 0.5f, this->width * 0.5f,
                                      -this->height * 0.5f, this->height * 0.5f,
                                      this->near, this->far);
    return(lightProjection * this->getViewTransform());
}
mat4 CameraNode::getOrthogonalProjectionTransform() {
    vec2 resolution = Engine::main->getScreenResolution();
    return(glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f));
}
mat4 CameraNode::getProjectionTransform() {
    vec2 resolution = Engine::main->getScreenResolution();
    float ratio = resolution.x / resolution.y;
    return(glm::perspective(this->field, ratio, this->near, this->far));
}
mat4 CameraNode::getViewTransform() {
    vec3 front = this->getFrontVectorInWorld() + this->getWorldPosition();
    return(glm::lookAt(this->getWorldPosition(), front, vec3(0.0f, 1.0f, 0.0f)));
}
