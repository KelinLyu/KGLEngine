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
void Node::loadFile(string file) {
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
void Node::addChild(Node* node) {
    this->children.push_back(node);
    node->parent = this;
}
void Node::update(mat4 parentTransform, float deltaTime) {
    this->calculateWorldTransform(parentTransform);
    if(this->animators.size() > 0) {
        for(unsigned int i = 0; i < this->animators.size(); i += 1) {
            this->animators[i]->update(deltaTime);
        }
    }
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->update(this->worldTransform);
    }
    for(unsigned int i = 0; i < this->children.size(); i += 1) {
        this->children[i]->update(this->worldTransform, deltaTime);
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
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->prepareForRendering();
    }
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
    return(glm_helper::getPosition(this->worldTransform));
}
mat4 Node::getWorldTransform() {
    return(this->worldTransform);
}
vec3 Node::convertLocalPositionToWorld(vec3 localPosition) {
    mat4 translateMatrix = translate(mat4(1.0f), localPosition);
    translateMatrix = this->worldTransform * translateMatrix;
    return(glm_helper::getPosition(translateMatrix));
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
mat4 Node::getBoneWorldTransform(string boneName) {
    mat4 result = mat4(0.0f);
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        result = this->geometries[i]->getBoneWorldTransform(boneName);
        if(result != mat4(0.0f)) {
            return(result);
        }
    }
    return(result);
}
void Node::stickToBoneOfNode(string boneName, Node *node) {
    mat4 transform = node->getBoneWorldTransform(boneName);
    if(this->parent != NULL) {
        transform = inverse(this->parent->worldTransform) * transform;
    }
    this->position = glm_helper::getPosition(transform);
    this->eulerAngles = glm_helper::getEularAngles(transform);
    this->updateTransform();
}
Node::~Node() {
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        delete(this->geometries[i]);
    }
    this->geometries.clear();
    this->children.clear();
    this->parent = NULL;
}
