// Developed by Kelin Lyu.
#include "Animation.hpp"
Animation::Animation(const aiScene* scene, Animator* animator, Geometry* geometry) {
    this->animator = animator;
    geometry->engineAddAnimationToGeometry(this);
    aiAnimation* animation = scene->mAnimations[0];
    map<string, BoneInfo>* bonesInfoMap = geometry->engineGetGeometryBonesInfoMap();
    this->rootAnimationBoneNode = new AnimationBoneNode();
    this->engineAnimationProcessNode(this->rootAnimationBoneNode, scene->mRootNode, bonesInfoMap);
    int size = animation->mNumChannels;
    for(unsigned int i = 0; i < size; i += 1) {
        aiNodeAnim* channel = animation->mChannels[i];
        string boneName = channel->mNodeName.data;
        if((*bonesInfoMap).find(boneName) != (*bonesInfoMap).end()) {
            this->bones[boneName] = new Bone(channel->mNodeName.data, channel);
        }
    }
}
Animation::~Animation() {
    this->rootAnimationBoneNode = NULL;
    this->bones.clear();
}
void Animation::engineAnimationProcessNode(AnimationBoneNode* targetNode, aiNode* node, map<string, BoneInfo>* bonesInfoMap) {
    string name = node->mName.data;
    if((*bonesInfoMap).find(name) != (*bonesInfoMap).end()) {
        targetNode->name = name;
    }else{
        targetNode->name = "";
    }
    targetNode->transform = assimp_helper::getMat4(node->mTransformation);
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        AnimationBoneNode* newNode = new AnimationBoneNode();
        this->engineAnimationProcessNode(newNode, node->mChildren[i], bonesInfoMap);
        targetNode->children.push_back(newNode);
    }
}
Animator* Animation::engineAnimationGetAnimator() {
    return(this->animator);
}
AnimationBoneNode* Animation::engineGetRootAnimationBoneNode() {
    return(this->rootAnimationBoneNode);
}
Bone* Animation::engineAnimationGetBone(string name) {
    if(this->bones.find(name) != this->bones.end()) {
        return(this->bones[name]);
    }
    return(NULL);
}
void Animation::engineAnimationEraseAnimator() {
    this->animator = NULL;
}
Animation* Animation::engineCopyAnimation(Animator* newAnimator) {
    Animation* animation = new Animation();
    animation->animator = newAnimator;
    animation->rootAnimationBoneNode = this->rootAnimationBoneNode;
    animation->bones = this->bones;
    return(animation);
}
