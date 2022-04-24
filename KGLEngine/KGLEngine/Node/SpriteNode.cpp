// Developed by Kelin Lyu.
#include "Node.hpp"
SpriteNode::SpriteNode(vec2 size) {
    this->engineInitializeUINode();
    this->currentSpriteNode = this;
    this->color = vec4(1.0f);
    this->texture = NULL;
    this->multiplyColor = vec3(1.0f);
    this->multiplyTexture = NULL;
    this->multiplyIntensity = 1.0f;
    this->emissionColor = vec3(0.0f);
    this->emissionTexture = NULL;
    this->emissionIntensity = 1.0f;
    this->size = size;
    this->spriteShader = new SpriteShader();
    Sprite* sprite = new Sprite();
    sprite->setShader(this->spriteShader);
    this->geometries.push_back(sprite);
}
Node* SpriteNode::copy() {
    SpriteNode* node = new SpriteNode(this->size);
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
    node->color = this->color;
    node->texture = this->texture;
    node->multiplyColor = this->multiplyColor;
    node->multiplyTexture = this->multiplyTexture;
    node->multiplyIntensity = this->multiplyIntensity;
    node->emissionColor = this->emissionColor;
    node->emissionTexture = this->emissionTexture;
    node->emissionIntensity = this->emissionIntensity;
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        node->geometries.push_back(this->geometries[i]->copy(&node->animators));
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        node->addChildNode(this->childNodes[i]->copy());
    }
    return(node);
}
Node* SpriteNode::clone() {
    return(this->copy());
}
void SpriteNode::setSemitransparent() {
    this->spriteShader->setSemitransparent();
}
void SpriteNode::setAdditive() {
    this->spriteShader->setAdditive();
}
SpriteNode::~SpriteNode() {
    delete(this->spriteShader);
    this->spriteShader = NULL;
    this->texture = NULL;
    this->multiplyTexture = NULL;
    this->emissionTexture = NULL;
}
void SpriteNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data, bool shadowMap) {
    if(this->isDisabled) {
        return;
    }
    this->UINode::enginePrepareNodeForRendering(parentWorldTransform, data, shadowMap);
    if(shadowMap) {
        return;
    }
    this->spriteShader->alpha = this->alpha * data.x;
    this->spriteShader->defaultColor = this->color;
    this->spriteShader->textureMap = this->texture;
    this->spriteShader->defaultMultiplyColor = this->multiplyColor;
    this->spriteShader->multiplyTextureMap = this->multiplyTexture;
    this->spriteShader->multiplyIntensity = this->multiplyIntensity;
    this->spriteShader->defaultEmissionColor = this->emissionColor;
    this->spriteShader->emissionTextureMap = this->emissionTexture;
    this->spriteShader->emissionIntensity = this->emissionIntensity;
}
void SpriteNode::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    this->UINode::engineCalculateNodeWorldTransform(parentWorldTransform);
}
