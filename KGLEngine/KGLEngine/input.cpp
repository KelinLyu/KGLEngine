// Developed by Kelin.Lyu.
#include "input.hpp"
Input::KeyEvent::KeyEvent(int key, int state, float time) {
    this->key = key;
    this->state = state;
    this->stateUpdateTime = time;
}
Input::Input() {
    this->initialized = false;
    this->latestMousePosition = vec2(0.0f, 0.0f);
    this->mousePosition = vec2(0.0f, 0.0f);
    this->mouseTranslation = vec2(0.0f, 0.0f);
    this->scrollWheelAcceleration = 0.0f;
}
void Input::engineKeyEvent(int key, int state, float time, string character) {
    Input::KeyEvent* targetKeyEvent = NULL;
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i].key == key) {
            targetKeyEvent = &this->keyEvents[i];
        }
        i += 1;
    }
    if(targetKeyEvent == NULL) {
        Input::KeyEvent* newKeyEvent = new Input::KeyEvent(key, state, time);
        this->keyEvents.push_back(*newKeyEvent);
    }else{
        targetKeyEvent->state = state;
        targetKeyEvent->stateUpdateTime = time;
    }
    if(state == 1) {
        this->lastCharacter = character;
    }
}
void Input::engineMouseEvent(vec2 position) {
    this->latestMousePosition = position;
    if(!this->initialized) {
        this->mousePosition = position;
        this->initialized = true;
    }
}
void Input::engineScrollWheelEvent(float acceleration) {
    this->scrollWheelAcceleration = acceleration;
}
bool Input::isPressingKey(int key) {
    int i = 0;
    while(i < this->keyEvents.size()) {
        if(this->keyEvents[i].key == key) {
            if(this->keyEvents[i].state == 1 || this->keyEvents[i].state == 2) {
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
        if(this->keyEvents[i].key == key) {
            if(this->keyEvents[i].state == 1) {
                this->keyEvents[i].state = 2;
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
        if(this->keyEvents[i].key == key) {
            if(this->keyEvents[i].state == 3) {
                this->keyEvents[i].state = 0;
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
        if(this->keyEvents[i].key == key) {
            return(currentTime - this->keyEvents[i].stateUpdateTime);
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
vec2 Input::getMousePosition() {
    return(this->mousePosition);
}
vec2 Input::getMouseTranslation() {
    return(this->mouseTranslation);
}
float Input::getScrollWheelAcceleration() {
    float result = this->scrollWheelAcceleration;
    this->scrollWheelAcceleration = 0.0f;
    return(result);
}
void Input::update() {
    this->mouseTranslation = this->latestMousePosition - this->mousePosition;
    this->mousePosition = this->latestMousePosition;
}
Input::~Input() {
    this->keyEvents.clear();
}
