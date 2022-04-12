// Developed by Kelin.Lyu.
#ifndef core_hpp
#define core_hpp
#include "Toolkit/common.h"
#include "input.hpp"
#include "Shading/image.hpp"
#include "Shading/texture.hpp"
#include "Shading/shader.hpp"
#include "Shading/material.hpp"
#include "Geometry/geometry.hpp"
#include "SkeletalAnimation/bone.hpp"
#include "SkeletalAnimation/animation.hpp"
#include "SkeletalAnimation/animator.hpp"
#include "Node/node.hpp"
#include "Others/skybox.hpp"
class Node;
class CameraNode;
class LightNode;
class Skybox;
class Engine final {
private:
    bool useAbsolutePaths;
    string programDirectory;
    float screenWidth;
    float screenHeight;
    GLFWwindow* window;
    float fps;
    float currentFps;
    float currentTime;
    float updateTime;
    float deltaTime;
    bool cursorHidden;
    bool cursorLocked;
    static void keyboardInteractions(GLFWwindow* window, int key, int code,
                                     int action, int modifiers);
    static void mouseInteractions(GLFWwindow* window, int button,
                                  int action, int modifiers);
    static void mouseMovements(GLFWwindow* window, double x, double y);
    static void scrollWheelInteractions(GLFWwindow* window, double dx, double dy);
    vector<Geometry*> geometries;
    vector<LightNode*> lightNodes;
public:
    static Engine* main;
    Input* input;
    Node* root;
    CameraNode* camera;
    Skybox* skybox;
    unsigned int currentShaderProgramID;
    Engine(const char* windowTitle,
           float resolutionScaleFactor,
           bool fullscreenMode,
           int samples,
           const char* iconFile,
           bool useAbsolutePaths);
    void changeResolution(float resolutionScaleFactor);
    void hideCursor();
    void showCursor();
    void lockCursor();
    void unlockCursor();
    bool isRunning();
    bool shouldUpdate();
    void update();
    void prepareGeometryForRendering(Geometry* geometry);
    void prepareLightNodeForRendering(LightNode* lightNode);
    void render();
    string getProgramDirectory();
    vec2 getScreenResolution();
    float getTime();
    float getDeltaTime();
    float getCurrentFps();
    void terminate();
    ~Engine();
};
#endif
