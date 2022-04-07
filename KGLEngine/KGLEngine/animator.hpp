// Developed by Kelin.Lyu.
#ifndef animator_hpp
#define animator_hpp
#include "Toolkit/common.h"
#include "core.hpp"
class Node;
class Animation;
class Animator {
private:
    int state;
    float time;
    float duration;
    float baseSpeed;
    float fadeIn;
    float fadeOut;
    float currentBlendFactor;
    float stateChangeTime;
    float stateChangeBlendFactor;
    vector<Animation*> animations;
    float getFadeInFactor(float progress);
    float getFadeOutFactor(float progress);
public:
    bool repeats;
    bool clamps;
    float speed;
    float blendFactor;
    Animator(string file, Node* node);
    void update(float deltaTime);
    void reset();
    void play(float fadeIn, float fadeOut = 0.0f);
    void stop(float fadeOut);
    bool isPlaying();
    float getTime();
    float getBlendFactor();
    float getCurrentBlendFactor();
    ~Animator();
};
#endif
