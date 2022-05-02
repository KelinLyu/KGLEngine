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
- [Read Player Inputs](#read-player-inputs)
- [More About the Engine Class](#more-about-the-engine-class)
- [Set up a Camera and Render Some Stuff](#set-up-a-camera-and-render-some-stuff)
- [Change a Node’s Transform and Hide a Node](#change-a-nodes-transform-and-hide-a-node)
- [Load Textures and Models](#load-textures-and-models)
- [More About the Geometry Class](#more-about-the-geometry-class)
- [Render Skyboxes](#render-skyboxes)
- [Use the Built-in PBR Shader and Helpful Shader Methods](#use-the-built-in-pbr-shader-and-helpful-shader-methods)
- [Create Light Nodes](#create-light-nodes)
- [Render Shadows](#render-shadows)
- [Play and Control Skeletal Animations](#play-and-control-skeletal-animations)
- [Track Bone Nodes](#track-bone-nodes)
- [Add Particle Systems](#add-particle-systems)
- Add UI Nodes
- Render Images
- Load Font Files and Render Labels
- Copy, Clone, and Freeze Nodes
- More About the Node Class
- More About the Shader Class
- Create Smooth Animations
- Play Static and Positional Audio Files
- Advanced Tips
- Release Your Game

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

// Delete the engine after the game is terminated:
delete(engine);
```
And that's it. Build and run the project, and you will see an empty window.

## Load the Game's Assets

[Tutorial Catalog](#tutorial-catalog)

Of course, your game needs to import assets like images, models, and audio files. Therefore, you need to tell the engine where it can find them. By default, the engine looks at the executable's directory for the files, which is the **current working directory**. You can also manually change the engine's working directory to somewhere else:
```
engine->workingDirectory = "/Desktop/Project"
```
Note that the engine's workingDirectory variable should **not** contain a slash at the end. And when loading files, you should always include a slash at the beginning of the paths.

## Read Player Inputs

[Tutorial Catalog](#tutorial-catalog)

It is very straightforward to handle user interactions. For detecting continuous keyboard inputs, write the following code inside the render loop:
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

The engine won't render anything if you don't specify the main camera node. Therefore, you have to create one and set the engine's mainCameraNode variable:
```
CameraNode* cameraNode = new CameraNode(60.0f, 0.1f, 1000.0f);
engine->addNode(cameraNode);
engine->mainCameraNode = cameraNode;
```
The parameters for the constructor are:
- An angle in degrees that indicates the camera's field of view. 
- The distance of the near plane.
- The distance of the far plane.

After creating a camera node, you should attach it to the engine by calling the engine's addNode method. Otherwise, other nodes attached to the camera will not be rendered. Of course, you can also attach the camera node under another node, which is pretty common if you want the camera to follow a character, for instance. Finally, you should set the engine's mainCameraNode so that the engine knows which camera it should use. You can create multiple camera nodes and switch the camera using this method.

**Note that my engine uses the positive X-axis as the front, unlike most engines and modeling software that uses the positive Z-axis. Therefore, the camera points towards the positive X-axis by default, and it is the same for the directional lights and particle systems. Therefore, you will have to rotate these nodes to change their default orientation.**

Now, let's create an FPS camera system. We will load a cube and let the player smoothly control the camera by pressing the W/S/A/D/Q/E keys (forward, backward, leftward, rightward, downward, and upward). You can find more details about the technologies used in this example in future chapters.

First, right after the line where you create the engine, you need to lock the cursor:
```
engine->lockCursor();
```
Then, create the camera node. Note that we need to adjust its position and also create two variables. We will transform the camera node to achieve the target position and Euler angles in every frame to get smooth movements and rotations:
```
CameraNode* cameraNode = new CameraNode(60.0f, 0.1f, 1000.0f);
cameraNode->position = vec3(-3.0f, 0.0f, 0.0f);
engine->addNode(cameraNode);
engine->mainCameraNode = cameraNode;
   
vec3 cameraTargetPosition = cameraNode->position;
vec3 cameraTargetEulerAngles = cameraNode->eulerAngles;
```
Finally, add a simple cube in front of the camera so that when you test the program, you will see that the camera is actually moving:
```
PBRShader* shader = new PBRShader(0.5f, 0.5f);
   
Node* cubeNode = new Node();
cubeNode->loadUnitCube();
cubeNode->geometries[0]->setShader(shader);
engine->addNode(cubeNode);
```
Now, let's implement the logic inside the main loop. First, since we have locked the cursor, we need a keyboard shortcut for termination:
```
if(engine->input->wasKeyPressed(KEY_ESCAPE)) {
    engine->terminate();
}
```
Then, we update the target position and Euler angles according to player inputs:
```
// Update target position:
if(engine->input->isPressingKey(KEY_W)) {
    cameraTargetPosition += cameraNode->getFrontVectorInWorld() * 0.2f;
}else if(engine->input->isPressingKey(KEY_S)) {
    cameraTargetPosition += cameraNode->getBackVectorInWorld() * 0.2f;
}
if(engine->input->isPressingKey(KEY_A)) {
    cameraTargetPosition += cameraNode->getLeftVectorInWorld() * 0.2f;
}else if(engine->input->isPressingKey(KEY_D)) {
    cameraTargetPosition += cameraNode->getRightVectorInWorld() * 0.2f;
}
if(engine->input->isPressingKey(KEY_E)) {
    cameraTargetPosition += cameraNode->getUpVectorInWorld() * 0.2f;
}else if(engine->input->isPressingKey(KEY_Q)) {
    cameraTargetPosition += cameraNode->getDownVectorInWorld() * 0.2f;
}
       
// Update target euler angles:
vec2 mouseTranslation = engine->input->getMouseTranslation() * 0.1f;
cameraTargetEulerAngles.y -= mouseTranslation.x;
cameraTargetEulerAngles.z -= mouseTranslation.y;

// Prevent overflow:
cameraTargetEulerAngles.z = glm::max(-60.0f, glm::min(cameraTargetEulerAngles.z, 60.0f));
```
After that, we will create two animations to update the camera's position and Euler angles according to the target position and Euler angles. The Animation class will be explained in a future chapter.
```
// Smooth movement:
Animation* moveAnimation = new Animation("moveCamera", 0.2f);
moveAnimation->setVec3Animation(&cameraNode->position, cameraTargetPosition);
engine->playAnimation(moveAnimation);
       
// Smooth rotation:
Animation* rotateAnimation = new Animation("rotateCamera", 0.1f);
rotateAnimation->setEulerAnglesAnimation(&cameraNode->eulerAngles, cameraTargetEulerAngles);
engine->playAnimation(rotateAnimation);
```
Finally, outside the main loop, don't forget to delete all the allocated resources:
```
delete(cameraNode);
delete(shader);
delete(cubeNode);
```
Now, build and run the program.

# Change a Node’s Transform and Hide a Node

[Tutorial Catalog](#tutorial-catalog)

You can simply change a node's transform via modifying its position, eulerAngles, and scale variables. Note that all the angles you provide to the engine must use degrees instead of radians.

When you move, rotate, or scale a node, all the child nodes of that node will follow its transformation. In other words, the child nodes will transform inside the parent's coordinate system.

If you want a node always to face another node, such as a billboard effect, you can set the node's orientationTargetNode. By default, the orientationTargetNode is set to NULL. After you set it to another node in the scene, the node will constantly overwrite its rotation so that its positive Z-axis points towards the target node. However, since the engine defines the positive X-axis as the front (which is a design mistake), you must introduce another node. For example, suppose you want a camera node to look at a cube node using the orientationTargetNode. In that case, you need a control node and set the orientationTargetNode of the control node. Finally, attach the camera node to the control node and modify the camera node's rotation.

As you already know, you have to attach a node to the engine or another node. Otherwise, the node and all its child nodes won't be rendered.
```
engine->addNode(nodeA);
nodeA->addChildNode(nodeB);
```
To detach a node from the parent node, call the node's removeFromParentNode method:
```
nodeA->removeFromParentNode();
nodeB->removeFromParentNode();
```
If you delete a node, it will automatically detach it. However, its child nodes will have dangling pointers pointing to the deleted node.

You can access the parent node and child nodes of a node. They are declared in the following way in the Node's hpp file:
```
Node* parent;
vector<Node*> childNodes;
```
These fields are read-only. If you wish to modify them, you must call the addNode, addChildNode, and removeFromParentNode methods.

Finally, if you want to hide a node temporarily without playing with the node structures, you should use the isDisabled variable:
```
nodeA->isDisabled = true;
nodeA->isDisabled = false;
```

# Load Textures and Models

[Tutorial Catalog](#tutorial-catalog)

This chapter will continue to use the cube and the FPS camera system achieved in the previous chapters.

First, you need to know how to load textures. The shaders require textures to achieve specific visual effects. You can load a texture in the following way:
```
Texture* texture = new Texture("/Resources/Image.png", 2.0f, true);
```
The parameters are:
- The relative path of the target image file to load. Theoretically, all the image formats are supported, but I only tested and used the png format.
- The anisotropic filtering factor. A value of 0.0f disables anisotropic filtering.
- Whether to generate mipmaps for the texture.

To test the texture loaded, for now, we attach the texture as the diffuse map of the shader object:
```
PBRShader* shader = new PBRShader(0.5f, 0.5f);
shader->setDiffuseMap(texture);
```
Loading a large texture file takes time, so you should always prepare all the textures in the game's loading stage. In addition, you should try your best to reduce the number of textures by reusing the same texture file or merging them using image-editing and modeling software.

And don't forget to delete the texture when you no longer need it:
```
delete(texture);
```

Loading 3D model files are pretty much the same. First, remove the line that calls the loadUnitCube method. Then, load the model by calling the node's loadModelFile method, providing only the file's relative path. Finally, set the shader of every geometry of the node. You can check out the main.cpp of the demo program for how it is done:
```
sceneNode = new Node();
sceneNode->loadModelFile("/Resources/Demo/Scene/Model.dae");
sceneNode->geometries[0]->setShader(supportShader);
sceneNode->geometries[1]->setShader(groundShader);
sceneNode->geometries[2]->setShader(barkShader);
sceneNode->geometries[3]->setShader(rockShader);
sceneNode->geometries[4]->setShader(campShader);
//...
```
You have to manually set the shaders for all the geometries, or the geometry will appear in purple. I recommend you test one geometry at a time to avoid setting the wrong shader to a geometry. You should also print out the size of the node's geometry vector ahead of time.

There is no need to delete the geometries of a node because deleting the node itself automatically deletes all its geometries (unless the node is a clone of another node, which will be discussed in a future chapter).

# More About the Geometry Class

[Tutorial Catalog](#tutorial-catalog)

If you want to hide one geometry but still let the engine render other geometries of the node, you can set the geometry's isHidden variable:
```
node->geometries[2]->isHidden = true;
```

You can also specify a rendering order of a geometry. For opaque geometries, this is unnecessary. However, if the geometry has a semi-transparent shader, you must make the geometry's renderingOrder higher than all other geometries behind it. The engine cannot sort the geometries by their distance because the geometries might have overlapping components.
```
node->geometries[5]->renderingOrder = 20.0f;
```

The Geometry class also has two bit-mask variables. The first one is the renderingBitMask. Both the CameraNode class and the Geometry class have this field. When the engine renders a geometry, it performs an AND operation between the current camera's renderingBitMask and that of the geometries. If the result is zero, the engine will not render the geometry. 
```
node->geometries[4]->renderingBitMask = 2;
cameraNode->renderingBitMask = 4;
```
The second is the lightingBitMask, which specifies whether a light node can affect a geometry. 

The default value for the two bit-masks is -1, which sets all bits to 1. By default, the geometry is rendered by all the cameras and is affected by all the light nodes.

Finally, you should know about the boundingSpherePosition and the boundingSphereRadius variables. When a node loads a model file, the engine tries to build a bounding sphere that covers the entire geometry. And when determining whether a point light affects the geometry, the engine compares the sum of the sphere's radius and the light's radius with the distance between the light and the node. If the distance is larger than the sum, the light will not affect the geometry, making your game more efficient.

However, sometimes you need to manually edit the bounding sphere's position and radius. For example, if you play a skeletal animation that causes a character's head to grow ten times larger, the geometry will undoubtedly exceed the bounding sphere. In this case, you should increase the size of the bounding sphere:
```
node->geometries[3]->boundingSphereRadius *= 5.0f;
```
You can print out the geometry's affectedLightCount to check the number of lights affecting the geometry.

# Render Skyboxes

[Tutorial Catalog](#tutorial-catalog)

It is straightforward to create a skybox:
```
Skybox* skybox = new Skybox("/Resources/Front.png", 
                            "/Resources/Back.png",
                            "/Resources/Up.png",
                            "/Resources/Down.png",
                            "/Resources/Left.png",
                            0.0f);
engine->skybox = skybox;
```
The first six parameters are the image files of all the sides of the cube. The last parameter is the anisotropic filtering factor.

Note that the order of the images might be different. For example, for some skyboxes, the left and right images are the front and back sides. If you see that the skybox looks incorrect, you should adjust the order. And similar to camera nodes, you can switch different skyboxes by setting the engine's skybox.

# Use the Built-in PBR Shader and Helpful Shader Methods

[Tutorial Catalog](#tutorial-catalog)

The built-in PBR shader can help you develop the game very quickly. However, you are strongly recommended to write your own shader files so that your game can be better optimized. Please read the shader chapter about that.

When constructing the built-in PBR shader, you need to provide two parameters: the metallic factor and the roughness factor, both between 0.0f and 1.0f.

Then, you can set the shader's diffuse behavior using the following variables and functions in the Shader class's hpp:
```
vec4 diffuseColor;
float diffuseIntensity;
void setDiffuseMap(Texture* texture);
```
Things are very straightforward. Note that the shader will ignore the diffuse color after setting the diffuse map.

The following variable and function set the normal map:
```
float normalIntensity;
void setNormalMap(Texture* texture);
```
And for the heightmap:
```
float heightIntensity;
vec2 heightLayerRange;
void setHeightMap(Texture* texture);
```
The heightLayerRange specifies the range of steps to iterate when calculating the height of a fragment. The less, the more efficient. The higher, the more accurate.

Next, you can also provide a metallic map and a roughness map:
```
float metallic;
float metallicIntensity;
bool invertMetallic;
void setMetallicMap(Texture* texture);

float roughness;
float roughnessIntensity;
bool invertRoughness; 
void setRoughnessMap(Texture* texture);
```
Note that the invert variables askes the shader to calculate one minus the actual value for the final result.

Finally, here are the remaining fields:
```
float reflectionIntensity;
void setReflectionMap(Texture* texture);

float ambientOcclusionIntensity;
void setAmbientOcclusionMap(Texture* texture);

vec3 multiplyColor;
float multiplyIntensity;
void setMultiplyMap(Texture* texture);

vec3 emissionColor;
float emissionIntensity;
void setEmissionMap(Texture* texture);
```
Note that the setReflectionMap method only accepts a spherical map.

The Shader class itself also has some useful methods. For example, the following functions modify the blend mode:
```
void setOpaque();
void setAdditive();
void setSemitransparent();
```
By default, the engine renders the shader in opaque mode. If you set the shader as additive or semitransparent, you need to set the geometry's rendering order. Sometimes you also need to change the following variable to false if your geometries have many components overlapping with each other.
```
shader->writeToDepthBuffer = false;
```
If you are making an FPS game where the character's hand and weapon should always be on top of everything else, you should clear the depth buffer before rendering the shader. This is also helpful if you want to have 3D models on top of UI images.
```
shader->clearDepthBuffer = true;
```

# Create Light Nodes

[Tutorial Catalog](#tutorial-catalog)

Creating lights is similar to creating cameras. First, you create an instance of a light node, providing only the color factor as the parameter. The color factor indicates how intense each RGB channel is, and the value does not have an upper bound. Then, you attach the light node to the engine or another node in the scene.
```
LightNode* lightNode = new LightNode(vec3(5.0f, 5.0f, 10.0f));
engine->addNode(lightNode);
```
Now, you need to specify the light's behavior. Here are the methods:
```
void setAmbientLight();
void setDirectionalLight();
void setPointLight(float attenuationExponent, float range);
void setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle); 
```
The ambient light lights up all the pixels evenly. The directional light shoots parallel light rays and can cause shadows. The point light emits light rays in every direction within a given range, and the spotlight shoots light rays inside a cone within a range. Note that the attenuation exponent controls how the light fades as the object moves away. The formula used here is simply the following:
```
float progress = (range - currentDistance) / range;
float lightIntensity = progress^attenuationExponent;
```
As I mentioned before, the directional lights and spotlights point towards the positive X-axis by default.

If you wish to increase, reduce, or remove the highlight caused by a light node, you can modify its highlight's intensity:
```
lightNode->highlightIntensity = 0.5f;
```
The penetration range allows the pixels inside to shine, ignoring the normal vector's direction. 
```
lightNode->penetrationRange = 1.0f;
```
The penetration range only works with point lights and spotlights. By default, this variable is set to zero. Also, the same attenuation exponent applies to the penetration range as well.

Finally, the lightingBitMask has already been introduced in a previous chapter. And the shadowBitMask will be introduced in the next chapter.

Note that the maximum number of lights you can have affecting a single geometry is 30.

# Render Shadows

[Tutorial Catalog](#tutorial-catalog)

In the current version, only directional light nodes can render shadows. To do so, first, call the activateDirectionalLightShadow method of the light node after setting it as directional light:
```
void activateDirectionalLightShadow(unsigned int mapSize, 
                                    float projectionSize, 
                                    float near, 
                                    float far, 
                                    float xOffset, 
                                    float bias, 
                                    int samples);
```
About the parameters:
- The mapSize is the size of the shadow map in pixels. The larger, the more accurate, but it will take longer to render.
- The projectionSize is a distance in meters. It limits the boundaries of the shadow area.
- For the near, far, and xOffset parameters, since rendering shadows is simply done by rendering the scene's depth through an orthogonal camera within the directional light node, the engine has to generate a camera node as well. The near and far variables control the distance of the camera's near and far planes. The xOffset parameter moves the camera along the X-axis. Usually, you should set the xOffset with a negative value.
- The bias is simply the shadow bias, a common technique for rendering shadows.
- The samples variable controls how blurry the shadows are.

After calling the above function, you should also render the shadows manually before you call the engine's render function in the main loop:
```
engine->renderDirectionalLightShadowMap(lightNode);
```
The only argument is a pointer to the directional light node.

Note that the maximum number of shadow maps you can have is 6.

# Play and Control Skeletal Animations

[Tutorial Catalog](#tutorial-catalog)

The engine makes it extremely easy to load, play, and control skeletal animations. However, it requires you to be familiar with modeling software like MAYA.

First, you should prepare a rigid 3D model file. Although the Assimp library accepts almost every format, it fails to import some of the formats very often. After conducting many experiments, I realize that the DAE format is the most stable, but I am not sure whether it's the same for you. Therefore, I strongly encourage you to compile and test Assimp by yourself.

Second, you need to prepare a loopable base animation, for example, an idle animation for a character. The animation should have the exact bone structure as the model file. Having fewer or extra bones may result in undefined behavior. Also, all the bones require a least one keyframe. The most secure way of achieving this is to have the first and last frame added for every bone.

Now, you can load the base animation by calling the loadAnimator method in the following way, providing a unique name for the animation and the relative path of the file:
```
Animator* animator = cowboyNode->loadAnimator("playing", "/Resources/Demo/Cowboy/Animation.dae");
```
The loadAnimator function returns an animator that controls the loaded animation. Then you can simply play and stop the animation, providing fade-in and fade-out durations:
```
animator->play(0.5f);
animator->stop(1.0f);

// Provide both the fade-in and fade-out durations for sudden animations:
animator->play(0.5f, 0.5f);
```
You can now load other animations on top of the base animation. You do not need to set keyframes for every bone in other animations. For example, it is totally fine to have an animation that only animates a character's lower body. Just remember that the order you load the animations matters. The newly-loaded animations cover the old ones.

There are two types of animations: loopable animations and sudden animations. The loopable animations (idle, moving, falling, etc.) will not stop once you play them, and you have to call the stop method manually. On the other hand, sudden animations (jumping, attacking, impacted, dying, etc.) will fade out and stop automatically when they are about to end. You can set the animator's repeats property to tell the engine whether the animation is loopable or sudden:
```
animator->repeats = true;
```
By default, an animator's repeats variable is set to false. After switching its value to true, you should provide both the fade-in and fade-out durations when calling the play method. You can still stop the animation early by calling the stop method.

Some sudden animations, such as the dying animation, need to stop at the final frame. You can achieve this by setting the clamps variable:
```
animator->clamps = true;
```

You can also set and update the following fields:
```
float timeOffset;
float speed;
float blendFactor;
```
The timeOffset variable allows the animation to start early or late. For example, when a large crowd of characters is playing the same animation, it is better to randomize the timeOffset for every character so that the animation will look different. The speed variable is straightforward. The blendFactor controls the animation's intensity with a value between 0 and 1.

There are also some helper methods. For example, you can check whether the animation is playing or not, get its time progress, reads its total duration, and get its current blend factor. 
```
bool isPlaying();
float getTime();
float getDuration();
float getCurrentBlendFactor();
```
Here is an example of the use of the last function. If you want the character to move forward while playing the running animation, it is better to multiply its moving speed with the running animation's current blend factor. In this way, the speed also increases or decreases when the animation fades in or fades out.

You don't have to keep all the animator pointers because you can ask the node for it:
```
Animator* animator = node->getAnimator("idle");
```
The getAnimator is an O(n) algorithm that simply iterates through all the animators of the node and returns the one with the name that matches the parameter. Therefore, if you want to control a large group of animators, you should avoid using the above method. Instead, you should do the following:
```
animator1->animatorBitMask = 2;
animator2->animatorBitMask = 4;
//...

node->playAnimators(6, 0.5f);
node->stopAnimators(6, 0.5f);

// Provide the optional fade-out duration: 
node->playAnimators(6, 0.5f, 1.0f);
```
In short, you can set the animator's animatorBitMask, and ask the node to play and stop the animators for you by providing a bit-mask as the first argument. The node then performs an AND operation using the provided bit-mask and all the animator's animatorBitMasks. If the result is not zero, the node plays or stops the animation.

# Track Bone Nodes

[Tutorial Catalog](#tutorial-catalog)

When you want to attach a weapon to a character's hand or achieve something similar, you need to implement bone tracking. By default, the engine does not calculate the transform of a bone node unless you explicitly request it in the following way:
```
Node* boneNode = characterNode->generateBoneNode("RightHand");
```
The parameter of the generateBoneNode method must match the actual name of the bone you assigned in the modeling software.

After generating the bone node, you can add other nodes to it. The bone node's position, rotation, and scale are fixed because the engine will overwrite them when it updates the node's animators.

# Add Particle Systems

[Tutorial Catalog](#tutorial-catalog)
[Tutorial Catalog](#tutorial-catalog)
[Tutorial Catalog](#tutorial-catalog)
