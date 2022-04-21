// Developed by Kelin Lyu.
#include "Input.hpp"
Input::Input() {
    this->initialized = false;
    this->latestMousePosition = vec2(0.0f);
    this->mousePosition = vec2(0.0f);
    this->mouseTranslation = vec2(0.0f);
    this->scrollWheelAcceleration = 0.0f;
}
bool Input::isPressingKey(int key) {
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i]->key == key) {
            if(this->keyEvents[i]->state == 1 || this->keyEvents[i]->state == 2) {
                return(true);
            }else{
                return(false);
            }
        }
        i += 1;
    }
    return(false);
}
bool Input::wasKeyPressed(int key) {
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i]->key == key) {
            if(this->keyEvents[i]->state == 1) {
                this->keyEvents[i]->state = 2;
                return(true);
            }else{
                return(false);
            }
        }
        i += 1;
    }
    return(false);
}
bool Input::wasKeyReleased(int key) {
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i]->key == key) {
            if(this->keyEvents[i]->state == 3) {
                this->keyEvents[i]->state = 0;
                return(true);
            }else{
                return(false);
            }
        }
        i += 1;
    }
    return(false);
}
float Input::getKeyDuration(int key, float currentTime) {
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i]->key == key) {
            return(currentTime - this->keyEvents[i]->stateUpdateTime);
        }
        i += 1;
    }
    return(0.0f);
}
string Input::getLastPressedCharacter() {
    string result = this->lastCharacter;
    this->lastCharacter = "";
    return(result);
}
vec2 Input::getMouseScreenPosition() {
    return(this->mousePosition / Engine::main->getWindowResolution());
}
vec2 Input::getMouseTranslation() {
    return(this->mouseTranslation);
}
float Input::getScrollWheelAcceleration() {
    float result = this->scrollWheelAcceleration;
    this->scrollWheelAcceleration = 0.0f;
    return(result);
}
Input::~Input() {
    for(unsigned int i = 0; i < this->keyEvents.size(); i += 1) {
        delete(this->keyEvents[i]);
    }
    this->keyEvents.clear();
}
void Input::engineSetInputKeyEvent(int key, unsigned int state, float time, string character) {
    InputKeyEvent* targetInputKeyEvent = NULL;
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i]->key == key) {
            targetInputKeyEvent = this->keyEvents[i];
        }
        i += 1;
    }
    if(targetInputKeyEvent == NULL) {
        InputKeyEvent* newInputKeyEvent = new InputKeyEvent();
        newInputKeyEvent->key = key;
        newInputKeyEvent->state = state;
        newInputKeyEvent->stateUpdateTime = time;
        this->keyEvents.push_back(newInputKeyEvent);
    }else{
        targetInputKeyEvent->state = state;
        targetInputKeyEvent->stateUpdateTime = time;
    }
    if(state == 1) {
        this->lastCharacter = character;
    }
}
void Input::engineSetMouseEvent(vec2 position) {
    this->latestMousePosition = position;
    if(!this->initialized) {
        this->mousePosition = position;
        this->initialized = true;
    }
}
void Input::engineSetScrollWheelEvent(float acceleration) {
    this->scrollWheelAcceleration = acceleration;
}
void Input::engineUpdateInput() {
    this->mouseTranslation = this->latestMousePosition - this->mousePosition;
    this->mousePosition = this->latestMousePosition;
}
