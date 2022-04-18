// Developed by Kelin Lyu.
#include "Node.hpp"
CameraNode::CameraNode(float field, float near, float far) {
    this->engineInitializeNode();
    this->field = glm::radians(field);
    this->near = near;
    this->far = far;
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
