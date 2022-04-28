// Developed by Kelin Lyu.
#include "Animator.hpp"
Animator::Animator(string name, string file, vector<string>* boneNames, vector<mat4>* boneTransforms) {
    this->animatorBitMask = -1;
    this->name = name;
    this->state = 0;
    this->time = 0.0f;
    this->repeats = true;
    this->clamps = false;
    this->timeOffset = 0.0f;
    this->speed = 1.0f;
    this->blendFactor = 1.0f;
    this->fadeIn = 0.0f;
    this->fadeOut = 0.0f;
    this->currentBlendFactor = 0.0f;
    this->stateChangeTime = 0.0f;
    this->stateChangeBlendFactor = 0.0f;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(Engine::main->workingDirectory + file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "\nFailed to load the animation file: "
        << Engine::main->workingDirectory + file << "!\n" << endl;
        exit(0);
    }
    aiAnimation* animation = scene->mAnimations[0];
    this->duration = animation->mDuration;
    this->baseSpeed = animation->mTicksPerSecond;
    this->rootAnimationBoneNode = new AnimationBoneNode();
    this->engineAnimatorProcessNode(this->rootAnimationBoneNode, scene->mRootNode);
    int size = animation->mNumChannels;
    for(unsigned int i = 0; i < size; i += 1) {
        aiNodeAnim* channel = animation->mChannels[i];
        string boneName = channel->mNodeName.data;
        this->bones[boneName] = new Bone(channel->mNodeName.data, channel);
        bool found = false;
        for(unsigned int j = 0; j < (*boneNames).size(); j += 1) {
            if(boneName == (*boneNames)[j]) {
                found = true;
                break;
            }
        }
        if(!found) {
            (*boneNames).push_back(boneName);
            (*boneTransforms).push_back(mat4(1.0f));
        }
    }
}
void Animator::reset() {
    this->time = 0.0f;
    this->state = 0;
    this->currentBlendFactor = 0.0f;
}
void Animator::play(float fadeIn, float fadeOut) {
    this->fadeOut = fadeOut;
    if(this->state == 1 || this->state == 2) {
        return;
    }else if(this->state == 0) {
        this->time = this->timeOffset * this->baseSpeed;
    }
    if(!this->repeats) {
        this->reset();
    }
    this->fadeIn = fadeIn;
    this->startTime = Engine::main->getTime();
    this->stateChangeTime = Engine::main->getTime();
    this->stateChangeBlendFactor = this->currentBlendFactor;
    this->state = 1;
}
void Animator::stop(float fadeOut) {
    if(this->state == 3 || this->state == 0) {
        return;
    }
    this->fadeOut = fadeOut;
    this->stateChangeTime = Engine::main->getTime();
    this->stateChangeBlendFactor = this->currentBlendFactor;
    this->state = 3;
}
bool Animator::isPlaying() {
    return(this->state > 0);
}
float Animator::getTime() {
    return(this->time / this->baseSpeed);
}
float Animator::getDuration() {
    return(this->duration / this->baseSpeed);
}
float Animator::getCurrentBlendFactor() {
    return(this->currentBlendFactor);
}
Animator::~Animator() {
    for(unsigned int i = 0; i < this->animationBoneNodes.size(); i += 1) {
        this->animationBoneNodes[i]->children.clear();
        delete(this->animationBoneNodes[i]);
    }
    this->rootAnimationBoneNode = NULL;
    map<string, Bone*>::iterator iterator;
    for(iterator = this->bones.begin(); iterator != this->bones.end(); iterator++) {
        delete(iterator->second);
    }
    this->bones.clear();
}
string Animator::engineGetAnimatorName() {
    return(this->name);
}
float Animator::engineGetAnimatorTime() {
    return(this->time);
}
float Animator::engineGetAnimatorCurrentBlendFactor() {
    return(this->blendFactor * this->currentBlendFactor);
}
float Animator::engineGetAnimatorFadeInFactor(float progress) {
    float x = pi<float>() * progress / (this->fadeIn / this->speed);
    float value = (-cos(x) + 1) * 0.5 * (1 - this->stateChangeBlendFactor);
    return(value + this->stateChangeBlendFactor);
}
float Animator::engineGetAnimatorFadeOutFactor(float progress) {
    float x = pi<float>() * progress / (this->fadeOut / this->speed);
    float value = (cos(x) + 1) * 0.5;
    return(value * this->stateChangeBlendFactor);
}
void Animator::engineUpdateAnimator() {
    if(this->state == 0) {
        return;
    }
    float newTime = this->time + Engine::main->getDeltaTime() * this->baseSpeed * this->speed;
    if(newTime > this->duration) {
        if(!this->clamps) {
            this->time = fmod(newTime, this->duration);
        }
    }else{
        this->time = newTime;
    }
    if(this->state == 1) {
        float progress = Engine::main->getTime() - this->stateChangeTime;
        if(progress > (this->fadeIn / this->speed)) {
            this->state = 2;
            this->currentBlendFactor = 1.0;
            this->stateChangeTime = Engine::main->getTime();
        }else{
            this->currentBlendFactor = this->engineGetAnimatorFadeInFactor(progress);
        }
    }else if(this->state == 3) {
        float progress = Engine::main->getTime() - this->stateChangeTime;
        if(progress > (this->fadeOut / this->speed)) {
            this->state = 0;
            this->currentBlendFactor = 0.0;
            this->stateChangeTime = Engine::main->getTime();
        }else{
            this->currentBlendFactor = this->engineGetAnimatorFadeOutFactor(progress);
        }
    }
    if(!this->repeats && !this->clamps) {
        if(this->state == 1 || this->state == 2) {
            float progress = Engine::main->getTime() - this->startTime;
            float maxProgress = (this->duration / this->baseSpeed - this->fadeOut) / this->speed;
            if(progress > maxProgress) {
                this->stop(this->fadeOut);
            }
        }
    }
}
Animator* Animator::engineCopyAnimator() {
    Animator* animator = new Animator();
    animator->name = this->name;
    animator->state = this->state;
    animator->time = this->time;
    animator->repeats = this->repeats;
    animator->clamps = this->clamps;
    animator->speed = this->speed;
    animator->blendFactor = this->blendFactor;
    animator->fadeIn = this->fadeIn;
    animator->fadeOut = this->fadeOut;
    animator->currentBlendFactor = this->currentBlendFactor;
    animator->stateChangeTime = this->stateChangeTime;
    animator->stateChangeBlendFactor = this->stateChangeBlendFactor;
    animator->duration = this->duration;
    animator->baseSpeed = this->baseSpeed;
    animator->animatorBitMask = this->animatorBitMask;
    animator->rootAnimationBoneNode = animator->engineCopyAnimationBoneNode(this->rootAnimationBoneNode);
    animator->bones = this->bones;
    return(animator);
}
AnimationBoneNode* Animator::engineCopyAnimationBoneNode(AnimationBoneNode* targetNode) {
    AnimationBoneNode* animationBoneNode = new AnimationBoneNode();
    animationBoneNode->name = targetNode->name;
    animationBoneNode->position = targetNode->position;
    animationBoneNode->rotation = targetNode->rotation;
    animationBoneNode->scale = targetNode->scale;
    animationBoneNode->index = targetNode->index;
    for(unsigned int i = 0; i < targetNode->children.size(); i += 1) {
        animationBoneNode->children.push_back(this->engineCopyAnimationBoneNode(targetNode->children[i]));
    }
    this->animationBoneNodes.push_back(animationBoneNode);
    return(animationBoneNode);
}
void Animator::engineAnimatorProcessNode(AnimationBoneNode* targetNode, aiNode* node) {
    string name = node->mName.data;
    targetNode->name = name;
    mat4 transform = assimp_helper::getMat4(node->mTransformation);
    targetNode->position = glm_helper::getPosition(transform);
    targetNode->rotation = glm::quat_cast(transform);
    targetNode->scale = glm_helper::getScale(transform);
    targetNode->index = -1;
    for(unsigned int i = 0; i < node->mNumChildren; i += 1) {
        AnimationBoneNode* newNode = new AnimationBoneNode();
        this->engineAnimatorProcessNode(newNode, node->mChildren[i]);
        targetNode->children.push_back(newNode);
    }
    this->animationBoneNodes.push_back(targetNode);
}
void Animator::engineUpdateAnimatorBoneIndices(vector<string>* boneNames) {
    for(unsigned int i = 0; i < this->animationBoneNodes.size(); i += 1) {
        for(unsigned int j = 0; j < (*boneNames).size(); j += 1) {
            if(this->animationBoneNodes[i]->name == (*boneNames)[j]) {
                this->animationBoneNodes[i]->index = j;
                break;
            }
        }
    }
}
AnimationBoneNode* Animator::engineAnimatorGetRootAnimationBoneNode() {
    return(this->rootAnimationBoneNode);
}
Bone* Animator::engineAnimatorGetBone(string name) {
    if(this->bones.find(name) != this->bones.end()) {
        return(this->bones[name]);
    }
    return(NULL);
}
