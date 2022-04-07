// Developed by Kelin.Lyu.
#ifndef core_hpp
#define core_hpp
#include "Toolkit/common.h"
#include "input.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "geometry.hpp"
#include "node.hpp"
#include "bone.hpp"
#include "animation.hpp"
#include "animator.hpp"
class Node;
class CameraNode;
class LightNode;
class Engine final {
private:
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
    string programDirectory;
    float screenWidth;
    float screenHeight;
    GLFWwindow* window;
    Input* input;
    Node* root;
    CameraNode* cameraNode;
    Engine(const char* windowTitle,
           float resolutionScaleFactor,
           bool fullscreenMode,
           const char* iconFile);
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
    float getTime();
    float getDeltaTime();
    float getCurrentFps();
    void terminate();
    ~Engine();
};
#endif
