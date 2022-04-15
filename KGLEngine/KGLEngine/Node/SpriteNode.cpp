// Developed by Kelin Lyu.
#include "Node.hpp"
SpriteNode::SpriteNode(vec2 size) {
    this->engineInitializeUINode();
    this->color = vec4(1.0f);
    this->texture = NULL;
    this->multiplyColor = vec3(1.0f);
    this->multiplyTexture = NULL;
    this->multiplyIntensity = 0.0f;
    this->emissionColor = vec3(0.0f);
    this->emissionTexture = NULL;
    this->emissionIntensity = 0.0f;
    this->size = size;
    this->spriteShader = new SpriteShader();
    Sprite* sprite = new Sprite();
    sprite->setShader(this->spriteShader);
    this->geometries.push_back(sprite);
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
void SpriteNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    if(this->isDisabled) {
        return;
    }
    this->UINode::enginePrepareNodeForRendering(parentWorldTransform, data);
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
