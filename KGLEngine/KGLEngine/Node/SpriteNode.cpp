// Developed by Kelin Lyu.
#include "Node.hpp"
SpriteNode::SpriteNode(vec2 size) {
    this->engineInitializeUINode();
    this->material = NULL;
    this->color = vec4(1.0f);
    this->texture = NULL;
    this->multiplyColor = vec3(1.0f);
    this->multiplyTexture = NULL;
    this->multiplyIntensity = 0.0f;
    this->emissionColor = vec3(0.0f);
    this->emissionTexture = NULL;
    this->emissionIntensity = 0.0f;
    this->size = size;
    this->material = new UIMaterial();
    Sprite* sprite = new Sprite();
    sprite->setUIMaterial(material);
    this->geometries.push_back(sprite);
}
void SpriteNode::setAlpha() {
    this->material->setAlpha();
}
void SpriteNode::setAdditive() {
    this->material->setAdditive();
}
SpriteNode::~SpriteNode() {
    this->material = NULL;
    this->texture = NULL;
    this->multiplyTexture = NULL;
    this->emissionTexture = NULL;
}
void SpriteNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    this->UINode::enginePrepareNodeForRendering(parentWorldTransform, data);
    this->material->alpha = this->alpha * data.x;
    this->material->defaultColor = this->color;
    this->material->textureMap = this->texture;
    this->material->defaultMultiplyColor = this->multiplyColor;
    this->material->multiplyTextureMap = this->multiplyTexture;
    this->material->multiplyIntensity = this->multiplyIntensity;
    this->material->defaultEmissionColor = this->emissionColor;
    this->material->emissionTextureMap = this->emissionTexture;
    this->material->emissionIntensity = this->emissionIntensity;
}
void SpriteNode::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    this->UINode::engineCalculateNodeWorldTransform(parentWorldTransform);
}
