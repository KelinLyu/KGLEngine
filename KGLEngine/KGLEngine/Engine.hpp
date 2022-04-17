// Developed by Kelin Lyu.
#ifndef Engine_hpp
#define Engine_hpp
#include "Toolkit/libraries.h"
#include "System/Input.hpp"
#include "System/FontLibrary.hpp"
#include "System/Font.hpp"
#include "Shading/Image.hpp"
#include "Shading/Texture.hpp"
#include "Shading/Shader.hpp"
#include "Geometry/Geometry.hpp"
#include "SkeletalAnimation/Bone.hpp"
#include "SkeletalAnimation/Animation.hpp"
#include "SkeletalAnimation/Animator.hpp"
#include "Node/Node.hpp"
class Input;
class Skybox;
class Node;
class CameraNode;
class LightNode;
class PointNode;
class Engine final {
private:
    vec2 screenResolution;
    vec2 windowResolution;
    GLFWwindow* window;
    float fps;
    float currentFps;
    float currentTime;
    float updateTime;
    float deltaTime;
    bool cursorHidden;
    bool cursorLocked;
    Node* rootNode;
public:
    string workingDirectory;
    static Engine* main;
    Input* input;
    CameraNode* camera;
    Skybox* skybox;
    vector<Geometry*> preparedGeometries;
    vector<LightNode*> preparedLightNodes;
    Engine(const char* windowTitle,
           float resolutionScaleFactor,
           bool fullscreenMode,
           int samples,
           const char* iconFile);
    void changeResolution(float resolutionScaleFactor);
    void hideCursor();
    void showCursor();
    void lockCursor();
    void unlockCursor();
    bool isRunning();
    bool shouldUpdate();
    void addNode(Node* node);
    void render();
    vec2 getScreenResolution();
    vec2 getWindowResolution();
    float getTime();
    float getDeltaTime();
    float getCurrentFps();
    void terminate();
    ~Engine();
    static void engineReceiveKeyboardInteractions(GLFWwindow* window, int key, int code, int action, int modifiers);
    static void engineReceiveMouseInteractions(GLFWwindow* window, int button, int action, int modifiers);
    static void engineReceiveMouseMovements(GLFWwindow* window, double x, double y);
    static void engineReceiveScrollWheelInteractions(GLFWwindow* window, double dx, double dy);
};
#endif
