// Developed by Kelin.Lyu.
#include "core.hpp"
Engine* Engine::main;
Engine::Engine(const char* windowTitle,
               float resolutionScaleFactor,
               bool fullscreenMode,
               const char* iconFile) {
    this->fps = 1.0f / 60.0f;
    this->currentFps = 0.0f;
    this->currentTime = 0.0f;
    this->updateTime = 0.0f;
    this->deltaTime = 0.0f;
    this->cursorHidden = false;
    this->cursorLocked = false;
    Engine::main = this;
    // get the executable's location:
    char directory[1024];
    if(getcwd(directory, sizeof(directory)) == NULL) {
        cout << "\nFailed to find the executable's location using getcwd()!" << endl;
        exit(1);
    }
    this->programDirectory = string(directory);
    // initialize the window:
    glfwTerminate();
    if(!glfwInit()) {
        cout << "\nFailed to initialize glfw!" << endl;
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if(fullscreenMode) {
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
    // create the window:
    this->screenWidth = (float)mode->width;
    this->screenHeight = (float)mode->height;
    int width = (int)(this->screenWidth * resolutionScaleFactor);
    int height = (int)(this->screenHeight * resolutionScaleFactor);
    if(fullscreenMode) {
        this->window = glfwCreateWindow(width, height, windowTitle, monitor, NULL);
    }else{
        this->window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    }
    glViewport(0, 0, width, height);
    if(this->window == NULL) {
        cout << "\nFailed to initialize the glfw window!" << endl;
        exit(1);
    }
    // set the window's icon if provided:
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
    glfwSetKeyCallback(this->window, Engine::keyboardInteractions);
    glfwSetMouseButtonCallback(this->window, Engine::mouseInteractions);
    glfwSetCursorPosCallback(this->window, Engine::mouseMovements);
    glfwSetScrollCallback(this->window, Engine::scrollWheelInteractions);
    glfwMakeContextCurrent(this->window);
    // initialize glew:
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    this->root = new Node();
}
void Engine::changeResolution(float resolutionScaleFactor) {
    int width = (int)(this->screenWidth * resolutionScaleFactor);
    int height = (int)(this->screenHeight * resolutionScaleFactor);
    glfwSetWindowSize(this->window, width, height);
    glViewport(0, 0, width, height);
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
void Engine::keyboardInteractions(GLFWwindow* window, int key, int code,
                                  int action, int modifiers) {
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
        Engine::main->input->engineKeyEvent(key, 1, Engine::main->getTime(), character);
    }else if(action == GLFW_RELEASE) {
        Engine::main->input->engineKeyEvent(key, 3, Engine::main->getTime(), "");
    }
}
void Engine::mouseInteractions(GLFWwindow* window, int button,
                               int action, int modifiers) {
    int key = KEY_UNKNOWN;
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        key = MOUSE_BUTTON_LEFT;
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        key = MOUSE_BUTTON_RIGHT;
    }else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
        key = MOUSE_BUTTON_MIDDLE;
    }
    if(action == GLFW_PRESS) {
        Engine::main->input->engineKeyEvent(key, 1, Engine::main->getTime(), "");
    }else if(action == GLFW_RELEASE) {
        Engine::main->input->engineKeyEvent(key, 3, Engine::main->getTime(), "");
    }
}
void Engine::mouseMovements(GLFWwindow* window, double x, double y) {
    vec2 position = vec2(x, y);
    Engine::main->input->engineMouseEvent(position);
}
void Engine::scrollWheelInteractions(GLFWwindow* window, double dx, double dy) {
    Engine::main->input->engineScrollWheelEvent(dy);
}
bool Engine::isRunning() {
    return(!glfwWindowShouldClose(this->window));
}
bool Engine::shouldUpdate() {
    glfwPollEvents();
    bool result = false;
    this->currentTime = glfwGetTime();
    this->deltaTime = this->currentTime - this->updateTime;
    this->currentFps = 1.0f / deltaTime;
    if(this->deltaTime >= this->fps) {
        this->input->update();
        this->root->updateAnimators(mat4(1.0f), this->deltaTime);
        this->updateTime = this->currentTime;
        result = true;
    }
    return(result);
}
void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    this->geometries.clear();
    this->lightNodes.clear();
    this->root->prepareForRendering(mat4(1.0f));
    for(unsigned int i = 0; i < this->geometries.size(); i += 1) {
        this->geometries[i]->render(&this->lightNodes);
    }
    this->geometries.clear();
    this->lightNodes.clear();
    glfwSwapInterval(1);
    glfwSwapBuffers(this->window);
}
void Engine::prepareGeometryForRendering(Geometry* geometry) {
    this->geometries.push_back(geometry);
}
void Engine::prepareLightNodeForRendering(LightNode* lightNode) {
    this->lightNodes.push_back(lightNode);
}
float Engine::getTime() {
    return(this->currentTime);
}
float Engine::getDeltaTime() {
    return(this->deltaTime);
}
float Engine::getCurrentFps() {
    return(this->currentFps);
}
void Engine::terminate() {
    glfwSetWindowShouldClose(this->window, GL_TRUE);
}
Engine::~Engine() {
    glfwTerminate();
    delete(this->input);
}
