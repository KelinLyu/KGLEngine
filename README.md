[![Banner](https://github.com/KelinLyu/KGLEngine/blob/main/KGLEngine/Resources/GitHub%20Images/Banner.png)](#)

# Introduction

This is a small but powerful OpenGL game engine I developed for my team for the course CSE 125 at UCSD. Although the engine is programmed in only a month, it supports most features of a modern game engine, such as playing skeletal animations, rendering shadows, adding complex particle systems, playing positional audios, etc. More importantly, it is very straightforward to use: you load the textures, models, and other types of assets to the engine and update their properties according to your game's logic, and that's it. You can try out the [released demo](https://github.com/KelinLyu/KGLEngine/releases) first. Although the engine is not perfect, it is suitable for doing small game projects as long as you are familiar with C++, GLSL, and one modeling software like MAYA. **By the way, I am working on a better engine based on my experiences learned from making this one.**

[![Demo](https://github.com/KelinLyu/KGLEngine/blob/main/KGLEngine/Resources/GitHub%20Images/Demo.png)](#)

# About & License & Acknowledgements

Developed by Kelin.Lyu. Licensed under the MIT license. I want to thank professor Geoffrey M. Voelker, TA Alexander Garza, my groupmates Felix Zhang, Jackie Li, Kelvin Yu, and especially Zifan Zhang for providing valuable feedback on this project. The free assets that the demo uses:
- Cinzel from [Google fonts](https://fonts.google.com).
- The ground's textures from [Textures.com](https://www.textures.com/library).
- A static cowboy model from [Creazilla.com](https://creazilla.com/nodes/1539732-cowboy-3d-model).
- Other characters and all the animations from [Mixamo.com](https://www.mixamo.com/#/).
- A guitar model from [Free3D.com](https://free3d.com/3d-model/acoustic-guitar-85235.html).
- The skybox's cubemap from the [Unity Asset Store](https://assetstore.unity.com/packages/2d/textures-materials/sky/allsky-free-10-sky-skybox-set-146014).
- Trees, stones, and other models from the [Unity Asset Store](https://assetstore.unity.com/packages/3d/environments/fantasy/ultimate-fantasy-creator-lite-45579).
- Bushes from the [Unity Asset Store](https://assetstore.unity.com/packages/3d/vegetation/plants/yughues-free-bushes-13168).

# Tutorial Catalog

- [Configurate the Development Environment](#configurate-the-development-environment)
- [Create the Engine, a Window, and the Render Loop](#create-the-engine-a-window-and-the-render-loop)
- [Load the Game's Assets](#load-the-games-assets)
- [Read User Inputs](#read-user-inputs)
- [More About the Engine Class](#more-about-the-engine-class)
- [Set up a Camera and Render Some Stuff](#set-up-a-camera-and-render-some-stuff)
- Change a Node’s Transform and Implement an FPS Camera System
- Load Textures and Models
- Render Skyboxes
- Use the Built-in Pbr Shader
- Create Light Nodes
- Render Shadows
- Play and Control Skeletal Animations
- Track Bone Nodes
- Add Particle Systems
- Add UI Nodes
- Render Images
- Load Font Files and Render Labels
- Copy, Clone, and Freeze Nodes
- More About the Node Class
- More About the Geometry Class
- More About the Shader Class
- Create Smooth Animations
- Play Static and Positional Audio Files

## Configurate the Development Environment

[Tutorial Catalog](#tutorial-catalog)

The engine depends on the following libraries:
- GLFW for creating a window and receiving user inputs.
- GLEW for running OpenGL.
- GLM for doing linear algebra.
- STB (stb_image.h) for loading images.
- Assimp for importing models and animations.
- Freetype for loading font files.
- SFML for loading and playing audio files.

There are already many tutorials about linking libraries so I will go over everything quickly. For developers using a Mac computer like me, do the following:
1. Install Xcode.
2. Install Homebrew.
3. With homebrew, install GLFW, GLEW, GLM, Assimp, Freetype, and SFML.
4. Open Xcode and create a macOS Command Line Tool project.
5. Download the engine's source files.
6. Drag the KGLEngine folder (which directly contains the Engine.hpp file) to the main.cpp's location in Xcode. Make sure that **Copy items if needed** is selected.
7. Download the stb_image.h from GitHub and copy it to the KGLEngine/Toolkit folder.
8. Go to the project's Build Settings.
9. For **Header Search Paths**, type the Homebrew's include folder. For me, this is /opt/homebrew/include/. Also, the Freetype library needs a separate one, which is /opt/homebrew/Cellar/freetype/2.12.0/include/freetype2/.
10. For the **Library Search Paths**, type the lib folders of the six libraries. For example, for the GLFW library, this is /opt/homebrew/Cellar/glfw/3.3.6/lib/. For other libraries, this is pretty much the same.
11. For the **Validate Workspace - Ignored Frameworks**, type OpenGL.
12. For the **Documentation Comments**, change the value to No.
13. For the **Enabled Hardened Runtime**, change the value to No.
14. Go to the project's Build Phases and look at the **Link Binary With Libraries** section.
15. Click the + icon and add the OpenGL.framework.
16. For each path you typed in the **Library Search Paths** in the project's Build Settings **except for the GLM library**, drag the dylib file under that path into the linked binaries list (or you can still click the + icon, click Add Other, and locate the dylib files). Most lib folders contain only one actual dylib file, while the others are shortcuts. However, for the SFML library, you will see many of them. Only the libsfml-audio and libsfml-system are needed.
17. Include KGLEngine/Engine.hpp in main.cpp and build the project.

For Windows developers, here are the steps to create a Visual Studio project. I admit that I had never used Visual Studio before, so my way of creating a new project might look inelegant.
1. Download all the required libraries.
2. Build the libraries if needed. If you are a lazy person like me, you can use Assimp's old binary, but some of the model formats are unsupported or buggy. You have to compile Freetype, but since an sln file is provided, you can simply open it with Visual Studio and build the project. Make sure to build in Release mode.
3. Download the engine's source files.
4. Download the stb_image.h from GitHub and copy it to the KGLEngine/Toolkit folder.
5. Drag the KGLEngine folder (which directly contains the Engine.hpp file) to the project's Source Files folder so that Visual Studio will import all the files inside the KGLEngine folder.
6. Open the project's Property Page by clicking **Project** - **Properties**.
7. Under **Configuration Properties** - **C/C++** - **General** - **Additional Include Directories**, add the paths to all the include folders of the libraries.
8. Under **Configuration Properties** - **Linker** - **General** - **Additional Library Directories**, add the paths to all the libraries' folders that contain the lib files **except for the GLM library**. The names of the folders are different, for example, for the GLFW library, the folder is titled lib-vc2019.
9. Under **Configuration Properties** - **Linker** - **Input** - **Additional Dependencies**, type the following lib files: glew32s.lib, glfw3.lib, OpenGL32.lib, assimp.lib, freetype.lib, sfml-audio.lib, and sfml-system.lib.
10. Include the Engine.hpp in the cpp file containing the main function. Note that you can type "../" to jump to the parent directory.
11. Build the project.

If building the project produces no errors, you are good to go.

## Create the Engine, a Window, and the Render Loop

[Tutorial Catalog](#tutorial-catalog)

The first step you need to do is to create an instance of the engine inside the main function, which will subsequently create a window:
```
Engine* engine = new Engine("KGLEngine Demo", 0.8f, 0, "/Resources/Icon.png");
```
Parameters:
- The title of the window.
- The scale factor of the window. If the value is between 0 and 1, the window's size will be the scale factor multiplying the size of the monitor. If the value is 1, the window will appear in fullscreen mode.
- The Multisampling factor.
- The path to the icon image of the window, or NULL if you do not want one. Read the upcoming chapter about how to specify the paths to the resource files.

Next, you need to create a render loop:
```
while(engine->isRunning()) {
    if(engine->shouldUpdate()) {

        // Implement the game's logic...

        engine->render();
    }
}
```
And that's it. Build and run the project, and you will see an empty window.

## Load the Game's Assets

[Tutorial Catalog](#tutorial-catalog)

Of course, your game needs to import assets like images, models, and audio files. Therefore, you need to tell the engine where it can find them. By default, the engine looks at the executable's directory for the files, which is the **current working directory**. You can also manually change the engine's working directory to somewhere else:
```
engine->workingDirectory = "/Desktop/Project"
```
Note that the engine's workingDirectory variable should **not** contain a slash at the end. And when loading files, you should always include a slash at the beginning of the paths.

## Read User Inputs

[Tutorial Catalog](#tutorial-catalog)

It is very straightforward to handle user inputs. For detecting continuous keyboard inputs, write the following code inside the render loop:
```
if(engine->input->isPressingKey(KEY_W)) {
    // Move forward!
}
```
For detecting key press or release only **once**, use:
```
if(engine->input->wasKeyReleased(KEY_ESCAPE)) {
    engine->terminate();
}

// Or try the following:
// if(engine->input->wasKeyPressed(KEY_ESCAPE)) {...}
```
The above functions only trigger once after the player press or release a key. More specifically, after the player presses the key and you ask the engine whether the key was pressed, the engine returns true and clears the record of the key so that in the next frame, the engine will return false when you ask the same question. However, if you are uninterested in the state of that key, the record will remain untouched. Therefore, sometimes you must call the above functions to clear a key's record explicitly. For example, you want the player to press a key as soon as she enters a new scene to dodge a rocket, but if she has been holding that key all the time and you forgot to clear the key's record before the rocket launches, the player will automatically dodge that rocket.

You can also get the duration since the time when the state of a key changed:
```
// First, you need to get the current time from the engine:
double currentTime = engine->getTime();

// Then, let's print out how long the user has been pressing the space bar:
if(engine->input->isPressingKey(KEY_SPACE)) {
    double duration = engine->input->getKeyDuration(KEY_SPACE, currentTime);
    cout << "You have been holding space for " << duration << "s." << endl;
}
```
If you want to implement a text box, using the methods above for text input might be complex. Therefore, you can use the following function that returns the recently pressed key's character. After getting the character, the engine reset it to the empty string. Note that this character will be capitalized if the user is holding shift.
```
string character = engine->input->getLastPressedCharacter();
if(character.length() > 0) {
    cout << "You typed [" << character << "]." << endl;
}
```
You are probably asking, what about mouse events? Well, you can detect mouse clicks just like the keyboard events. You simply need to replace the key's name with MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, or MOUSE_BUTTON_MIDDLE.
To get the mouse's position and translation after the previous frame, use:
```
vec2 position = engine->input->getMouseScreenPosition();
vec2 translation = engine->input->getMouseTranslation();
```
The mouse's position is called the screen position and is connected to the UI system of the engine. It will be discussed in future chapters.

You can also get the acceleration of the mouse's scroll wheel as well, by calling:
```
float acceleration = engine->input->engineSetScrollWheelEvent();
```
These are all the interactions that have been implemented. **The interaction logic should only be implemented inside the if statement in the while loop.**

## More About the Engine Class

[Tutorial Catalog](#tutorial-catalog)

Here are some helper methods of the Engine class that you might find helpful. First, here comes the cursor functions:
```
engine->hideCursor();
engine->showCursor();
engine->lockCursor();
engine->unlockCursor();
```
The first two functions are straightforward and helpful if you want to implement a custom cursor appearance. The third function locks the cursor to the center of the screen and automatically hides it. This is necessary if you're developing an FPS or TPS game where the screen's boundaries should not limit the mouse's movement. Finally, the last function unlocks the cursor and shows it if you didn't hide the cursor.

There are also some getters of the Engine class. For example, you will find the following methods in the engine's header file: 
```
vec2 getScreenResolution();
vec2 getWindowResolution();
float getTime();
float getDeltaTime();
float getCurrentFPS();
```
I think the names of these functions already explain their functionalities, so I won't waste any time discussing them. Note that the only way to close a window is by calling the terminate method:
```
engine->terminate();
```
As a busy UCSD student, I probably won't have time to explain every variable and function in this tutorial. I will only talk about the most important features of the engine. Therefore, you are encouraged to look at the hpp files yourself to learn more about a class. Although I didn't write any documentation, I always kept the names of variables, functions, and parameters as specific as possible. For example, the terrifyingly-long function below simply does what it says to do–rendering the shadow map of a directional light node:
```
void renderDirectionalLightShadowMap(LightNode* directionalLightNode);
```
Unless you want to understand the structure of the engine and how it works, you do not need to read the private members of a class and all the functions starting with the word "engine" like the one below. These functions usually appear at the end of a header file, and calling these functions may result in undefined behavior.
```
void engineUpdateGeometrySkeletalAnimations(vector<mat4> boneTransforms);
``` 

## Set up a Camera and Render Some Stuff

[Tutorial Catalog](#tutorial-catalog)

