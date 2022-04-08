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
void Node::loadUnitCube() {
    this->geometries.push_back(new UnitCube());
}
void Node::loadGeometry(string file) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(Engine::main->getProgramDirectory() + file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "\nFailed to load the 3D model file: "
             << Engine::main->getProgramDirectory() + file << "!\n" << endl;
        exit(0);
    }
    this->processNode(scene->mRootNode, scene);
}
Animator* Node::loadAnimator(string file) {
    Animator* animator = new Animator(file, this);
    this->animators.push_back(animator);
    return(animator);
}
void Node::setShader(Shader* shader, int geometryIndex) {
    if(0 <= geometryIndex && geometryIndex < this->geometries.size()) {
        this->geometries[geometryIndex]->setShader(shader);
    }
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
