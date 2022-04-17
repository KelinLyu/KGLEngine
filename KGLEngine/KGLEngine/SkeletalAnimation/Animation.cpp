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
    this->engineMatchAnimationBoneNodeWithBones(this->rootAnimationBoneNode);
}
Animation::~Animation() {
    this->rootAnimationBoneNode = NULL;
    this->bones.clear();
}
void Animation::engineProcessNode(AnimationBoneNode* targetNode, aiNode* node) {
    targetNode->boneIndex = -1;
    targetNode->name = node->mName.data;
    targetNode->transform = assimp_helper::getMat4(node->mTransformation);
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        AnimationBoneNode* newNode = new AnimationBoneNode();
        this->engineProcessNode(newNode, node->mChildren[i]);
        targetNode->children.push_back(newNode);
    }
}
void Animation::engineMatchAnimationBoneNodeWithBones(AnimationBoneNode* targetNode) {
    for(unsigned int i = 0; i < this->bones.size(); i += 1) {
        if(this->bones[i]->engineGetName() == targetNode->name) {
            targetNode->boneIndex = i;
            break;
        }
    }
    for(unsigned int i = 0; i < targetNode->children.size(); i += 1) {
        this->engineMatchAnimationBoneNodeWithBones(targetNode->children[i]);
    }
}
Animator* Animation::engineGetAnimator() {
    return(this->animator);
}
AnimationBoneNode* Animation::engineGetRootAnimationBoneNode() {
    return(this->rootAnimationBoneNode);
}
Bone* Animation::engineGetBone(int index) {
    if(index == -1) {
        return(NULL);
    }
    return(this->bones[index]);
}
void Animation::engineEraseAnimator() {
    this->animator = NULL;
}
Animation* Animation::engineCopyAnimation(Animator* newAnimator) {
    Animation* animation = new Animation();
    animation->animator = newAnimator;
    animation->rootAnimationBoneNode = this->rootAnimationBoneNode;
    animation->bones = this->bones;
    return(animation);
}
