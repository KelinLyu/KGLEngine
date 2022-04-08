// Developed by Kelin.Lyu.
#include "node.hpp"
CameraNode::CameraNode(float field, float near, float far) {
    this->classType = "CameraNode";
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->parent = NULL;
    this->isHidden = false;
    this->field = field;
    this->near = near;
    this->far = far;
}
mat4 CameraNode::getProjectionTransform() {
    vec2 resolution = Engine::main->getScreenResolution();
    float ratio = (float)resolution.x / (float)resolution.y;
    return(perspective(this->field, ratio, this->near, this->far));
}
mat4 CameraNode::getViewTransform() {
    return(lookAt(this->getWorldPosition(),
                  this->getFrontVectorInWorld() + this->getWorldPosition(),
                  this->getUpVectorInWorld()));
}
