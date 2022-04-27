// Developed by Kelin Lyu.
#include "Engine.hpp"
Engine* Engine::main;
Engine::Engine(const char* windowTitle,
               float resolutionScaleFactor,
               int samples,
               const char* iconFile) {
    this->FPS = 1.0f / 60.0f;
    this->currentFPS = 0.0f;
    this->currentTime = 0.0f;
    this->updateTime = 0.0f;
    this->deltaTime = 0.0f;
    this->cursorHidden = false;
    this->cursorLocked = false;
    this->resolutionScaleFactor = glm::clamp(resolutionScaleFactor, 0.0f, 1.0f);
    Engine::main = this;
    char directory[1024];
    if(getcwd(directory, sizeof(directory)) == NULL) {
        cout << "\nFailed to find the executable's location!\n" << endl;
        exit(1);
    }
    this->workingDirectory = string(directory);
    glfwTerminate();
    if(!glfwInit()) {
        cout << "\nFailed to initialize glfw!\n" << endl;
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if(this->resolutionScaleFactor == 1.0f) {
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    this->screenResolution = vec2(mode->width, mode->height);
    if(this->resolutionScaleFactor == 1.0f) {
        this->windowResolution = this->screenResolution;
        this->window = glfwCreateWindow(this->windowResolution.x, this->windowResolution.y, windowTitle, monitor, NULL);
    }else{
        this->windowResolution = this->screenResolution * resolutionScaleFactor;
        this->window = glfwCreateWindow(this->windowResolution.x, this->windowResolution.y, windowTitle, NULL, NULL);
    }
    glViewport(0, 0, this->windowResolution.x, this->windowResolution.y);
    if(this->window == NULL) {
        cout << "\nFailed to initialize the glfw window!\n" << endl;
        exit(1);
    }
    if(iconFile != NULL) {
        GLFWimage images[1];
        Image* icon = new Image(iconFile);
        images[0].pixels = icon->data;
        images[0].width = icon->width;
        images[0].height = icon->height;
        glfwSetWindowIcon(this->window, 1, images);
        delete(icon);
    }
    this->input = new Input();
    glfwSetKeyCallback(this->window, Engine::engineReceiveKeyboardInteractions);
    glfwSetMouseButtonCallback(this->window, Engine::engineReceiveMouseInteractions);
    glfwSetCursorPosCallback(this->window, Engine::engineReceiveMouseMovements);
    glfwSetScrollCallback(this->window, Engine::engineReceiveScrollWheelInteractions);
    glfwMakeContextCurrent(this->window);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    this->rootNode = new Node();
    this->mainCameraNode = NULL;
    this->skybox = NULL;
}
void Engine::hideCursor() {
    this->cursorHidden = true;
    if(!this->cursorLocked) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
}
void Engine::showCursor() {
    this->cursorHidden = false;
    if(!this->cursorLocked) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
void Engine::lockCursor() {
    this->cursorLocked = true;
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Engine::unlockCursor() {
    this->cursorLocked = false;
    if(this->cursorHidden) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }else{
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
bool Engine::isRunning() {
    return(!glfwWindowShouldClose(this->window));
}
bool Engine::shouldUpdate() {
    glfwPollEvents();
    bool result = false;
    this->currentTime = glfwGetTime();
    this->deltaTime = this->currentTime - this->updateTime;
    this->currentFPS = 1.0f / deltaTime;
    if(this->deltaTime >= this->FPS) {
        this->updateTime = this->currentTime;
        this->input->engineUpdateInput();
        this->rootNode->engineUpdateNodeAnimators(mat4(1.0f));
        map<string, Animation*>::iterator iterator;
        for(iterator = this->animations.begin(); iterator != this->animations.end();) {
            if(iterator->second->engineShouldRemoveAnimation()) {
                this->animations.erase(iterator++);
            }else{
                ++iterator;
            }
        }
        result = true;
    }
    return(result);
}
void Engine::addNode(Node* node) {
    this->rootNode->addChildNode(node);
}
void Engine::playAnimation(Animation* animation) {
    animation->engineActivateAnimation();
    this->animations[animation->engineGetAnimationName()] = animation;
}
void Engine::stopAnimation(string name) {
    Animation* animation = new Animation(name, 0.0f);
    animation->engineActivateAnimation();
    this->animations[name] = animation;
}
void Engine::render() {
    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glDepthMask(GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if(this->mainCameraNode != NULL) {
        vec3 cameraNodePosition = this->mainCameraNode->getWorldPosition();
        vec3 cameraNodeFrontVector = this->mainCameraNode->getFrontVectorInWorld();
        vec3 cameraNodeUpVector = this->mainCameraNode->getUpVectorInWorld();
        Listener::setPosition(cameraNodePosition.x, cameraNodePosition.y, cameraNodePosition.z);
        Listener::setDirection(cameraNodeFrontVector.x, cameraNodeFrontVector.y, cameraNodeFrontVector.z);
        Listener::setUpVector(cameraNodeUpVector.x, cameraNodeUpVector.y, cameraNodeUpVector.z);
        this->preparedGeometries.clear();
        this->preparedLightNodes.clear();
        this->preparedLightNodeShadows.clear();
        this->rootNode->enginePrepareNodeForRendering(mat4(1.0f), vec2(1.0f, 0.0f), false);
        if(this->skybox != NULL) {
            this->skybox->engineRenderGeometry(0);
        }
        for(unsigned int i = 0; i < this->preparedGeometries.size(); i += 1) {
            this->preparedGeometries[i]->engineRenderGeometry(0);
        }
        this->preparedGeometries.clear();
        this->preparedLightNodes.clear();
        this->preparedLightNodeShadows.clear();
    }
    glfwSwapInterval(1);
    glfwSwapBuffers(this->window);
}
void Engine::renderDirectionalLightShadowMap(LightNode* directionalLightNode) {
    CameraNode* currentCameraNode = this->mainCameraNode;
    directionalLightNode->engineGetDirectionalLightCameraNode()->renderingBitMask = directionalLightNode->shadowBitMask;
    this->mainCameraNode = directionalLightNode->engineGetDirectionalLightCameraNode();
    glViewport(0, 0, directionalLightNode->engineLightNodeGetShadowMapSize(), directionalLightNode->engineLightNodeGetShadowMapSize());
    glBindFramebuffer(GL_FRAMEBUFFER, directionalLightNode->engineLightNodeGetShadowBuffer());
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    if(this->mainCameraNode != NULL) {
        this->preparedGeometries.clear();
        this->preparedLightNodes.clear();
        this->rootNode->enginePrepareNodeForRendering(mat4(1.0f), vec2(1.0f, 0.0f), true);
        for(unsigned int i = 0; i < this->preparedGeometries.size(); i += 1) {
            this->preparedGeometries[i]->engineRenderGeometry(1);
        }
        this->preparedGeometries.clear();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->mainCameraNode = currentCameraNode;
}
vec2 Engine::getScreenResolution() {
    return(this->screenResolution);
}
vec2 Engine::getWindowResolution() {
    return(this->windowResolution);
}
float Engine::getTime() {
    return(this->currentTime);
}
float Engine::getDeltaTime() {
    return(this->deltaTime);
}
float Engine::getCurrentFPS() {
    return(this->currentFPS);
}
void Engine::terminate() {
    glfwSetWindowShouldClose(this->window, GL_TRUE);
}
Engine::~Engine() {
    glfwTerminate();
    delete(this->input);
    delete(this->rootNode);
    this->rootNode = NULL;
    this->mainCameraNode = NULL;
    Engine::main = NULL;
}
void Engine::engineReceiveKeyboardInteractions(GLFWwindow* window, int key, int code, int action, int modifiers) {
    if(action == GLFW_PRESS) {
        string character = "";
        if(key == 32) {
            character = " ";
        }else{
            const char* name = glfwGetKeyName(key, code);
            if(name != NULL) {
                if(modifiers & 0x01) {
                    char uppercase[1];
                    strcpy_s(uppercase, name);
                    uppercase[0] = toupper(uppercase[0]);
                    character = uppercase;
                }else{
                    character = name;
                }
            }
        }
        Engine::main->input->engineSetInputKeyEvent(key, 1, Engine::main->getTime(), character);
    }else if(action == GLFW_RELEASE) {
        Engine::main->input->engineSetInputKeyEvent(key, 3, Engine::main->getTime(), "");
    }
}
void Engine::engineReceiveMouseInteractions(GLFWwindow* window, int button, int action, int modifiers) {
    int key = KEY_UNKNOWN;
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        key = MOUSE_BUTTON_LEFT;
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        key = MOUSE_BUTTON_RIGHT;
    }else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
        key = MOUSE_BUTTON_MIDDLE;
    }
    if(action == GLFW_PRESS) {
        Engine::main->input->engineSetInputKeyEvent(key, 1, Engine::main->getTime(), "");
    }else if(action == GLFW_RELEASE) {
        Engine::main->input->engineSetInputKeyEvent(key, 3, Engine::main->getTime(), "");
    }
}
void Engine::engineReceiveMouseMovements(GLFWwindow* window, double x, double y) {
    vec2 position = vec2(x, y);
    Engine::main->input->engineSetMouseEvent(position);
}
void Engine::engineReceiveScrollWheelInteractions(GLFWwindow* window, double dx, double dy) {
    Engine::main->input->engineSetScrollWheelEvent(dy);
}
