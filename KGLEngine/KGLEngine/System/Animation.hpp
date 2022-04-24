// Developed by Kelin Lyu.
#ifndef Animation_hpp
#define Animation_hpp
#include "../Engine.hpp"
class Animation final {
private:
    string name;
    float startTime;
    float duration;
    unsigned int timingMode;
    unsigned int type;
    float* floatVariable;
    float floatOriginalValue;
    float floatValue;
    vec2* vec2Variable;
    vec2 vec2OriginalValue;
    vec2 vec2Value;
    vec3* vec3Variable;
    vec3 vec3OriginalValue;
    vec3 vec3Value;
    vec4* vec4Variable;
    vec4 vec4OriginalValue;
    vec4 vec4Value;
    bool hasCompletionHandler;
    std::function<void()> completionHandler;
public:
    Animation(string name, float duration);
    void setFloatAnimation(float* variable, float value);
    void setAngleAnimation(float* variable, float value);
    void setVec2Animation(vec2* variable, vec2 value);
    void setVec3Animation(vec3* variable, vec3 value);
    void setVec4Animation(vec4* variable, vec4 value);
    void setEulerAnglesAnimation(vec3* variable, vec3 value);
    void setLinearTimingMode();
    void setEaseInTimingMode();
    void setEaseOutTimingMode();
    void setEaseInEaseOutTimingMode();
    void setCompletionHandler(std::function<void()> completionHandler);
    ~Animation();
    string engineGetAnimationName();
    void engineActivateAnimation();
    float engineCalculateProgress(float timeProgress);
    bool engineShouldRemoveAnimation();
};
#endif
