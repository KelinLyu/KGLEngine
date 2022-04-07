// Developed by Kelin.Lyu.
#include "animation.hpp"
Animation::Animation(const aiScene* scene, Animator* animator, Geometry* geometry) {
    this->animator = animator;
    this->geometry = geometry;
    this->geometry->addAnimation(this);
    aiAnimation* animation = scene->mAnimations[0];
    this->assimpRootNode = new AssimpNode();
    this->processNode(this->assimpRootNode, scene->mRootNode);
    int size = animation->mNumChannels;
    map<string, BoneInfo>& newBonesInfoMap = geometry->getBonesInfoMap();
    int& boneCount = geometry->getBonesCount();
    for(unsigned int i = 0; i < size; i += 1) {
        aiNodeAnim* channel = animation->mChannels[i];
        string boneName = channel->mNodeName.data;
        if(newBonesInfoMap.find(boneName) == newBonesInfoMap.end()) {
            newBonesInfoMap[boneName].id = boneCount;
            boneCount = boneCount + 1;
        }
        this->bones.push_back(new Bone(channel->mNodeName.data, channel));
    }
    this->bonesInfoMap = newBonesInfoMap;
    this->boneTransforms.reserve(BONES_LIMIT);
    for(unsigned int i = 0; i < BONES_LIMIT; i += 1) {
        this->boneTransforms.push_back(mat4(1.0f));
    }
}
void Animation::processNode(AssimpNode* target, aiNode* node) {
    target->name = node->mName.data;
    target->transform = assimp_helper::getMat4(node->mTransformation);
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        AssimpNode* newNode = new AssimpNode();
        this->processNode(newNode, node->mChildren[i]);
        target->children.push_back(newNode);
    }
}
Bone* Animation::getBone(string name) {
    for(unsigned int i = 0; i < this->bones.size(); i += 1) {
        if(this->bones[i]->getName() == name) {
            return(this->bones[i]);
        }
    }
    return(NULL);
}
