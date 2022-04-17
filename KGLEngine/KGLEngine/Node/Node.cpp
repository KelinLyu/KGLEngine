// Developed by Kelin Lyu.
#include "Node.hpp"
Node::Node() {
    this->engineInitializeNode();
}
void Node::addChildNode(Node* node) {
    node->removeFromParentNode();
    node->parent = this;
    this->childNodes.push_back(node);
}
void Node::removeFromParentNode() {
    if(this->parent != NULL) {
        for(unsigned int i = 0; i < this->parent->childNodes.size(); i += 1) {
            if(this == this->parent->childNodes[i]) {
                this->parent->childNodes.erase(this->parent->childNodes.begin() + i);
                break;
            }
        }
        this->parent = NULL;
    }
}
void Node::loadUnitCube() {
    this->geometries.push_back(new UnitCube());
}
void Node::loadModelFile(string file) {
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(Engine::main->workingDirectory + file, flags);
    if(scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "\nFailed to load the 3D model file: "
             << Engine::main->workingDirectory + file << "!\n" << endl;
        exit(0);
    }
    this->engineProcessNode(scene->mRootNode, scene);
}
Animator* Node::loadAnimator(string file) {
    Animator* animator = new Animator(file, this);
    this->animators.push_back(animator);
    return(animator);
}
Node* Node::generateBoneNode(string boneName) {
    Node* boneNode = new Node();
    this->boneNodes[boneName] = boneNode;
    this->addChildNode(boneNode);
    return(boneNode);
}
Node* Node::copy() {
    Node* node = new Node();
    node->position = this->position;
    node->eulerAngles = this->eulerAngles;
    node->scale = this->scale;
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        node->geometries.push_back(this->geometries[i]->copy());
    }
    return(node);
}
Node* Node::clone() {
    Node* node = new Node();
    node->position = this->position;
    node->eulerAngles = this->eulerAngles;
    node->scale = this->scale;
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        if(this->geometries[i]->engineGetGeometryInstanceCount() == 0) {
            this->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
        }
        node->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
        node->geometries.push_back(this->geometries[i]);
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        node->addChildNode(this->childNodes[i]->clone());
    }
    return(node);
}
void Node::freeze() {
    this->updateTransform();
    this->engineRecursivelyFreezeChildNodes(&this->geometries);
}
void Node::updateTransform() {
    if(this->parent != NULL) {
        this->engineCalculateNodeWorldTransform(this->parent->getWorldTransform());
        for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
            this->childNodes[i]->updateTransform();
        }
    }
}
mat4 Node::getWorldTransform() {
    if(this->worldTransform == mat4(-1.0f)) {
        if(this->parent != NULL) {
            this->engineCalculateNodeWorldTransform(this->parent->getWorldTransform());
        }else{
            this->engineCalculateNodeWorldTransform(mat4(1.0f));
        }
    }
    return(this->worldTransform);
}
vec3 Node::getWorldPosition() {
    return(glm_helper::getPosition(this->worldTransform));
}
vec3 Node::getWorldEulerAngles() {
    return(glm_helper::getEularAngles(this->worldTransform));
}
vec3 Node::getWorldScale() {
    return(glm_helper::getScale(this->worldTransform));
}
vec3 Node::convertLocalPositionToWorld(vec3 localPosition) {
    mat4 translateMatrix = glm::translate(mat4(1.0f), localPosition);
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
vec3 Node::getPositionOnScreen() {
    if(Engine::main->camera != NULL) {
        vec4 worldPosition = vec4(this->getWorldPosition(), 1.0f);
        mat4 viewTransform = Engine::main->camera->getViewTransform();
        mat4 projectionTransform = Engine::main->camera->getProjectionTransform();
        mat4 transform = projectionTransform * viewTransform * this->worldTransform;
        vec4 projection;
        projection.x = worldPosition.x * transform[0][0] + worldPosition.y * transform[1][0] + worldPosition.z * transform[2][0] + transform[3][0];
        projection.y = worldPosition.x * transform[0][1] + worldPosition.y * transform[1][1] + worldPosition.z * transform[2][1] + transform[3][1];
        projection.z = worldPosition.x * transform[0][2] + worldPosition.y * transform[1][2] + worldPosition.z * transform[2][2] + transform[3][2];
        projection.w = worldPosition.x * transform[0][3] + worldPosition.y * transform[1][3] + worldPosition.z * transform[2][3] + transform[3][3];
        vec3 result;
        result.x = projection.x * 0.5f / projection.w + 0.5f;
        result.y = 0.5f - projection.y * 0.5f / projection.w;
        result.z = projection.z / projection.w;
        return(result);
    }
    return(vec3(0.0f));
}
Node::~Node() {
    this->removeFromParentNode();
    this->childNodes.clear();
    this->boneNodes.clear();
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        if(this->geometryInstancingIndex >= 0 && this->geometries[i]->engineGetGeometryInstanceCount() > 1) {
            this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, mat4(0.0f), true);
        }else{
            delete(this->geometries[i]);
        }
    }
    this->geometries.clear();
    this->childNodes.clear();
    this->parent = NULL;
}
void Node::engineInitializeNode() {
    this->name = "";
    this->tags = 0;
    this->parent = NULL;
    this->isDisabled = false;
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->worldTransform = mat4(-1.0f);
    this->geometryInstancingIndex = -1;
}
void Node::engineProcessNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i += 1) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->geometries.push_back(new Geometry(mesh));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        this->engineProcessNode(node->mChildren[i], scene);
    }
}
void Node::engineUpdateNodeAnimators(mat4 parentWorldTransform) {
    if(this->animators.size() > 0) {
        for(unsigned int i = 0; i < this->animators.size(); i += 1) {
            this->animators[i]->engineUpdateAnimator();
        }
    }
    if(this->isDisabled) {
        return;
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->engineUpdateGeometryAnimations();
    }
    map<string, Node*>::iterator iterator;
    for(iterator = this->boneNodes.begin(); iterator != this->boneNodes.end(); iterator++) {
        mat4 transform = mat4(0.0f);
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            transform = this->geometries[i]->engineGetGeometryBoneTransform(iterator->first);
            if(transform != mat4(0.0f)) {
                break;
            }
        }
        iterator->second->position = glm_helper::getPosition(transform);
        iterator->second->eulerAngles = glm_helper::getEularAngles(transform);
        iterator->second->updateTransform();
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->engineUpdateNodeAnimators(this->worldTransform);
    }
}
void Node::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    if(this->isDisabled) {
        return;
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->enginePrepareGeometryForRendering(this->worldTransform);
        if(this->geometryInstancingIndex >= 0) {
            this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, this->worldTransform, false);
        }
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->enginePrepareNodeForRendering(this->worldTransform, data);
    }
}
void Node::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    mat4 translateMatrix = glm::translate(mat4(1.0f), this->position);
    mat4 rotateMatrix = glm::eulerAngleXYZ(glm::radians(this->eulerAngles.x),
                                           glm::radians(this->eulerAngles.y),
                                           glm::radians(this->eulerAngles.z));
    mat4 scaleMatrix = glm::scale(mat4(1.0f), vec3(this->scale));
    this->worldTransform = parentWorldTransform * (translateMatrix * rotateMatrix * scaleMatrix);
}
void Node::engineRecursivelyFreezeChildNodes(vector<Geometry*>* allGeometries) {
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        if(find(allGeometries->begin(), allGeometries->end(), this->geometries[i]) == allGeometries->end()) {
            allGeometries->push_back(this->geometries[i]);
        }
        if(this->geometries[i]->engineGetGeometryInstanceCount() == 0) {
            this->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
        }
        if(this->geometryInstancingIndex >= 0) {
            this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, this->worldTransform, true);
        }
    }
    while(this->childNodes.size() > 0) {
        this->childNodes[0]->engineRecursivelyFreezeChildNodes(allGeometries);
        this->childNodes[0]->geometryInstancingIndex = -1;
        this->childNodes[0]->geometries.clear();
        delete(this->childNodes[0]);
    }
}
