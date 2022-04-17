// Developed by Kelin Lyu.
#ifndef Animator_hpp
#define Animator_hpp
#include "../Engine.hpp"
class Node;
class Animation;
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
    vector<Animation*> animations;
public:
    bool repeats;
    bool clamps;
    float speed;
    float blendFactor;
    Animator() = default;
    Animator(string name, string file, Node* node);
    void reset();
    void play(float fadeIn, float fadeOut = 0.0f);
    void stop(float fadeOut);
    bool isPlaying();
    float getTime();
    float getCurrentBlendFactor();
    ~Animator();
    string engineAnimationGetAnimatorName();
    float engineAnimationGetAnimatorCurrentBlendFactor();
    float engineAnimationGetAnimatorFadeInFactor(float progress);
    float engineAnimationGetAnimatorFadeOutFactor(float progress);
    void engineUpdateAnimator();
    Animator* engineCopyAnimator();
};
#endif
