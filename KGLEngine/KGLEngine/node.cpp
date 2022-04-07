// Developed by Kelin.Lyu.
#include "node.hpp"
Node::Node() {
    this->classType = "Node";
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->parent = NULL;
    this->isHidden = false;
}
void Node::loadGeometry(string file) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(Engine::main->programDirectory + file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "\nFailed to load the 3D model file: " << file << "!" << endl;
        exit(0);
    }
    this->processNode(scene->mRootNode, scene);
}
Animator* Node::loadAnimator(string file) {
    Animator* animator = new Animator(file, this);
    this->animators.push_back(animator);
    return(animator);
}
void Node::addChild(Node* node) {
    this->children.push_back(node);
    node->parent = this;
}
void Node::updateAnimators(mat4 parentTransform, float deltaTime) {
    this->calculateWorldTransform(parentTransform);
    if(this->animators.size() > 0) {
        for(unsigned int i = 0; i < this->animators.size(); i += 1) {
            this->animators[i]->update(deltaTime);
        }
    }
    for(unsigned int i = 0; i < this->children.size(); i += 1) {
        this->children[i]->updateAnimators(this->worldTransform, deltaTime);
    }
}
void Node::updateTransform() {
    if(this->parent != NULL) {
        this->calculateWorldTransform(this->parent->getWorldTransform());
        for(unsigned int i = 0; i < this->children.size(); i += 1) {
            this->children[i]->updateTransform();
        }
    }
}
void Node::prepareForRendering(mat4 parentTransform) {
    if(this->isHidden) {
        return;
    }
    this->calculateWorldTransform(parentTransform);
    if(this->classType == "LightNode") {
        LightNode* lightNode = static_cast<LightNode*>(this);
        Engine::main->prepareLightNodeForRendering(lightNode);
    }
    for(unsigned int i = 0; i < this->children.size(); i += 1) {
        this->children[i]->prepareForRendering(this->worldTransform);
    }
}
void Node::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i += 1) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->geometries.push_back(new Geometry(mesh));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        this->processNode(node->mChildren[i], scene);
    }
}
void Node::calculateWorldTransform(mat4 parentTransform) {
    mat4 translateMatrix = glm::translate(mat4(1.0f), this->position);
    mat4 rotateMatrix = glm::eulerAngleXYZ(radians(this->eulerAngles.x),
                                           radians(this->eulerAngles.y),
                                           radians(this->eulerAngles.z));
    mat4 scaleMatrix = glm::scale(mat4(1.0f), vec3(this->scale));
    this->worldTransform = parentTransform * (translateMatrix * rotateMatrix * scaleMatrix);
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->update(this->worldTransform);
    }
}
vec3 Node::getWorldPosition() {
    vec3 worldPosition = vec3(this->worldTransform[3][0], this->worldTransform[3][1], this->worldTransform[3][2]);
    return(worldPosition);
}
mat4 Node::getWorldTransform() {
    return(this->worldTransform);
}
vec3 Node::convertLocalPositionToWorld(vec3 localPosition) {
    mat4 translateMatrix = translate(mat4(1.0f), localPosition);
    translateMatrix = this->worldTransform * translateMatrix;
    vec3 result = vec3(translateMatrix[3][0], translateMatrix[3][1], translateMatrix[3][2]);
    return(result);
}
vec3 Node::convertLocalVectorToWorld(vec3 localVector) {
    return(this->convertLocalPositionToWorld(localVector) - this->getWorldPosition());
}
vec3 Node::getFrontVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(1.0f, 0.0f, 0.0f)));
}
vec3 Node::getBackVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(-1.0f, 0.0f, 0.0f)));
}
vec3 Node::getLeftVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(0.0f, 0.0f, -1.0f)));
}
vec3 Node::getRightVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(0.0f, 0.0f, 1.0f)));
}
vec3 Node::getUpVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(0.0f, 1.0f, 0.0f)));
}
vec3 Node::getDownVectorInWorld() {
    return(this->convertLocalVectorToWorld(vec3(0.0f, -1.0f, 0.0f)));
}
void Node::setRenderingOrder(int renderingOrder) {
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->setRenderingOrder(renderingOrder);
    }
}
Node::~Node() {
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        delete(this->geometries[i]);
    }
    this->geometries.clear();
    this->children.clear();
    this->parent = NULL;
}
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
    float ratio = (float)Engine::main->screenWidth / (float)Engine::main->screenHeight;
    return(perspective(this->field, ratio, this->near, this->far));
}
mat4 CameraNode::getViewTransform() {
    return(lookAt(this->getWorldPosition(),
                  this->getFrontVectorInWorld() + this->getWorldPosition(),
                  this->getUpVectorInWorld()));
}
LightNode::LightNode(vec3 color) {
    this->classType = "LightNode";
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->parent = NULL;
    this->isHidden = false;
    this->type = -1;
    this->color = color;
    this->attenuationExponent = 0.0f;
    this->range = 0.0f;
    this->innerAngle = 0.0f;
    this->outerAngle = 0.0f;
}
void LightNode::setAmbientLight() {
    this->type = 0;
}
void LightNode::setDirectionalLight() {
    this->type = 1;
}
void LightNode::setPointLight(float attenuationExponent, float range) {
    this->type = 2;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
}
void LightNode::setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle) {
    this->type = 3;
    this->attenuationExponent = attenuationExponent;
    this->range = range;
    this->innerAngle = innerAngle;
    this->outerAngle = outerAngle;
}
void LightNode::configurateShader(Shader* shader, int index, mat4 worldTransform) {
    shader->setInt("lights[" + to_string(index) + "].type", this->type);
    shader->setVec3("lights[" + to_string(index) + "].color", this->color);
    shader->setVec3("lights[" + to_string(index) + "].position", this->getWorldPosition());
    shader->setVec3("lights[" + to_string(index) + "].direction", this->getFrontVectorInWorld());
    shader->setFloat("lights[" + to_string(index) + "].attenuationExponent", this->attenuationExponent);
    shader->setFloat("lights[" + to_string(index) + "].range", this->range);
    shader->setFloat("lights[" + to_string(index) + "].innerAngle", cos(radians(this->innerAngle)));
    shader->setFloat("lights[" + to_string(index) + "].outerAngle", cos(radians(this->outerAngle)));
}
