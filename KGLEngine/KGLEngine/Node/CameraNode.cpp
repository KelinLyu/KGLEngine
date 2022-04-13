// Developed by Kelin Lyu.
#include "Node.hpp"
CameraNode::CameraNode(float field, float near, float far) {
    this->engineInitializeNode();
    this->field = field;
    this->near = near;
    this->far = far;
}
mat4 CameraNode::getProjectionTransform() {
    vec2 resolution = Engine::main->getScreenResolution();
    float ratio = resolution.x / resolution.y;
    return(glm::perspective(this->field, ratio, this->near, this->far));
}
mat4 CameraNode::getViewTransform() {
    vec3 front = this->getFrontVectorInWorld() + this->getWorldPosition();
    vec3 up = this->getUpVectorInWorld();
    return(glm::lookAt(this->getWorldPosition(), front, up));
}
