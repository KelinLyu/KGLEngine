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
        if(this->geometryInstancingIndex >= 0) {
            for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
                this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, mat4(0.0f), false);
            }
        }
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
Animator* Node::loadAnimator(string name, string file) {
    Animator* animator = new Animator(name, file, &this->boneNames, &this->boneTransforms);
    this->animators.push_back(animator);
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        this->animators[i]->engineUpdateAnimatorBoneIndices(&this->boneNames);
    }
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->engineUpdateGeometryBoneIndices(&this->boneNames);
    }
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
    node->name = this->name;
    node->isDisabled = this->isDisabled;
    node->renderingBitMask = this->renderingBitMask;
    node->position = this->position;
    node->eulerAngles = this->eulerAngles;
    node->scale = this->scale;
    node->orientationTargetNode = this->orientationTargetNode;
    node->boneNames = this->boneNames;
    node->boneTransforms = this->boneTransforms;
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        node->animators.push_back(this->animators[i]->engineCopyAnimator());
    }
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        node->geometries.push_back(this->geometries[i]->copy(&node->animators));
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        Node* newNode = this->childNodes[i]->copy();
        node->addChildNode(newNode);
        map<string, Node*>::iterator iterator;
        for(iterator = this->boneNodes.begin(); iterator != this->boneNodes.end(); iterator++) {
            if(iterator->second == this->childNodes[i]) {
                node->boneNodes[iterator->first] = newNode;
                break;
            }
        }
    }
    return(node);
}
Node* Node::clone() {
    Node* node = new Node();
    node->name = this->name;
    node->isDisabled = this->isDisabled;
    node->renderingBitMask = this->renderingBitMask;
    node->position = this->position;
    node->eulerAngles = this->eulerAngles;
    node->scale = this->scale;
    node->orientationTargetNode = this->orientationTargetNode;
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        if(this->geometries[i]->engineGetGeometryInstanceCount() == 0) {
            this->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
        }
        node->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
        node->geometries.push_back(this->geometries[i]);
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        Node* newNode = this->childNodes[i]->clone();
        node->addChildNode(newNode);
        map<string, Node*>::iterator iterator;
        for(iterator = this->boneNodes.begin(); iterator != this->boneNodes.end(); iterator++) {
            if(iterator->second == this->childNodes[i]) {
                node->boneNodes[iterator->first] = newNode;
                break;
            }
        }
    }
    return(node);
}
void Node::freeze() {
    this->updateTransform();
    this->engineRecursivelyFreezeChildNodes(&this->geometries, &this->frozenNodeGeometryInstancingIndices);
}
Animator* Node::getAnimator(string name) {
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        if(this->animators[i]->engineGetAnimatorName() == name) {
            return(this->animators[i]);
        }
    }
    return(NULL);
}
void Node::playAnimators(unsigned int mask, float fadeIn, float fadeOut) {
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        if((this->animators[i]->animatorBitMask & mask) > 0) {
            this->animators[i]->play(fadeIn, fadeOut);
        }
    }
}
void Node::stopAnimators(unsigned int mask, float fadeOut) {
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        if((this->animators[i]->animatorBitMask & mask) > 0) {
            this->animators[i]->stop(fadeOut);
        }
    }
}
void Node::updateTransform() {
    if(this->parent != NULL) {
        this->engineCalculateNodeWorldTransform(this->parent->getWorldTransform());
        for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
            this->childNodes[i]->updateTransform();
        }
    }
}
Node* Node::getChildNode(string name) {
    if(this->name == name) {
        return(this);
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        Node* childNode = this->childNodes[i]->getChildNode(name);
        if(childNode != NULL) {
            return(childNode);
        }
    }
    return(NULL);
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
    return(glm_helper::getEulerAngles(this->worldTransform));
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
    if(Engine::main->mainCameraNode != NULL) {
        vec4 worldPosition = vec4(this->getWorldPosition(), 1.0f);
        mat4 viewTransform = Engine::main->mainCameraNode->getViewTransform();
        mat4 projectionTransform = Engine::main->mainCameraNode->getProjectionTransform();
        mat4 transform = projectionTransform * viewTransform * this->worldTransform;
        vec4 projection;
        projection.x = worldPosition.x * transform[0][0] + worldPosition.y * transform[1][0] + worldPosition.z * transform[2][0] + transform[3][0];
        projection.y = worldPosition.x * transform[0][1] + worldPosition.y * transform[1][1] + worldPosition.z * transform[2][1] + transform[3][1];
        projection.z = worldPosition.x * transform[0][2] + worldPosition.y * transform[1][2] + worldPosition.z * transform[2][2] + transform[3][2];
        projection.w = worldPosition.x * transform[0][3] + worldPosition.y * transform[1][3] + worldPosition.z * transform[2][3] + transform[3][3];
        vec3 result;
        result.x = projection.x * 0.5f / projection.w + 0.5f;
        result.y = 0.5f - projection.y * 0.5f / projection.w;
        result.z = glm::length(Engine::main->mainCameraNode->getWorldPosition() - this->getWorldPosition());
        if(projection.w < 0.0f) {
            result.z = -result.z;
        }
        return(result);
    }
    return(vec3(0.0f));
}
CameraNode* Node::convertToCameraNode() {
    return(this->currentCameraNode);
}
LightNode* Node::convertToLightNode() {
    return(this->currentLightNode);
}
ParticleNode* Node::convertToParticleNode() {
    return(this->currentParticleNode);
}
UINode* Node::convertToUINode() {
    return(this->currentUINode);
}
SpriteNode* Node::convertToSpriteNode() {
    return(this->currentSpriteNode);
}
TextNode* Node::convertToTextNode() {
    return(this->currentTextNode);
}
Node::~Node() {
    this->removeFromParentNode();
    this->childNodes.clear();
    this->boneNodes.clear();
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        delete(this->animators[i]);
    }
    this->animators.clear();
    if(this->frozenNodeGeometryInstancingIndices.size() > 0) {
        map<Geometry*, vector<unsigned int>>::iterator iterator;
        for(iterator = this->frozenNodeGeometryInstancingIndices.begin();
            iterator != this->frozenNodeGeometryInstancingIndices.end();
            iterator++) {
            for(unsigned i = 0; i < iterator->second.size(); i += 1) {
                iterator->first->engineUpdateGeometryInstanceTransform(iterator->second[i], mat4(0.0f), true);
            }
            iterator->second.clear();
        }
        this->frozenNodeGeometryInstancingIndices.clear();
    }else if(this->geometryInstancingIndex == -1) {
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            if(this->geometryInstancingIndex >= 0 && this->geometries[i]->engineGetGeometryInstanceCount() > 1) {
                this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, mat4(0.0f), true);
            }else{
                delete(this->geometries[i]);
            }
        }
    }
    this->geometries.clear();
    this->childNodes.clear();
    this->parent = NULL;
}
void Node::engineInitializeNode() {
    this->name = "";
    this->parent = NULL;
    this->isDisabled = false;
    this->renderingBitMask = -1;
    this->position = vec3(0.0f);
    this->eulerAngles = vec3(0.0f);
    this->scale = vec3(1.0f);
    this->orientationTargetNode = NULL;
    this->worldTransform = mat4(-1.0f);
    this->geometryInstancingIndex = -1;
    this->hasUnfreezableGeometries = false;
    this->currentCameraNode = NULL;
    this->currentLightNode = NULL;
    this->currentUINode = NULL;
    this->currentSpriteNode = NULL;
    this->currentTextNode = NULL;
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
    if(this->isDisabled) {
        return;
    }
    if(this->animators.size() > 0) {
        for(unsigned int i = 0; i < this->animators.size(); i += 1) {
            this->animators[i]->engineUpdateAnimator();
        }
        this->engineNodeCalculateBoneTransforms(this->animators[0]->engineAnimatorGetRootAnimationBoneNode(), mat4(1.0f));
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            this->geometries[i]->engineUpdateGeometrySkeletalAnimations(this->boneTransforms);
        }
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
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
        iterator->second->eulerAngles = glm_helper::getEulerAngles(transform);
        iterator->second->updateTransform();
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->engineUpdateNodeAnimators(this->worldTransform);
    }
}
void Node::engineNodeCalculateBoneTransforms(AnimationBoneNode *node, mat4 parentTransform) {
    string nodeName = node->name;
    vec3 position = node->position;
    quat rotation = node->rotation;
    vec3 scale = node->scale;
    int index = node->index;
    for(unsigned int i = 0; i < this->animators.size(); i += 1) {
        float blendFactor = this->animators[i]->engineGetAnimatorCurrentBlendFactor();
        if(blendFactor == 0.0f) {
            continue;
        }
        Bone* bone = this->animators[i]->engineAnimatorGetBone(nodeName);
        if(bone == NULL) {
            continue;
        }else{
            bone->engineUpdateBoneAnimation(this->animators[i]->engineGetAnimatorTime());
            position = glm::mix(position, bone->engineGetBonePosition(), blendFactor);
            rotation = glm::slerp(rotation, bone->engineGetBoneRotation(), blendFactor);
            scale = glm::mix(scale, bone->engineGetBoneScale(), blendFactor);
        }
    }
    mat4 finalTransform = glm::translate(mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(mat4(1.0f), scale);
    mat4 globalTransform = parentTransform * finalTransform;
    if(index != -1) {
        this->boneTransforms[index] = globalTransform;
    }
    for(unsigned int i = 0; i < node->children.size(); i += 1) {
        this->engineNodeCalculateBoneTransforms(node->children[i], globalTransform);
    }
}
void Node::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data, bool shadowMap) {
    if(this->isDisabled) {
        return;
    }
    this->engineCalculateNodeWorldTransform(parentWorldTransform);
    if(this->orientationTargetNode != NULL) {
        vec3 front = this->orientationTargetNode->getWorldPosition();
        mat4 matrix = glm::lookAt(this->getWorldPosition(), front, vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::inverse(matrix);
        this->worldTransform = glm::scale(matrix, this->scale);
    }
    if((this->renderingBitMask & Engine::main->mainCameraNode->renderingBitMask) > 0) {
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            this->geometries[i]->enginePrepareGeometryForRendering(this->worldTransform);
            if(this->geometryInstancingIndex >= 0) {
                this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, this->worldTransform, false);
            }
        }
    }
    for(unsigned int i = 0; i < this->childNodes.size(); i += 1) {
        this->childNodes[i]->enginePrepareNodeForRendering(this->worldTransform, data, shadowMap);
    }
}
void Node::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    mat4 translateMatrix = glm::translate(mat4(1.0f), this->position);
    mat4 rotateMatrix = glm::eulerAngleYZX(glm::radians(this->eulerAngles.y),
                                           glm::radians(this->eulerAngles.z),
                                           glm::radians(this->eulerAngles.x));
    mat4 scaleMatrix = glm::scale(mat4(1.0f), vec3(this->scale));
    this->worldTransform = parentWorldTransform * (translateMatrix * rotateMatrix * scaleMatrix);
}
void Node::engineRecursivelyFreezeChildNodes(vector<Geometry*>* allGeometries, map<Geometry*, vector<unsigned int>>* indices) {
    if(!this->hasUnfreezableGeometries) {
        for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
            if(find(allGeometries->begin(), allGeometries->end(), this->geometries[i]) == allGeometries->end()) {
                allGeometries->push_back(this->geometries[i]);
            }
            if(this->geometries[i]->engineGetGeometryInstanceCount() == 0) {
                this->geometryInstancingIndex = this->geometries[i]->engineGeometryAddInstance();
            }
            if(this->geometryInstancingIndex != -1) {
                this->geometries[i]->engineUpdateGeometryInstanceTransform(this->geometryInstancingIndex, this->worldTransform, true);
                if(indices->find(this->geometries[i]) == indices->end()) {
                    vector<unsigned int> array;
                    array.push_back(this->geometryInstancingIndex);
                    (*indices)[this->geometries[i]] = array;
                }else{
                    (*indices)[this->geometries[i]].push_back(this->geometryInstancingIndex);
                }
            }
        }
    }
    this->geometryInstancingIndex = -1;
    this->animators.clear();
    while(this->childNodes.size() > 0) {
        this->childNodes[0]->engineRecursivelyFreezeChildNodes(allGeometries, indices);
        this->childNodes[0]->geometryInstancingIndex = -1;
        this->childNodes[0]->geometries.clear();
        map<Geometry*, vector<unsigned int>>::iterator iterator;
        for(iterator = this->childNodes[0]->frozenNodeGeometryInstancingIndices.begin();
            iterator != this->childNodes[0]->frozenNodeGeometryInstancingIndices.end();
            iterator++) {
            if(indices->find(iterator->first) == indices->end()) {
                (*indices)[iterator->first] = iterator->second;
            }else{
                (*indices)[iterator->first].insert((*indices)[iterator->first].end(), iterator->second.begin(), iterator->second.end());
            }
            iterator->second.clear();
        }
        this->childNodes[0]->frozenNodeGeometryInstancingIndices.clear();
        delete(this->childNodes[0]);
    }
}
