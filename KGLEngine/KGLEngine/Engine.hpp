// Developed by Kelin Lyu.
#ifndef Engine_hpp
#define Engine_hpp
#include "Toolkit/libraries.h"
#include "System/Input.hpp"
#include "Shading/Image.hpp"
#include "Shading/Texture.hpp"

#include "Shading/Shader.hpp"
#include "Shading/Material.hpp"

#include "Geometry/Geometry.hpp"

#include "SkeletalAnimation/Bone.hpp"
#include "SkeletalAnimation/Animation.hpp"
#include "SkeletalAnimation/Animator.hpp"
#include "Node/Node.hpp"

//#include "Others/skybox.hpp"

class Input;
class Node;
class CameraNode;
class LightNode;
class Skybox;
class Engine final {
private:
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
    
    
public:
    string workingDirectory;
    static Engine* main;
    Input* input;
    Node* root;
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
    void update();
    void prepareGeometryForRendering(Geometry* geometry);
    void prepareLightNodeForRendering(LightNode* lightNode);
    void render();
    
    
    
    
    vec2 getScreenResolution();
    float getTime();
    float getDeltaTime();
    float getCurrentFps();
    void terminate();
    ~Engine();
};
#endif
