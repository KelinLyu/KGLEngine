// Developed by Kelin Lyu.
#ifndef Input_hpp
#define Input_hpp
#include "../Engine.hpp"
class Input final {
private:
    class KeyEvent final {
    public:
        int key;
        unsigned int state;
        float stateUpdateTime;
        KeyEvent(int key, unsigned int state, float time);
    };
    bool initialized;
    vector<KeyEvent> keyEvents;
    string lastCharacter;
    vec2 latestMousePosition;
    vec2 mousePosition;
    vec2 mouseTranslation;
    float scrollWheelAcceleration;
public:
    Input();
    bool isPressingKey(int key);
    bool wasKeyPressed(int key);
    bool wasKeyReleased(int key);
    float getKeyDuration(int key, float currentTime);
    string getLastPressedCharacter();
    vec2 getMousePosition();
    vec2 getMouseTranslation();
    float getScrollWheelAcceleration();
    ~Input();
    void engineSetKeyEvent(int key, unsigned int state, float time, string character);
    void engineSetMouseEvent(vec2 position);
    void engineSetScrollWheelEvent(float acceleration);
    void engineUpdateInput();
};
#endif
