// Developed by Kelin.Lyu.
#ifndef input_hpp
#define input_hpp
#include "Toolkit/common.h"
class Input final {
private:
    class KeyEvent final {
    public:
        int key;
        int state;
        float stateUpdateTime;
        KeyEvent(int key, int state, float time);
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
    void engineKeyEvent(int key, int state, float time, string character);
    void engineMouseEvent(vec2 position);
    void engineScrollWheelEvent(float acceleration);
    bool isPressingKey(int key);
    bool wasKeyPressed(int key);
    bool wasKeyReleased(int key);
    float getKeyDuration(int key, float currentTime);
    string getLastPressedCharacter();
    vec2 getMousePosition();
    vec2 getMouseTranslation();
    float getScrollWheelAcceleration();
    void update();
    void clear();
    ~Input();
};
#endif
