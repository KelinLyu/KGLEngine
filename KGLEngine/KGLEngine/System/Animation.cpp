// Developed by Kelin Lyu.
#include "Animation.hpp"
Animation::Animation(string name, float duration) {
    this->name = name;
    this->startTime = 0;
    this->duration = glm::abs(duration);
    this->timingMode = 0;
    this->type = 0;
    this->hasCompletionHandler = false;
}
void Animation::setFloatAnimation(float* variable, float value) {
    this->type = 1;
    this->floatVariable = variable;
    this->floatOriginalValue = *variable;
    this->floatValue = value;
}
void Animation::setAngleAnimation(float* variable, float value) {
    this->type = 1;
    this->floatVariable = variable;
    this->floatOriginalValue = *variable;
    this->floatValue = value;
    while(this->floatValue - this->floatOriginalValue > 180.0f) {
        this->floatValue -= 360.0f;
    }
    while(this->floatOriginalValue - this->floatValue > 180.0f) {
        this->floatValue += 360.0f;
    }
}
void Animation::setVec2Animation(vec2* variable, vec2 value) {
    this->type = 2;
    this->vec2Variable = variable;
    this->vec2OriginalValue = *variable;
    this->vec2Value = value;
}
void Animation::setVec3Animation(vec3* variable, vec3 value) {
    this->type = 3;
    this->vec3Variable = variable;
    this->vec3OriginalValue = *variable;
    this->vec3Value = value;
}
void Animation::setVec4Animation(vec4* variable, vec4 value) {
    this->type = 4;
    this->vec4Variable = variable;
    this->vec4OriginalValue = *variable;
    this->vec4Value = value;
}
void Animation::setEulerAnglesAnimation(vec3* variable, vec3 value) {
    this->type = 3;
    this->vec3Variable = variable;
    this->vec3OriginalValue = *variable;
    this->vec3Value = value;
    while(this->vec3Value.x - this->vec3OriginalValue.x > 180.0f) {
        this->vec3Value.x -= 360.0f;
    }
    while(this->vec3OriginalValue.x - this->vec3Value.x > 180.0f) {
        this->vec3Value.x += 360.0f;
    }
    while(this->vec3Value.y - this->vec3OriginalValue.y > 180.0f) {
        this->vec3Value.y -= 360.0f;
    }
    while(this->vec3OriginalValue.y - this->vec3Value.y > 180.0f) {
        this->vec3Value.y += 360.0f;
    }
    while(this->vec3Value.z - this->vec3OriginalValue.z > 180.0f) {
        this->vec3Value.z -= 360.0f;
    }
    while(this->vec3OriginalValue.z - this->vec3Value.z > 180.0f) {
        this->vec3Value.z += 360.0f;
    }
}
void Animation::setLinearTimingMode() {
    this->timingMode = 0;
}
void Animation::setEaseInTimingMode() {
    this->timingMode = 1;
}
void Animation::setEaseOutTimingMode() {
    this->timingMode = 2;
}
void Animation::setEaseInEaseOutTimingMode() {
    this->timingMode = 3;
}
void Animation::setCompletionHandler(std::function<void()> completionHandler) {
    this->hasCompletionHandler = true;
    this->completionHandler = std::move(completionHandler);
}
string Animation::engineGetAnimationName() {
    return(this->name);
}
void Animation::engineActivateAnimation() {
    this->startTime = Engine::main->getTime();
}
float Animation::engineCalculateProgress(float timeProgress) {
    if(this->timingMode == 1) {
        return(1.0f - glm::cos((timeProgress * glm::pi<float>()) / 2.0f));
    }else if(this->timingMode == 2) {
        return(glm::sin((timeProgress * glm::pi<float>()) / 2.0f));
    }else if(this->timingMode == 3) {
        return(-(glm::cos(timeProgress * glm::pi<float>()) - 1.0f) / 2.0f);
    }
    return(timeProgress);
}
bool Animation::engineShouldRemoveAnimation() {
    float timeProgress = (Engine::main->getTime() - this->startTime) / this->duration;
    timeProgress = glm::clamp(timeProgress, 0.0f, 1.0f);
    float progress = this->engineCalculateProgress(timeProgress);
    if(this->type == 1) {
        (*this->floatVariable) = this->floatOriginalValue + (this->floatValue - this->floatOriginalValue) * progress;
    }else if(this->type == 2) {
        (*this->vec2Variable) = this->vec2OriginalValue + (this->vec2Value - this->vec2OriginalValue) * progress;
    }else if(this->type == 3) {
        (*this->vec3Variable) = this->vec3OriginalValue + (this->vec3Value - this->vec3OriginalValue) * progress;
    }else if(this->type == 4) {
        (*this->vec4Variable) = this->vec4OriginalValue + (this->vec4Value - this->vec4OriginalValue) * progress;
    }
    if(timeProgress >= 1.0f) {
        if(this->hasCompletionHandler) {
            this->completionHandler();
        }
        return(true);
    }
    return(false);
}
