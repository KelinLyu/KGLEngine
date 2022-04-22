// Developed by Kelin Lyu.
#ifndef Animator_hpp
#define Animator_hpp
#include "../Engine.hpp"
struct AnimationBoneNode final {
    string name;
    vec3 position;
    quat rotation;
    vec3 scale;
    int index;
    vector<AnimationBoneNode*> children;
};
class Bone;
class Node;
class Animator final {
private:
    unsigned int state;
    string name;
    float time;
    float duration;
    float baseSpeed;
    float fadeIn;
    float fadeOut;
    float currentBlendFactor;
    float stateChangeTime;
    float stateChangeBlendFactor;
    AnimationBoneNode* rootAnimationBoneNode;
    vector<AnimationBoneNode*> animationBoneNodes;
    map<string, Bone*> bones;
public:
    unsigned int animatorBitMask;
    bool repeats;
    bool clamps;
    float timeOffset;
    float speed;
    float blendFactor;
    Animator() = default;
    Animator(string name, string file, vector<string>* boneNames, vector<mat4>* boneTransforms);
    void reset();
    void play(float fadeIn, float fadeOut = 0.0f);
    void stop(float fadeOut);
    bool isPlaying();
    float getTime();
    float getDuration();
    float getCurrentBlendFactor();
    ~Animator();
    string engineGetAnimatorName();
    float engineGetAnimatorTime();
    float engineGetAnimatorCurrentBlendFactor();
    float engineGetAnimatorFadeInFactor(float progress);
    float engineGetAnimatorFadeOutFactor(float progress);
    void engineUpdateAnimator();
    Animator* engineCopyAnimator();
    AnimationBoneNode* engineCopyAnimationBoneNode(AnimationBoneNode* targetNode);
    void engineAnimatorProcessNode(AnimationBoneNode* targetNode, aiNode* node);
    void engineUpdateAnimatorBoneIndices(vector<string>* boneNames);
    AnimationBoneNode* engineAnimatorGetRootAnimationBoneNode();
    Bone* engineAnimatorGetBone(string name);
};
#endif
