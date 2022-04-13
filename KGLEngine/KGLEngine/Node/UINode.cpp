// Developed by Kelin Lyu.
#include "Node.hpp"
UINode::UINode() {
    this->engineInitializeUINode();
}
UINode::UINode(vec2 size) {
    this->engineInitializeUINode();
    this->size = size;
    this->material = new UIMaterial();
    Sprite* sprite = new Sprite();
    sprite->setUIMaterial(material);
    this->geometries.push_back(sprite);
}
void UINode::loadSprite(Sprite* sprite) {
    this->geometries.push_back(sprite);
}
void UINode::engineInitializeUINode() {
    this->engineInitializeNode();
    this->engineNodeSetUINode(this);
    this->material = NULL;
    this->screenPosition = vec2(0.0f);
    this->position = vec2(0.0f);
    this->rotation = 0.0f;
    this->scale = vec2(1.0f);
    this->size = vec2(0.0f);
    this->alpha = 1.0f;
    this->color = vec4(1.0f);
    this->texture = NULL;
    this->multiplyColor = vec3(1.0f);
    this->multiplyTexture = NULL;
    this->multiplyIntensity = 0.0f;
    this->emissionColor = vec3(0.0f);
    this->emissionTexture = NULL;
    this->emissionIntensity = 0.0f;
    this->renderingOrder = 0.0f;
}
void UINode::setAlpha() {
    if(this->material != NULL) {
        this->material->setAlpha();
    }
}
void UINode::setAdditive() {
    if(this->material != NULL) {
        this->material->setAdditive();
    }
}
UINode::~UINode() {
    this->material = NULL;
    this->texture = NULL;
    this->multiplyTexture = NULL;
    this->emissionTexture = NULL;
}
void UINode::enginePrepareUINodeForRendering() {
    if(this->geometries.size() > 0) {
        float baseRenderingOrder = 0.0f;
        if(this->parent != NULL) {
            baseRenderingOrder = this->parent->engineGetUINodeRenderingOrder();
        }
        this->cumulativeRenderingOrder = baseRenderingOrder + this->renderingOrder;
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            this->geometries[i]->renderingOrder = this->cumulativeRenderingOrder;
        }
    }
    if(this->material != NULL) {
        this->material->alpha = this->alpha;
        this->material->defaultColor = this->color;
        this->material->textureMap = this->texture;
        this->material->defaultMultiplyColor = this->multiplyColor;
        this->material->multiplyTextureMap = this->multiplyTexture;
        this->material->multiplyIntensity = this->multiplyIntensity;
        this->material->defaultEmissionColor = this->emissionColor;
        this->material->emissionTextureMap = this->emissionTexture;
        this->material->emissionIntensity = this->emissionIntensity;
    }
}
float UINode::engineGetUINodeCumulativeRenderingOrder() {
    return(this->cumulativeRenderingOrder);
}
