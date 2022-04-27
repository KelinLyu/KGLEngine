// Developed by Kelin Lyu.
#ifndef Engine_hpp
#define Engine_hpp
#include "Toolkit/libraries.h"
#include "System/Input.hpp"
#include "System/FontLibrary.hpp"
#include "System/Font.hpp"
#include "System/Animation.hpp"
#include "System/AudioBuffer.hpp"
#include "Shading/Image.hpp"
#include "Shading/Texture.hpp"
#include "Shading/Shader.hpp"
#include "Geometry/Geometry.hpp"
#include "SkeletalAnimation/Bone.hpp"
#include "SkeletalAnimation/Animator.hpp"
#include "Node/Node.hpp"
class Input;
class Animation;
class Skybox;
class Node;
class CameraNode;
class LightNode;
class PointNode;
class Engine final {
private:
    vec2 screenResolution;
    vec2 windowResolution;
    float resolutionScaleFactor;
    GLFWwindow* window;
    float FPS;
    float currentFPS;
    float currentTime;
    float updateTime;
    float deltaTime;
    bool cursorHidden;
    bool cursorLocked;
    Node* rootNode;
    map<string, Animation*> animations;
public:
    string workingDirectory;
    static Engine* main;
    Input* input;
    CameraNode* mainCameraNode;
    Skybox* skybox;
    vector<Geometry*> preparedGeometries;
    vector<LightNode*> preparedLightNodes;
    vector<LightNode*> preparedLightNodeShadows;
    Engine(const char* windowTitle,
           float resolutionScaleFactor,
           int samples,
           const char* iconFile);
    void hideCursor();
    void showCursor();
    void lockCursor();
    void unlockCursor();
    bool isRunning();
    bool shouldUpdate();
    void addNode(Node* node);
    void playAnimation(Animation* animation);
    void stopAnimation(string name);
    void render();
    void renderDirectionalLightShadowMap(LightNode* directionalLightNode);
    vec2 getScreenResolution();
    vec2 getWindowResolution();
    float getTime();
    float getDeltaTime();
    float getCurrentFPS();
    void terminate();
    ~Engine();
    static void engineReceiveKeyboardInteractions(GLFWwindow* window, int key, int code, int action, int modifiers);
    static void engineReceiveMouseInteractions(GLFWwindow* window, int button, int action, int modifiers);
    static void engineReceiveMouseMovements(GLFWwindow* window, double x, double y);
    static void engineReceiveScrollWheelInteractions(GLFWwindow* window, double dx, double dy);
};
#endif
