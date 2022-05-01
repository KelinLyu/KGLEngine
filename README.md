[![Banner](https://github.com/KelinLyu/KGLEngine/blob/main/KGLEngine/Resources/GitHub%20Images/Banner.png)](#)
# Introduction
This is a small but powerful OpenGL game engine I developed for my team for the course CSE 125 at UCSD. Although the engine is programmed in only a month, it supports most features of a modern game engine, such as playing skeletal animations, rendering shadows, adding complex particle systems, playing positional audios, etc. More importantly, it is very straightforward to use: you load the textures, models, and other types of assets to the engine and update their properties according to your game's logic, and that's it. You can try out the [released demo](https://github.com/KelinLyu/KGLEngine/releases) first. Although the engine is not perfect, it is suitable for doing small game projects as long as you are familiar with C++, GLSL, and one modeling software like MAYA. By the way, I am working on a better engine based on my experiences learned from making this one.
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

# Configurate the Development Environment
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
