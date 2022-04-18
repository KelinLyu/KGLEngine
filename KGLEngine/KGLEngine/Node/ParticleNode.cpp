// Developed by Kelin Lyu.
#include "Node.hpp"
ParticleNode::ParticleNode(int birthrate, float duration, float durationVariation) {
    this->engineInitializeNode();
    
    this->renderingOrder = 0.0f;
    
    this->renderer = new ParticleRenderer();
    this->shader = new ParticleShader();
    this->renderer->setShader(this->shader);
    
    
}


void ParticleNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    if(this->isDisabled) {
        return;
    }
    this->Node::enginePrepareNodeForRendering(parentWorldTransform, data);
    this->renderer->enginePrepareGeometryForRendering(this->worldTransform);
    
    
    this->shader->setVec3("baseWorldPosition", this->getWorldPosition());
    
    
    
    
}
