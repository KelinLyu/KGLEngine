// Developed by Kelin Lyu.
#include "Animator.hpp"
Animator::Animator(string name, string file, Node* node) {
    this->name = name;
    this->state = 0;
    this->time = 0.0f;
    this->repeats = true;
    this->clamps = false;
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
    for(unsigned int i = 0; i < node->geometries.size(); i += 1) {
        if(node->geometries[i]->engineCheckWhetherGeometryHasBones()) {
            Animation* animation = new Animation(scene, this, node->geometries[i]);
            this->animations.push_back(animation);
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
        this->time = 0.0f;
    }
    if(!this->repeats) {
        this->reset();
    }
    this->fadeIn = fadeIn;
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
    return(this->time);
}
float Animator::getCurrentBlendFactor() {
    return(this->currentBlendFactor);
}
Animator::~Animator() {
    for(unsigned int i = 0; i < this->animations.size(); i += 1) {
        this->animations[i]->engineAnimationEraseAnimator();
    }
    this->animations.clear();
}
string Animator::engineAnimationGetAnimatorName() {
    return(this->name);
}
float Animator::engineAnimationGetAnimatorCurrentBlendFactor() {
    return(this->blendFactor * this->currentBlendFactor);
}
float Animator::engineAnimationGetAnimatorFadeInFactor(float progress) {
    float x = pi<float>() * progress / (this->fadeIn / this->speed);
    float value = (-cos(x) + 1) * 0.5 * (1 - this->stateChangeBlendFactor);
    return(value + this->stateChangeBlendFactor);
}
float Animator::engineAnimationGetAnimatorFadeOutFactor(float progress) {
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
            this->currentBlendFactor = this->engineAnimationGetAnimatorFadeInFactor(progress);
        }
    }else if(this->state == 3) {
        float progress = Engine::main->getTime() - this->stateChangeTime;
        if(progress > (this->fadeOut / this->speed)) {
            this->state = 0;
            this->currentBlendFactor = 0.0;
            this->stateChangeTime = Engine::main->getTime();
        }else{
            this->currentBlendFactor = this->engineAnimationGetAnimatorFadeOutFactor(progress);
        }
    }
    if(!this->repeats && !this->clamps) {
        if(this->state == 1 || this->state == 2) {
            float progress = Engine::main->getTime() - this->stateChangeTime;
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
    return(animator);
}
