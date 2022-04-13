// Developed by Kelin Lyu.
#include "Animation.hpp"
Animation::Animation(const aiScene* scene, Animator* animator, Geometry* geometry) {
    this->animator = animator;
    geometry->engineAddAnimationToGeometry(this);
    aiAnimation* animation = scene->mAnimations[0];
    this->rootAnimationBoneNode = new AnimationBoneNode();
    this->engineProcessNode(this->rootAnimationBoneNode, scene->mRootNode);
    int size = animation->mNumChannels;
    map<string, BoneInfo>& newBonesInfoMap = geometry->engineGetGeometryBonesInfoMapReference();
    int& boneCount = geometry->engineGetGeometryBonesCountReference();
    for(unsigned int i = 0; i < size; i += 1) {
        aiNodeAnim* channel = animation->mChannels[i];
        string boneName = channel->mNodeName.data;
        if(newBonesInfoMap.find(boneName) == newBonesInfoMap.end()) {
            newBonesInfoMap[boneName].id = boneCount;
            boneCount = boneCount + 1;
        }
        this->bones.push_back(new Bone(channel->mNodeName.data, channel));
    }
}
Animation::~Animation() {
    this->rootAnimationBoneNode = NULL;
    for(unsigned int i = 0; i < this->animationBoneNodes.size(); i += 1) {
        this->animationBoneNodes[i]->children.clear();
        delete(this->animationBoneNodes[i]);
    }
    this->animationBoneNodes.clear();
    this->bones.clear();
}
void Animation::engineProcessNode(AnimationBoneNode* target, aiNode* node) {
    target->name = node->mName.data;
    target->transform = assimp_helper::getMat4(node->mTransformation);
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        AnimationBoneNode* newNode = new AnimationBoneNode();
        this->engineProcessNode(newNode, node->mChildren[i]);
        target->children.push_back(newNode);
    }
    this->animationBoneNodes.push_back(target);
}
Animator* Animation::engineGetAnimator() {
    return(this->animator);
}
AnimationBoneNode* Animation::engineGetRootAnimationBoneNode() {
    return(this->rootAnimationBoneNode);
}
Bone* Animation::engineGetBone(string name) {
    for(unsigned int i = 0; i < this->bones.size(); i += 1) {
        if(this->bones[i]->engineGetName() == name) {
            return(this->bones[i]);
        }
    }
    return(NULL);
}
void Animation::engineEraseAnimator() {
    this->animator = NULL;
}
