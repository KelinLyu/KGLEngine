// Developed by Kelin Lyu.



#include "KGLEngine/Engine.hpp"



int main(int argc, char** argv) {
    
    
    
    // Construct the engine:
    Engine* engine = new Engine("KGLEngine Demo", 0.8f, 0, "/Resources/Icon.png");
    
    
    
    // If the Resources folder is not in the executable's directory,
    // you need to manually set the absolute path of the directory
    // containing that folder:
    //
    // engine->workingDirectory = "/Desktop/KGLEngine/KGLEngine";
    
    
    
    // Hide and lock the cursor to the center of the screen:
    engine->lockCursor();
    
    
    
    // The variable that controls the application:
    unsigned int applicationState = 1;
    // 0 = idle
    // 1 = entry point
    // 2 = load resources
    
    
    
    // The variable that controls the loading progress (from 0 to 100):
    unsigned int loadingState = 0;
    
    
    
    // The variables that control and loop the audios:
    float ambientAudioVolumeFactor = 0.0f;
    unsigned int ambientAudioState = 0;
    unsigned int bonfireAudioState = 0;
    float playingAnimatorTime = 0.0f;
    unsigned int guitarAudioState = 0;
    
    
    
    // Set up the camera node:
    CameraNode* cameraNode = new CameraNode(60.0f, 0.1f, 1000.0f);
    cameraNode->position = vec3(3.098423f, 1.624609f, 10.102654f);
    cameraNode->eulerAngles = vec3(0.0f, 105.199883f, 7.000003f);
    engine->addNode(cameraNode);
    engine->mainCameraNode = cameraNode;
    
    // Variables that control the camera:
    bool cameraControl = true;
    float cameraControlFactor = 0.0f;
    vec3 cameraTargetPosition = cameraNode->position;
    vec3 cameraTargetEulerAngles = cameraNode->eulerAngles;
    
    
    
    // The font file to load:
    Font* title = NULL;
    
    
    
    // Textures to load:
    Texture* logoMap = NULL;
    
    Texture* sceneReflectionMap = NULL;
    Texture* sceneMultiplyMap = NULL;
    
    Texture* groundDiffuseMap = NULL;
    Texture* groundNormalMap = NULL;
    Texture* groundHeightMap = NULL;
    Texture* groundMetallicMap = NULL;
    Texture* groundAmbientOcclusionMap = NULL;
    
    Texture* barkDiffuseMap = NULL;
    Texture* barkNormalMap = NULL;
    Texture* barkAmbientOcclusionMap = NULL;
    
    Texture* rockDiffuseMap = NULL;
    Texture* rockNormalMap = NULL;
    Texture* rockAmbientOcclusionMap = NULL;
    
    Texture* campDiffuseMap = NULL;
    Texture* campNormalMap = NULL;
    
    Texture* treeDiffuseMap = NULL;
    Texture* treeNormalMap = NULL;
    Texture* treeAmbientOcclusionMap = NULL;
    
    Texture* detailDiffuseMap = NULL;
    Texture* detailNormalMap = NULL;
    Texture* detailMetallicMap = NULL;
    Texture* detailAmbientOcclusionMap = NULL;
    
    Texture* propDiffuseMap = NULL;
    Texture* propNormalMap = NULL;
    Texture* propAmbientOcclusionMap = NULL;
    
    Texture* leavesDiffuseMap = NULL;
    Texture* leavesNormalMap = NULL;
    
    Texture* bush1DiffuseMap = NULL;
    Texture* bush1NormalMap = NULL;
    Texture* bush2DiffuseMap = NULL;
    Texture* bush2NormalMap = NULL;
    Texture* bush3DiffuseMap = NULL;
    Texture* bush3NormalMap = NULL;
    Texture* bush4DiffuseMap = NULL;
    Texture* bush4NormalMap = NULL;
    Texture* bush5DiffuseMap = NULL;
    Texture* bush5NormalMap = NULL;
    
    Texture* fireMap = NULL;
    Texture* smokeMap = NULL;
    Texture* sparkMap = NULL;
    
    Texture* cowboyDiffuseMap = NULL;
    
    Texture* guitarWood1DiffuseMap = NULL;
    Texture* guitarWood2DiffuseMap = NULL;
    
    Texture* SWAT1DiffuseMap = NULL;
    Texture* SWAT1NormalMap = NULL;
    Texture* SWAT2DiffuseMap = NULL;
    Texture* SWAT2NormalMap = NULL;
    Texture* SWAT3DiffuseMap = NULL;
    Texture* SWAT3NormalMap = NULL;
    
    Texture* girlDiffuseMap = NULL;
    Texture* girlNormalMap = NULL;
    Texture* girlMetallicMap = NULL;
    Texture* hairDiffuseMap = NULL;
    
    Texture* zombie1DiffuseMap = NULL;
    Texture* zombie1NormalMap = NULL;
    Texture* zombie2DiffuseMap = NULL;
    Texture* zombie2NormalMap = NULL;
    
    Texture* sister1DiffuseMap = NULL;
    Texture* sister1NormalMap = NULL;
    
    
    
    // Shaders to load:
    PBRShader* supportShader = NULL;
    PBRShader* groundShader = NULL;
    PBRShader* barkShader = NULL;
    PBRShader* rockShader = NULL;
    PBRShader* campShader = NULL;
    PBRShader* treeShader = NULL;
    PBRShader* detailShader = NULL;
    PBRShader* propShader = NULL;
    PBRShader* leavesShader = NULL;
    PBRShader* bush1Shader = NULL;
    PBRShader* bush2Shader = NULL;
    PBRShader* bush3Shader = NULL;
    PBRShader* bush4Shader = NULL;
    PBRShader* bush5Shader = NULL;
    
    PBRShader* cowboyHandsShader = NULL;
    PBRShader* cowboyBodyShader = NULL;
    
    PBRShader* guitarMetalShader = NULL;
    PBRShader* guitarPlasticShader = NULL;
    PBRShader* guitarWood1Shader = NULL;
    PBRShader* guitarWood2Shader = NULL;
    
    PBRShader* SWAT1Shader = NULL;
    PBRShader* SWAT2Shader = NULL;
    PBRShader* SWAT3Shader = NULL;
    
    PBRShader* girlShader = NULL;
    PBRShader* hairShader = NULL;
    
    PBRShader* zombie1Shader = NULL;
    PBRShader* zombie2Shader = NULL;
    
    PBRShader* sisterShader = NULL;
    
    
    
    // Nodes to load:
    UINode* baseNode = NULL;
    UINode* logoUnitNode = NULL;
    SpriteNode* logoNode = NULL;
    TextNode* logoLabelNode = NULL;
    TextNode* loadingLabelNode = NULL;
    TextNode* instructionsLabelNode = NULL;
    SpriteNode* backgroundNode = NULL;
    SpriteNode* curtainNode = NULL;
    
    Node* sceneNode = NULL;
    LightNode* moonlightNode = NULL;
    
    Node* bonfireNode = NULL;
    LightNode* bonfireLightNode = NULL;
    ParticleNode* fireNode = NULL;
    ParticleNode* smokeNode = NULL;
    ParticleNode* sparkNode = NULL;
    
    Node* cowboyNode = NULL;
    
    Node* guitarNode = NULL;
    
    Node* SWATNode = NULL;
    
    Node* girlNode = NULL;
    
    Node* zombieNode = NULL;
    
    Node* sisterNode = NULL;
    
    
    
    // The skybox to load:
    Skybox* skybox = NULL;
    
    
    
    // Main loop:
    while(engine->isRunning()) {
        if(engine->shouldUpdate()) {
            
            
            
            if(applicationState == 1) {
                applicationState = 0;
                
                
                
                // Load the font file:
                FontLibrary* fontLibrary = new FontLibrary();
                title = fontLibrary->loadFontFile("/Resources/Demo/Cinzel Font/Cinzel.ttf", 50);
                delete(fontLibrary);
                
                
                
                // Create the base node that contains all the UI nodes:
                baseNode = new UINode();
                baseNode->screenPosition = vec2(0.5f);
                baseNode->renderingOrder = 1000.0f;
                engine->addNode(baseNode);
                
                
                
                // Create the logo unit node that contains the logo image and a label:
                logoUnitNode = new UINode();
                baseNode->addChildNode(logoUnitNode);
                
                
                
                // Load the logo texture and create the logo node:
                logoMap = new Texture("/Resources/Icon.png");
                logoNode = new SpriteNode(vec2(0.09f));
                logoNode->position = vec2(-0.22f, -0.016f);
                logoNode->texture = logoMap;
                logoUnitNode->addChildNode(logoNode);
                
                
                
                // Create the logo label node:
                logoLabelNode = new TextNode(title, 0.06f, 1.0f, 0.0f);
                logoLabelNode->position = vec2(-0.165f, 0.0f);
                logoLabelNode->color = vec4(1.0f);
                logoLabelNode->text = "KGL Engine Demo";
                logoLabelNode->setLeftHorizontalAlignment();
                logoLabelNode->setTopVerticalAlignment();
                logoUnitNode->addChildNode(logoLabelNode);
                
                
                
                // Create the loading label node:
                loadingLabelNode = new TextNode(title, 0.04f, 1.0f, 0.0f);
                loadingLabelNode->position = vec2(0.0f, 0.03f);
                loadingLabelNode->alpha = 0.0f;
                loadingLabelNode->color = vec4(1.0f);
                loadingLabelNode->text = "Loading 0%";
                loadingLabelNode->setCenterHorizontalAlignment();
                loadingLabelNode->setTopVerticalAlignment();
                baseNode->addChildNode(loadingLabelNode);
                
                
                // Create the instruction label node:
                instructionsLabelNode = new TextNode(title, 0.03f, 1.0f, 0.005f);
                instructionsLabelNode->renderingOrder = 1000.0f;
                instructionsLabelNode->screenPosition = vec2(1.0f, 1.0f);
                instructionsLabelNode->position = vec2(-0.03f, -0.03f);
                instructionsLabelNode->color = vec4(1.0f);
                instructionsLabelNode->text = "Press ESC to toggle the cursor.";
                instructionsLabelNode->text += "\nPress W / S / A / D to move around.";
                instructionsLabelNode->text += "\nMove the mouse to rotate the camera.";
                instructionsLabelNode->setRightHorizontalAlignment();
                instructionsLabelNode->setBottomVerticalAlignment();
                instructionsLabelNode->isDisabled = true;
                engine->addNode(instructionsLabelNode);
                
                
                
                // Create the UI background node:
                backgroundNode = new SpriteNode(vec2(10.0f));
                backgroundNode->renderingOrder = -10.0f;
                backgroundNode->color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
                baseNode->addChildNode(backgroundNode);
                
                
                
                // Create the curtain node that covers the screen:
                curtainNode = new SpriteNode(vec2(10.0f));
                curtainNode->renderingOrder = 10.0f;
                curtainNode->color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
                baseNode->addChildNode(curtainNode);
                
                
                
                // Animate the curtain to show the logo unit node:
                Animation* curtainAnimation = new Animation("curtainAnimation", 3.0f);
                curtainAnimation->setFloatAnimation(&curtainNode->alpha, 0.0f);
                curtainAnimation->setEaseOutTimingMode();
                engine->playAnimation(curtainAnimation);
                
                
                
                // Move up te logo unit node after 2 seconds to make room for the loading label:
                Animation* delayAnimation1 = new Animation("delayAnimation1", 2.0f);
                delayAnimation1->setCompletionHandler([&] {
                    
                    Animation* logoUnitAnimation = new Animation("logoUnitAnimation", 1.0f);
                    logoUnitAnimation->setVec2Animation(&logoUnitNode->position, vec2(0.0f, -0.03f));
                    logoUnitAnimation->setEaseInEaseOutTimingMode();
                    engine->playAnimation(logoUnitAnimation);
                    
                });
                engine->playAnimation(delayAnimation1);
                
                
                
                // Show the loading label node after 2.5 seconds:
                Animation* delayAnimation2 = new Animation("delayAnimation2", 2.5f);
                delayAnimation2->setCompletionHandler([&] {
                    
                    Animation* loadingLabelAnimation = new Animation("loadingLabelAnimation", 1.0f);
                    loadingLabelAnimation->setFloatAnimation(&loadingLabelNode->alpha, 1.0f);
                    loadingLabelAnimation->setEaseInEaseOutTimingMode();
                    loadingLabelAnimation->setCompletionHandler([&] {
                        applicationState = 2;
                    });
                    engine->playAnimation(loadingLabelAnimation);
                    
                });
                engine->playAnimation(delayAnimation2);
                
                
                
            }else if(applicationState == 2) {
                
                if(loadingState == 1) {
                    string skyboxPath = "/Resources/Demo/Skybox/";
                    skybox = new Skybox(skyboxPath + "Front.png", skyboxPath + "Back.png",
                                        skyboxPath + "Up.png", skyboxPath + "Down.png",
                                        skyboxPath + "Right.png", skyboxPath + "Left.png",
                                        0.0f);
                    engine->skybox = skybox;
                    
                }else if(loadingState == 2) {
                    sceneReflectionMap = new Texture("/Resources/Demo/Scene/Textures/Scene R.png", 0.0f, false);
                
                }else if(loadingState == 3) {
                    sceneMultiplyMap = new Texture("/Resources/Demo/Scene/Textures/Dirt M.png", 0.0f, false);
                    
                }else if(loadingState == 4) {
                    groundDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Ground D.png", 2.0f, true);
                    
                }else if(loadingState == 5) {
                    groundNormalMap = new Texture("/Resources/Demo/Scene/Textures/Ground N.png", 1.0f, true);
                    
                }else if(loadingState == 6) {
                    groundHeightMap = new Texture("/Resources/Demo/Scene/Textures/Ground H.png", 1.0f, true);
                    
                }else if(loadingState == 7) {
                    groundMetallicMap = new Texture("/Resources/Demo/Scene/Textures/Ground M.png", 0.0f, true);
                    
                }else if(loadingState == 8) {
                    groundAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Ground AO.png", 0.0f, false);
                    
                }else if(loadingState == 9) {
                    barkDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bark D.png", 1.0f, true);
                    
                }else if(loadingState == 10) {
                    barkNormalMap = new Texture("/Resources/Demo/Scene/Textures/Bark N.png", 0.0f, true);
                    
                }else if(loadingState == 11) {
                    barkAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Bark AO.png", 0.0f, false);
                    
                }else if(loadingState == 12) {
                    rockDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Rock D.png", 1.0f, true);
                    
                }else if(loadingState == 13) {
                    rockNormalMap = new Texture("/Resources/Demo/Scene/Textures/Rock N.png", 0.0f, true);
                    
                }else if(loadingState == 14) {
                    rockAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Rock AO.png", 0.0f, false);
                    
                }else if(loadingState == 15) {
                    campDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Camp D.png", 1.0f, true);
                    
                }else if(loadingState == 16) {
                    campNormalMap = new Texture("/Resources/Demo/Scene/Textures/Camp N.png", 0.0f, false);
                    
                }else if(loadingState == 17) {
                    treeDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Tree D.png", 1.0f, true);
                    
                }else if(loadingState == 18) {
                    treeNormalMap = new Texture("/Resources/Demo/Scene/Textures/Tree N.png", 0.0f, true);
                    
                }else if(loadingState == 19) {
                    treeAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Tree AO.png", 0.0f, false);
                    
                }else if(loadingState == 20) {
                    detailDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Detail D.png", 1.0f, true);
                    
                }else if(loadingState == 21) {
                    detailNormalMap = new Texture("/Resources/Demo/Scene/Textures/Detail N.png", 0.0f, true);
                    
                }else if(loadingState == 22) {
                    detailMetallicMap = new Texture("/Resources/Demo/Scene/Textures/Detail M.png", 0.0f, true);
                    
                }else if(loadingState == 23) {
                    detailAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Detail AO.png", 0.0f, false);
                    
                }else if(loadingState == 24) {
                    propDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Prop D.png", 1.0f, true);
                    
                }else if(loadingState == 25) {
                    propNormalMap = new Texture("/Resources/Demo/Scene/Textures/Prop N.png", 0.0f, true);
                    
                }else if(loadingState == 26) {
                    propAmbientOcclusionMap = new Texture("/Resources/Demo/Scene/Textures/Prop AO.png", 0.0f, false);
                    
                }else if(loadingState == 27) {
                    leavesDiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Leaves D.png", 1.0f, true);
                    
                }else if(loadingState == 28) {
                    leavesNormalMap = new Texture("/Resources/Demo/Scene/Textures/Leaves N.png", 0.0f, false);
                    
                }else if(loadingState == 29) {
                    bush1DiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bush1 D.png", 1.0f, true);
                    
                }else if(loadingState == 30) {
                    bush1NormalMap = new Texture("/Resources/Demo/Scene/Textures/Bush1 N.png", 0.0f, false);
                    
                }else if(loadingState == 31) {
                    bush2DiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bush2 D.png", 1.0f, true);
                    
                }else if(loadingState == 32) {
                    bush2NormalMap = new Texture("/Resources/Demo/Scene/Textures/Bush2 N.png", 0.0f, false);
                    
                }else if(loadingState == 33) {
                    bush3DiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bush3 D.png", 1.0f, true);
                    
                }else if(loadingState == 34) {
                    bush3NormalMap = new Texture("/Resources/Demo/Scene/Textures/Bush3 N.png", 0.0f, false);
                    
                }else if(loadingState == 35) {
                    bush4DiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bush4 D.png", 1.0f, true);
                    
                }else if(loadingState == 36) {
                    bush4NormalMap = new Texture("/Resources/Demo/Scene/Textures/Bush4 N.png", 0.0f, false);
                    
                }else if(loadingState == 38) {
                    bush5DiffuseMap = new Texture("/Resources/Demo/Scene/Textures/Bush5 D.png", 1.0f, true);
                    
                }else if(loadingState == 39) {
                    bush5NormalMap = new Texture("/Resources/Demo/Scene/Textures/Bush5 N.png", 0.0f, false);
                    
                }else if(loadingState == 40) {
                    supportShader = new PBRShader(1.0f, 0.0f);
                    supportShader->diffuseColor = vec4(0.5f, 0.5f, 0.55f, 1.0f);
                    supportShader->setReflectionMap(sceneReflectionMap);
                    supportShader->reflectionIntensity = 0.1f;
                    
                }else if(loadingState == 41) {
                    groundShader = new PBRShader(0.2f, 0.8f);
                    groundShader->setDiffuseMap(groundDiffuseMap);
                    groundShader->setNormalMap(groundNormalMap);
                    groundShader->setHeightMap(groundHeightMap);
                    groundShader->heightIntensity = 0.1f;
                    groundShader->heightLayerRange = vec2(2.0f, 8.0f);
                    groundShader->setMetallicMap(groundMetallicMap);
                    groundShader->setRoughnessMap(groundMetallicMap);
                    groundShader->invertRoughness = true;
                    groundShader->setAmbientOcclusionMap(groundAmbientOcclusionMap);
                    
                }else if(loadingState == 42) {
                    barkShader = new PBRShader(0.0f, 0.9f);
                    barkShader->setDiffuseMap(barkDiffuseMap);
                    barkShader->setNormalMap(barkNormalMap);
                    barkShader->setAmbientOcclusionMap(barkAmbientOcclusionMap);
                    
                }else if(loadingState == 43) {
                    rockShader = new PBRShader(0.6f, 0.6f);
                    rockShader->setDiffuseMap(rockDiffuseMap);
                    rockShader->setNormalMap(rockNormalMap);
                    rockShader->setAmbientOcclusionMap(rockAmbientOcclusionMap);
                    
                }else if(loadingState == 44) {
                    campShader = new PBRShader(0.6f, 0.6f);
                    campShader->setDiffuseMap(campDiffuseMap);
                    campShader->setNormalMap(campNormalMap);
                    
                }else if(loadingState == 45) {
                    treeShader = new PBRShader(0.0f, 0.9f);
                    treeShader->setDiffuseMap(treeDiffuseMap);
                    treeShader->setNormalMap(treeNormalMap);
                    treeShader->setAmbientOcclusionMap(treeAmbientOcclusionMap);
                    
                }else if(loadingState == 46) {
                    detailShader = new PBRShader(0.0f, 0.8f);
                    detailShader->setDiffuseMap(detailDiffuseMap);
                    detailShader->setNormalMap(detailNormalMap);
                    detailShader->setMetallicMap(detailMetallicMap);
                    detailShader->setRoughnessMap(detailMetallicMap);
                    detailShader->invertRoughness = true;
                    detailShader->setAmbientOcclusionMap(detailAmbientOcclusionMap);
                    detailShader->setReflectionMap(sceneReflectionMap);
                    
                }else if(loadingState == 47) {
                    propShader = new PBRShader(0.0f, 0.8f);
                    propShader->setDiffuseMap(propDiffuseMap);
                    propShader->setNormalMap(propNormalMap);
                    propShader->setAmbientOcclusionMap(propAmbientOcclusionMap);
                    
                }else if(loadingState == 48) {
                    leavesShader = new PBRShader("/Resources/Demo/Scene/Shaders/Leaves", 0.2f, 0.6f);
                    leavesShader->setDiffuseMap(leavesDiffuseMap);
                    leavesShader->alphaCutThreshold = 0.5f;
                    leavesShader->setNormalMap(leavesNormalMap);
                    
                }else if(loadingState == 49) {
                    bush1Shader = new PBRShader("/Resources/Demo/Scene/Shaders/Plants", 0.2f, 0.6f);
                    bush1Shader->setDiffuseMap(bush1DiffuseMap);
                    bush1Shader->alphaCutThreshold = 0.5f;
                    bush1Shader->setNormalMap(bush1NormalMap);
                    
                }else if(loadingState == 50) {
                    bush2Shader = new PBRShader("/Resources/Demo/Scene/Shaders/Plants", 0.2f, 0.6f);
                    bush2Shader->setDiffuseMap(bush2DiffuseMap);
                    bush2Shader->alphaCutThreshold = 0.5f;
                    bush2Shader->setNormalMap(bush2NormalMap);
                    
                }else if(loadingState == 51) {
                    bush3Shader = new PBRShader("/Resources/Demo/Scene/Shaders/Plants", 0.2f, 0.6f);
                    bush3Shader->setDiffuseMap(bush3DiffuseMap);
                    bush3Shader->alphaCutThreshold = 0.5f;
                    bush3Shader->setNormalMap(bush3NormalMap);
                    
                }else if(loadingState == 52) {
                    bush4Shader = new PBRShader("/Resources/Demo/Scene/Shaders/Plants", 0.2f, 0.6f);
                    bush4Shader->setDiffuseMap(bush4DiffuseMap);
                    bush4Shader->alphaCutThreshold = 0.5f;
                    bush4Shader->setNormalMap(bush4NormalMap);
                    
                }else if(loadingState == 53) {
                    bush5Shader = new PBRShader("/Resources/Demo/Scene/Shaders/Plants", 0.2f, 0.6f);
                    bush5Shader->setDiffuseMap(bush5DiffuseMap);
                    bush5Shader->alphaCutThreshold = 0.5f;
                    bush5Shader->setNormalMap(bush5NormalMap);
                    
                }else if(loadingState == 54) {
                    sceneNode = new Node();
                    sceneNode->loadModelFile("/Resources/Demo/Scene/Model.dae");
                    sceneNode->geometries[0]->setShader(supportShader);
                    sceneNode->geometries[1]->setShader(groundShader);
                    sceneNode->geometries[2]->setShader(barkShader);
                    sceneNode->geometries[3]->setShader(rockShader);
                    sceneNode->geometries[4]->setShader(campShader);
                    sceneNode->geometries[5]->setShader(treeShader);
                    sceneNode->geometries[6]->setShader(detailShader);
                    sceneNode->geometries[7]->setShader(propShader);
                    sceneNode->geometries[8]->setShader(detailShader);
                    sceneNode->geometries[9]->setShader(leavesShader);
                    sceneNode->geometries[10]->setShader(barkShader);
                    sceneNode->geometries[11]->setShader(treeShader);
                    sceneNode->geometries[12]->setShader(treeShader);
                    sceneNode->geometries[13]->setShader(bush1Shader);
                    sceneNode->geometries[13]->renderingBitMask = 4;
                    sceneNode->geometries[13]->setDoubleSided();
                    sceneNode->geometries[14]->setShader(bush2Shader);
                    sceneNode->geometries[14]->renderingBitMask = 4;
                    sceneNode->geometries[14]->setDoubleSided();
                    sceneNode->geometries[15]->setShader(bush3Shader);
                    sceneNode->geometries[15]->renderingBitMask = 4;
                    sceneNode->geometries[15]->setDoubleSided();
                    sceneNode->geometries[16]->setShader(bush4Shader);
                    sceneNode->geometries[16]->renderingBitMask = 4;
                    sceneNode->geometries[16]->setDoubleSided();
                    sceneNode->geometries[17]->setShader(bush5Shader);
                    sceneNode->geometries[17]->renderingBitMask = 4;
                    sceneNode->geometries[17]->setDoubleSided();
                    engine->addNode(sceneNode);
                    
                }else if(loadingState == 55) {
                    moonlightNode = new LightNode(vec3(5.0f, 5.0f, 10.0f));
                    moonlightNode->setDirectionalLight();
                    moonlightNode->activateDirectionalLightShadow(2048, 30.0f, 0.1f, 40.0f, -10.0f, 0.002f, 1);
                    moonlightNode->position = vec3(10.0f, 8.0f, 0.0f);
                    moonlightNode->eulerAngles = vec3(0.0f, 180.0f, -20.0f);
                    moonlightNode->shadowBitMask = 2;
                    engine->addNode(moonlightNode);
                    
                }else if(loadingState == 56) {
                    fireMap = new Texture("/Resources/Demo/Particles/Fire.png");
                    smokeMap = new Texture("/Resources/Demo/Particles/Smoke.png");
                    sparkMap = new Texture("/Resources/Demo/Particles/Spark.png");
                    
                }else if(loadingState == 57) {
                    bonfireNode = new Node();
                    bonfireNode->position = vec3(1.88f, 0.6f, 3.69f);
                    bonfireNode->eulerAngles = vec3(0.0f, 0.0f, 90.0f);
                    
                    bonfireLightNode = new LightNode(vec3(60.0f, 10.0f, 0.0f));
                    bonfireLightNode->setPointLight(2.0f, 10.0f);
                    bonfireLightNode->penetrationRange = 2.0f;
                    bonfireLightNode->position = vec3(2.4f, 0.0f, 0.0f);
                    bonfireNode->addChildNode(bonfireLightNode);
                    
                    AudioBuffer* bonfireAudioBuffer = new AudioBuffer("/Resources/Demo/Audio Files/Bonfire.wav");
                    bonfireNode->loadAudioBuffer("bonfire1", bonfireAudioBuffer, 4.0f, 2.0f);
                    bonfireNode->loadAudioBuffer("bonfire2", bonfireAudioBuffer, 4.0f, 2.0f);
                    bonfireNode->changeAudioVolume("bonfire1", 0.0f, 0.0f);
                    bonfireNode->changeAudioVolume("bonfire2", 0.0f, 0.0f);
                    
                    engine->addNode(bonfireNode);
                    
                }else if(loadingState == 58) {
                    fireNode = new ParticleNode(300, 0.5f, 0.2f);
                    fireNode->renderingOrder = 12.0f;
                    fireNode->isAdditive = true;
                    fireNode->setEmissionSphere(0.4f, 0.8f);
                    fireNode->spreadingAngle = 10.0f;
                    fireNode->initialSpeed = 1.0f;
                    fireNode->initialSpeedVariation = 1.0f;
                    fireNode->initialRotationVariation = 360.0f;
                    fireNode->initialRotationVariation = 180.0f;
                    fireNode->acceleration = vec3(0.0f, 2.0f, 0.0f);
                    fireNode->rotatingSpeed = 180.0f;
                    fireNode->rotatingSpeedVariation = 90.0f;
                    fireNode->randomizeRotatingDirection = true;
                    fireNode->initialScale = 0.6f;
                    fireNode->initialScaleVariation = 0.5f;
                    fireNode->scalingSpeed = -0.6f;
                    fireNode->scalingSpeedVariation = 0.2f;
                    fireNode->texture = fireMap;
                    fireNode->setColorAnimation(vec4(1.0f, 0.4f, 0.0f, 0.0f), 0.0f);
                    fireNode->setColorAnimation(vec4(1.0f, 0.3f, 0.0f, 0.4f), 0.2f);
                    fireNode->setColorAnimation(vec4(1.0f, 0.2f, 0.0f, 0.1f), 0.6f);
                    fireNode->setColorAnimation(vec4(1.0f, 0.1f, 0.0f, 0.0f), 1.0f);
                    bonfireNode->addChildNode(fireNode);
                    
                }else if(loadingState == 59) {
                    smokeNode = new ParticleNode(15, 5.0f, 0.5f);
                    smokeNode->renderingOrder = 10.0f;
                    smokeNode->setEmissionSphere(0.4f, 0.8f);
                    smokeNode->spreadingAngle = 180.0f;
                    smokeNode->initialSpeed = 0.2f;
                    smokeNode->initialSpeedVariation = 0.1f;
                    smokeNode->acceleration = vec3(-0.2f, 1.0f, -0.2f);
                    smokeNode->accelerationVariation = vec3(0.1f, 0.5f, 0.1f);
                    smokeNode->initialRotationVariation = 360.0f;
                    smokeNode->rotatingSpeedVariation = 180.0f;
                    smokeNode->initialScale = 0.6f;
                    smokeNode->initialScaleVariation = 0.4f;
                    smokeNode->scalingSpeed = 0.6f;
                    smokeNode->scalingSpeedVariation = 0.4f;
                    smokeNode->texture = smokeMap;
                    smokeNode->setColorAnimation(vec4(1.0f, 1.0f, 1.0f, 0.0f), 0.0f);
                    smokeNode->setColorAnimation(vec4(1.0f, 1.0f, 1.0f, 0.2f), 0.2f);
                    smokeNode->setColorAnimation(vec4(0.0f, 0.0f, 0.0f, 0.2f), 0.7f);
                    smokeNode->setColorAnimation(vec4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f);
                    bonfireNode->addChildNode(smokeNode);
                    
                }else if(loadingState == 60) {
                    sparkNode = new ParticleNode(15, 1.0f, 0.5f);
                    sparkNode->renderingOrder = 15.0f;
                    sparkNode->isAdditive = true;
                    sparkNode->setEmissionSphere(0.5f, 1.0f);
                    sparkNode->spreadingAngle = 90.0f;
                    sparkNode->initialSpeed = 1.0f;
                    sparkNode->initialSpeedVariation = 1.0f;
                    sparkNode->initialRotationVariation = 180.0f;
                    sparkNode->acceleration = vec3(0.0f, 2.0f, 0.0f);
                    sparkNode->accelerationVariation = vec3(0.5f, 2.0f, 0.5f);
                    sparkNode->initialRotationVariation = 360.0f;
                    sparkNode->rotatingSpeed = 270.0f;
                    sparkNode->rotatingSpeedVariation = 180.0f;
                    sparkNode->randomizeRotatingDirection = true;
                    sparkNode->initialScale = 0.8f;
                    sparkNode->initialScaleVariation = 0.4f;
                    sparkNode->scalingSpeed = -0.1f;
                    sparkNode->scalingSpeedVariation = 0.1f;
                    sparkNode->texture = sparkMap;
                    sparkNode->setColorAnimation(vec4(1.0f, 0.6f, 0.2f, 0.0f), 0.0f);
                    sparkNode->setColorAnimation(vec4(1.0f, 0.6f, 0.2f, 1.0f), 0.1f);
                    sparkNode->setColorAnimation(vec4(1.0f, 0.6f, 0.2f, 1.0f), 0.8f);
                    sparkNode->setColorAnimation(vec4(1.0f, 0.6f, 0.2f, 0.0f), 1.0f);
                    bonfireNode->addChildNode(sparkNode);
                    
                }else if(loadingState == 61) {
                    cowboyDiffuseMap = new Texture("/Resources/Demo/Cowboy/Texture/Cowboy D.png", 1.0f, true);
                    
                }else if(loadingState == 62) {
                    cowboyHandsShader = new PBRShader("/Resources/Demo/Cowboy/Shader/Hands", 0.1f, 0.9f);
                    cowboyHandsShader->setDiffuseMap(cowboyDiffuseMap);
                    cowboyHandsShader->setFloat("shadowBias", 0.02f);
                    
                }else if(loadingState == 63) {
                    cowboyBodyShader = new PBRShader(0.1f, 0.9f);
                    cowboyBodyShader->setDiffuseMap(cowboyDiffuseMap);
                    cowboyBodyShader->setMultiplyMap(sceneMultiplyMap);
                    cowboyBodyShader->multiplyIntensity = 0.2f;
                    
                }else if(loadingState == 64) {
                    cowboyNode = new Node();
                    cowboyNode->loadModelFile("/Resources/Demo/Cowboy/Model.dae");
                    cowboyNode->geometries[0]->setShader(cowboyHandsShader);
                    cowboyNode->geometries[1]->setShader(cowboyBodyShader);
                    cowboyNode->geometries[1]->setDoubleSided();
                    cowboyNode->position = vec3(-2.087f, 1.38f, 0.282f);
                    cowboyNode->eulerAngles = vec3(1.418f, 63.423f, 0.69f);
                    cowboyNode->scale = vec3(0.75f);
                    engine->addNode(cowboyNode);
                    
                }else if(loadingState == 65) {
                    cowboyNode->loadAnimator("playing", "/Resources/Demo/Cowboy/Animation.dae");
                    
                }else if(loadingState == 67) {
                    guitarWood1DiffuseMap = new Texture("/Resources/Demo/Guitar/Textures/Wood1 D.png", 0.0f, false);
                    guitarWood2DiffuseMap = new Texture("/Resources/Demo/Guitar/Textures/Wood2 D.png", 0.0f, false);
                    
                }else if(loadingState == 68) {
                    guitarMetalShader = new PBRShader(1.0f, 0.0f);
                    guitarMetalShader->diffuseColor = vec4(1.0f);
                    
                    guitarPlasticShader = new PBRShader(0.0f, 0.5f);
                    
                    guitarWood1Shader = new PBRShader(0.0f, 0.5f);
                    guitarWood1Shader->setDiffuseMap(guitarWood1DiffuseMap);
                    
                    guitarWood2Shader = new PBRShader(0.0f, 0.5f);
                    guitarWood2Shader->setDiffuseMap(guitarWood2DiffuseMap);
                    
                }else if(loadingState == 69) {
                    guitarNode = new Node();
                    guitarNode->loadModelFile("/Resources/Demo/Guitar/Model.dae");
                    guitarNode->geometries[0]->setShader(guitarMetalShader);
                    guitarNode->geometries[1]->setShader(guitarMetalShader);
                    guitarNode->geometries[2]->setShader(guitarPlasticShader);
                    guitarNode->geometries[3]->setShader(guitarWood2Shader);
                    guitarNode->geometries[4]->setShader(guitarWood1Shader);
                    guitarNode->position = vec3(0.0f);
                    guitarNode->eulerAngles = vec3(0.0f);
                    guitarNode->scale = vec3(0.2f);
                    
                    Node* guitarPositionNode = cowboyNode->generateBoneNode("Guitar");
                    guitarPositionNode->addChildNode(guitarNode);
                    
                }else if(loadingState == 70) {
                    AudioBuffer* guitarAudioBuffer = new AudioBuffer("/Resources/Demo/Audio Files/Guitar.wav");
                    guitarNode->loadAudioBuffer("guitar1", guitarAudioBuffer, 6.0f, 1.0f);
                    guitarNode->loadAudioBuffer("guitar2", guitarAudioBuffer, 6.0f, 1.0f);
                    guitarNode->changeAudioVolume("guitar1", 0.0f, 0.0f);
                    guitarNode->changeAudioVolume("guitar2", 0.0f, 0.0f);
                    
                }else if(loadingState == 71) {
                    SWAT1DiffuseMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT1 D.png", 1.0f, true);
                    
                }else if(loadingState == 72) {
                    SWAT1NormalMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT1 N.png", 0.0f, false);
                    
                }else if(loadingState == 73) {
                    SWAT2DiffuseMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT2 D.png", 1.0f, true);
                    
                }else if(loadingState == 74) {
                    SWAT2NormalMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT2 N.png", 0.0f, false);
                    
                }else if(loadingState == 75) {
                    SWAT3DiffuseMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT3 D.png", 1.0f, true);
                    
                }else if(loadingState == 76) {
                    SWAT3NormalMap = new Texture("/Resources/Demo/SWAT/Textures/SWAT3 N.png", 0.0f, false);
                    
                }else if(loadingState == 77) {
                    SWAT1Shader = new PBRShader("/Resources/Demo/SWAT/Shader/SWAT", 0.2f, 0.9f);
                    SWAT1Shader->setDiffuseMap(SWAT1DiffuseMap);
                    SWAT1Shader->setNormalMap(SWAT1NormalMap);
                    SWAT1Shader->setReflectionMap(sceneReflectionMap);
                    SWAT1Shader->setFloat("shadowBias", 0.02f);
                    
                }else if(loadingState == 78) {
                    SWAT2Shader = new PBRShader("/Resources/Demo/SWAT/Shader/SWAT", 0.4f, 0.8f);
                    SWAT2Shader->setDiffuseMap(SWAT2DiffuseMap);
                    SWAT2Shader->setNormalMap(SWAT2NormalMap);
                    SWAT2Shader->setReflectionMap(sceneReflectionMap);
                    SWAT2Shader->setFloat("shadowBias", 0.02f);
                    
                }else if(loadingState == 79) {
                    SWAT3Shader = new PBRShader("/Resources/Demo/SWAT/Shader/SWAT", 0.8f, 0.2f);
                    SWAT3Shader->setDiffuseMap(SWAT3DiffuseMap);
                    SWAT3Shader->setNormalMap(SWAT3NormalMap);
                    SWAT3Shader->setReflectionMap(sceneReflectionMap);
                    SWAT3Shader->setFloat("shadowBias", 0.02f);
                    
                }else if(loadingState == 80) {
                    SWATNode = new Node();
                    SWATNode->loadModelFile("/Resources/Demo/SWAT/Model.dae");
                    SWATNode->geometries[0]->setShader(SWAT3Shader);
                    SWATNode->geometries[1]->setShader(SWAT1Shader);
                    SWATNode->geometries[2]->setShader(SWAT2Shader);
                    SWATNode->position = vec3(3.809f, 1.409f, 0.411f);
                    SWATNode->eulerAngles = vec3(-5.825f, -13.369f, -6.525f);
                    SWATNode->scale = vec3(2.8f);
                    engine->addNode(SWATNode);
                    
                }else if(loadingState == 81) {
                    Animator* sittingAnimator = SWATNode->loadAnimator("sitting", "/Resources/Demo/SWAT/Animation.dae");
                    sittingAnimator->play(0.0f);
                    
                }else if(loadingState == 82) {
                    girlDiffuseMap = new Texture("/Resources/Demo/Girl/Textures/Girl D.png", 1.0f, true);
                    
                }else if(loadingState == 83) {
                    girlNormalMap = new Texture("/Resources/Demo/Girl/Textures/Girl N.png", 0.0f, false);
                    
                }else if(loadingState == 84) {
                    girlMetallicMap = new Texture("/Resources/Demo/Girl/Textures/Girl M.png", 0.0f, false);
                    
                }else if(loadingState == 85) {
                    hairDiffuseMap = new Texture("/Resources/Demo/Girl/Textures/Hair D.png", 1.0f, true);
                    
                }else if(loadingState == 86) {
                    girlShader = new PBRShader(0.0f, 0.5f);
                    girlShader->setDiffuseMap(girlDiffuseMap);
                    girlShader->setNormalMap(girlNormalMap);
                    girlShader->setRoughnessMap(girlMetallicMap);
                    girlShader->invertRoughness = true;
                    girlShader->roughnessIntensity = 2.0f;
                    girlShader->setMultiplyMap(sceneMultiplyMap);
                    girlShader->multiplyIntensity = 0.3f;
                    
                }else if(loadingState == 87) {
                    hairShader = new PBRShader(0.0f, 1.0f);
                    hairShader->setDiffuseMap(hairDiffuseMap);
                    hairShader->alphaCutThreshold = 0.5f;
                    
                }else if(loadingState == 88) {
                    girlNode = new Node();
                    girlNode->loadModelFile("/Resources/Demo/Girl/Model.dae");
                    girlNode->geometries[0]->setShader(girlShader);
                    girlNode->geometries[1]->setShader(girlShader);
                    girlNode->geometries[2]->setShader(girlShader);
                    girlNode->geometries[3]->isHidden = true;
                    girlNode->geometries[4]->setShader(girlShader);
                    girlNode->geometries[5]->setShader(girlShader);
                    girlNode->geometries[6]->setShader(hairShader);
                    girlNode->geometries[6]->setDoubleSided();
                    girlNode->position = vec3(-1.311f, 0.754f, 4.095f);
                    girlNode->eulerAngles = vec3(-1.825f, 35.603f, 11.876f);
                    girlNode->scale = vec3(2.8f);
                    engine->addNode(girlNode);
                    
                }else if(loadingState == 89) {
                    Animator* beingSadAnimator = girlNode->loadAnimator("being sad", "/Resources/Demo/Girl/Animation.dae");
                    beingSadAnimator->play(0.0f);
                    
                }else if(loadingState == 90) {
                    zombie1DiffuseMap = new Texture("/Resources/Demo/Zombie/Textures/Zombie1 D.png", 1.0f, true);
                    
                }else if(loadingState == 91) {
                    zombie1NormalMap = new Texture("/Resources/Demo/Zombie/Textures/Zombie1 N.png", 0.0f, false);
                    
                }else if(loadingState == 92) {
                    zombie2DiffuseMap = new Texture("/Resources/Demo/Zombie/Textures/Zombie2 D.png", 1.0f, true);
                    
                }else if(loadingState == 93) {
                    zombie2NormalMap = new Texture("/Resources/Demo/Zombie/Textures/Zombie2 N.png", 0.0f, false);
                    
                }else if(loadingState == 94) {
                    zombie1Shader = new PBRShader(0.1f, 0.9f);
                    zombie1Shader->setDiffuseMap(zombie1DiffuseMap);
                    zombie1Shader->setNormalMap(zombie1NormalMap);
                    
                }else if(loadingState == 95) {
                    zombie2Shader = new PBRShader(0.2f, 0.8f);
                    zombie2Shader->setDiffuseMap(zombie2DiffuseMap);
                    zombie2Shader->setNormalMap(zombie2NormalMap);
                    
                }else if(loadingState == 96) {
                    zombieNode = new Node();
                    zombieNode->loadModelFile("/Resources/Demo/Zombie/Model.dae");
                    zombieNode->geometries[0]->setShader(zombie1Shader);
                    zombieNode->geometries[1]->setShader(zombie2Shader);
                    zombieNode->position = vec3(-0.852f, 1.163f, -8.557f);
                    zombieNode->eulerAngles = vec3(0.0f, 4.181f, 0.0f);
                    zombieNode->scale = vec3(3.0f);
                    engine->addNode(zombieNode);
                    
                }else if(loadingState == 97) {
                    Animator* dancingAnimator = zombieNode->loadAnimator("dancing", "/Resources/Demo/Zombie/Animation.dae");
                    dancingAnimator->play(0.0f);
                    
                }else if(loadingState == 98) {
                    sister1DiffuseMap = new Texture("/Resources/Demo/Sister/Textures/Sister D.png", 1.0f, true);
                    
                }else if(loadingState == 99) {
                    sister1NormalMap = new Texture("/Resources/Demo/Sister/Textures/Sister N.png", 0.0f, false);
                    
                }else if(loadingState == 100) {
                    sisterShader = new PBRShader(0.1f, 0.9f);
                    sisterShader->setDiffuseMap(sister1DiffuseMap);
                    sisterShader->setNormalMap(sister1NormalMap);
                    
                }else if(loadingState == 101) {
                    sisterNode = new Node();
                    sisterNode->loadModelFile("/Resources/Demo/Sister/Model.dae");
                    sisterNode->geometries[0]->setShader(sisterShader);
                    sisterNode->geometries[0]->setDoubleSided();
                    
                }else if(loadingState == 102) {
                    sisterNode->position = vec3(-7.22f, 1.102f, 0.632f);
                    sisterNode->eulerAngles = vec3(0.0f, -79.821f, 0.0f);
                    sisterNode->scale = vec3(2.0f);
                    engine->addNode(sisterNode);
                    
                }else if(loadingState == 103) {
                    Animator* hidingAnimator = sisterNode->loadAnimator("hiding", "/Resources/Demo/Sister/Animation.dae");
                    hidingAnimator->play(0.0f);
                    
                }else if(loadingState == 104) {
                    AudioBuffer* ambientAudioBuffer = new AudioBuffer("/Resources/Demo/Audio Files/Ambient.wav");
                    cameraNode->loadAudioBuffer("ambient1", ambientAudioBuffer);
                    cameraNode->loadAudioBuffer("ambient2", ambientAudioBuffer);
                    cameraNode->changeAudioVolume("ambient1", 0.0f, 0.0f);
                    cameraNode->changeAudioVolume("ambient2", 0.0f, 0.0f);
                    
                }
                
                
                
                // Update the loading label:
                unsigned int progress = (unsigned int)((float)loadingState * 100.0f / 110.0f);
                loadingLabelNode->text = "Loading " + to_string(progress) + "%";
                
                
                
                // Update the loading state:
                loadingState += 1;
                if(loadingState > 110.0f) {
                    
                    applicationState = 3;
                    
                    // Fade the curtain:
                    Animation* curtainAnimation1 = new Animation("curtainAnimation1", 2.0f);
                    curtainAnimation1->setFloatAnimation(&curtainNode->alpha, 1.0f);
                    curtainAnimation1->setEaseInTimingMode();
                    curtainAnimation1->setCompletionHandler([&] {
                        
                        // Hide the UI nodes:
                        logoUnitNode->isDisabled = true;
                        loadingLabelNode->isDisabled = true;
                        backgroundNode->isDisabled = true;
                        
                        // Show the instructions:
                        instructionsLabelNode->isDisabled = false;
                        
                        // Turn up the volume of the sounds:
                        bonfireNode->changeAudioVolume("bonfire1", 1.0f, 3.0f);
                        bonfireNode->changeAudioVolume("bonfire2", 1.0f, 3.0f);
                        guitarNode->changeAudioVolume("guitar1", 0.5f, 3.0f);
                        guitarNode->changeAudioVolume("guitar2", 0.5f, 3.0f);
                        Animation* volumeAnimation = new Animation("volumeAnimation", 3.0f);
                        volumeAnimation->setFloatAnimation(&ambientAudioVolumeFactor, 1.0f);
                        volumeAnimation->setEaseInEaseOutTimingMode();
                        engine->playAnimation(volumeAnimation);
                        
                        // Slowly allow camera control:
                        Animation* allowCameraControlAnimation = new Animation("allowCameraControlAnimation", 5.0f);
                        allowCameraControlAnimation->setFloatAnimation(&cameraControlFactor, 1.0f);
                        allowCameraControlAnimation->setEaseInEaseOutTimingMode();
                        engine->playAnimation(allowCameraControlAnimation);
                        
                        // Remove the curtain:
                        Animation* curtainAnimation2 = new Animation("curtainAnimation2", 3.0f);
                        curtainAnimation2->setFloatAnimation(&curtainNode->alpha, 0.0f);
                        curtainAnimation2->setEaseOutTimingMode();
                        engine->playAnimation(curtainAnimation2);
                        
                    });
                    engine->playAnimation(curtainAnimation1);
                }
                
                
                
            }else if(applicationState == 3) {
                
                
                
                // Press ESC to toggle the cursor:
                if(engine->input->wasKeyReleased(KEY_ESCAPE)) {
                    if(cameraControl) {
                        cameraControl = false;
                        engine->unlockCursor();
                    }else{
                        cameraControl = true;
                        engine->lockCursor();
                    }
                }
                
                
                
                // Update the camera:
                if(cameraControl) {
                    
                    // Update target position:
                    if(engine->input->isPressingKey(KEY_W)) {
                        cameraTargetPosition += cameraNode->getFrontVectorInWorld() * 0.2f * cameraControlFactor;
                    }else if(engine->input->isPressingKey(KEY_S)) {
                        cameraTargetPosition += cameraNode->getBackVectorInWorld() * 0.2f * cameraControlFactor;
                    }
                    if(engine->input->isPressingKey(KEY_A)) {
                        cameraTargetPosition += cameraNode->getLeftVectorInWorld() * 0.2f * cameraControlFactor;
                    }else if(engine->input->isPressingKey(KEY_D)) {
                        cameraTargetPosition += cameraNode->getRightVectorInWorld() * 0.2f * cameraControlFactor;
                    }
                    if(engine->input->isPressingKey(KEY_E)) {
                        cameraTargetPosition += cameraNode->getUpVectorInWorld() * 0.2f * cameraControlFactor;
                    }else if(engine->input->isPressingKey(KEY_Q)) {
                        cameraTargetPosition += cameraNode->getDownVectorInWorld() * 0.2f * cameraControlFactor;
                    }
                    
                    // Update target euler angles:
                    vec2 mouseTranslation = engine->input->getMouseTranslation() * 0.1f;
                    cameraTargetEulerAngles.y -= mouseTranslation.x * cameraControlFactor;
                    cameraTargetEulerAngles.z -= mouseTranslation.y * cameraControlFactor;
                    cameraTargetEulerAngles.z = glm::max(-60.0f, glm::min(cameraTargetEulerAngles.z, 60.0f));
                    
                    // Smooth movement:
                    Animation* moveAnimation = new Animation("moveCamera", 0.2f);
                    moveAnimation->setVec3Animation(&cameraNode->position, cameraTargetPosition);
                    engine->playAnimation(moveAnimation);
                    
                    // Smooth rotation:
                    Animation* rotateAnimation = new Animation("rotateCamera", 0.1f);
                    rotateAnimation->setEulerAnglesAnimation(&cameraNode->eulerAngles, cameraTargetEulerAngles);
                    engine->playAnimation(rotateAnimation);
                }
                
                
                
                
                // Update the ambient sound:
                float ambientVolume = (30.0f - glm::length(cameraNode->position)) / 20.0f;
                ambientVolume = glm::clamp(ambientVolume, 0.0f, 1.0f) * 0.25f * ambientAudioVolumeFactor;
                cameraNode->changeAudioVolume("ambient1", ambientVolume, 0.0f);
                cameraNode->changeAudioVolume("ambient2", ambientVolume, 0.0f);
                if(ambientAudioState == 0) {
                    ambientAudioState = 1;
                    cameraNode->playAudio("ambient1");
                }else if(ambientAudioState == 1) {
                    if(cameraNode->getAudioTime("ambient1") > 30.5f) {
                        ambientAudioState = 2;
                        cameraNode->playAudio("ambient2");
                    }
                }else if(ambientAudioState == 2) {
                    if(cameraNode->getAudioTime("ambient2") > 30.5f) {
                        ambientAudioState = 1;
                        cameraNode->playAudio("ambient1");
                    }
                }
                
                
                
                // Update the bonfire sound:
                if(bonfireAudioState == 0) {
                    bonfireAudioState = 1;
                    bonfireNode->playAudio("bonfire1");
                }else if(bonfireAudioState == 1) {
                    if(bonfireNode->getAudioTime("bonfire1") > 10.5f) {
                        bonfireAudioState = 2;
                        bonfireNode->playAudio("bonfire2");
                    }
                }else if(bonfireAudioState == 2) {
                    if(bonfireNode->getAudioTime("bonfire2") > 10.5f) {
                        bonfireAudioState = 1;
                        bonfireNode->playAudio("bonfire1");
                    }
                }
                
                
                
                // Update the guitar sound:
                if(guitarAudioState == 0) {
                    cowboyNode->getAnimator("playing")->reset();
                    cowboyNode->getAnimator("playing")->play(0.0f);
                    guitarAudioState = 1;
                    guitarNode->playAudio("guitar1");
                }else{
                    float currentPlayingAnimatorTime = cowboyNode->getAnimator("playing")->getTime();
                    if(playingAnimatorTime > currentPlayingAnimatorTime) {
                        if(guitarAudioState == 1) {
                            guitarAudioState = 2;
                            guitarNode->playAudio("guitar2");
                        }else{
                            guitarAudioState = 1;
                            guitarNode->playAudio("guitar1");
                        }
                    }
                    playingAnimatorTime = currentPlayingAnimatorTime;
                }
                
                
                
                // Animate the bonfire's light:
                Animation* bonfireLightRangeAnimation = new Animation("bonfireLightRange", 0.08f);
                bonfireLightRangeAnimation->setFloatAnimation(&bonfireLightNode->range, glm::linearRand(10.0f, 12.0f));
                engine->playAnimation(bonfireLightRangeAnimation);
                
                vec3 colorFactor = vec3(60.0f, 10.0f, 0.0f) * glm::linearRand(0.6f, 1.0f);
                Animation* bonfireLightColorAnimation = new Animation("bonfireLightColor", 0.08f);
                bonfireLightColorAnimation->setVec3Animation(&bonfireLightNode->colorFactor, colorFactor);
                engine->playAnimation(bonfireLightColorAnimation);
                
                
                
                // Render the moonlight shadows:
                engine->renderDirectionalLightShadowMap(moonlightNode);
                
            }
            
            
            
            // Clear record:
            engine->input->wasKeyReleased(KEY_ESCAPE);
            
            
            
            // Render the scene:
            engine->render();
        }
    }
    
    
    
    // Delete the resources:
    delete(cameraNode);
    delete(title);
    delete(logoMap);
    delete(sceneReflectionMap);
    delete(sceneMultiplyMap);
    delete(groundDiffuseMap);
    delete(groundNormalMap);
    delete(groundHeightMap);
    delete(groundMetallicMap);
    delete(groundAmbientOcclusionMap);
    delete(barkDiffuseMap);
    delete(barkNormalMap);
    delete(barkAmbientOcclusionMap);
    delete(rockDiffuseMap);
    delete(rockNormalMap);
    delete(rockAmbientOcclusionMap);
    delete(campDiffuseMap);
    delete(campNormalMap);
    delete(treeDiffuseMap);
    delete(treeNormalMap);
    delete(treeAmbientOcclusionMap);
    delete(detailDiffuseMap);
    delete(detailNormalMap);
    delete(detailMetallicMap);
    delete(detailAmbientOcclusionMap);
    delete(propDiffuseMap);
    delete(propNormalMap);
    delete(propAmbientOcclusionMap);
    delete(leavesDiffuseMap);
    delete(leavesNormalMap);
    delete(bush1DiffuseMap);
    delete(bush1NormalMap);
    delete(bush2DiffuseMap);
    delete(bush2NormalMap);
    delete(bush3DiffuseMap);
    delete(bush3NormalMap);
    delete(bush4DiffuseMap);
    delete(bush4NormalMap);
    delete(bush5DiffuseMap);
    delete(bush5NormalMap);
    delete(fireMap);
    delete(smokeMap);
    delete(sparkMap);
    delete(cowboyDiffuseMap);
    delete(guitarWood1DiffuseMap);
    delete(guitarWood2DiffuseMap);
    delete(SWAT1DiffuseMap);
    delete(SWAT1NormalMap);
    delete(SWAT2DiffuseMap);
    delete(SWAT2NormalMap);
    delete(SWAT3DiffuseMap);
    delete(SWAT3NormalMap);
    delete(girlDiffuseMap);
    delete(girlNormalMap);
    delete(girlMetallicMap);
    delete(hairDiffuseMap);
    delete(zombie1DiffuseMap);
    delete(zombie1NormalMap);
    delete(zombie2DiffuseMap);
    delete(zombie2NormalMap);
    delete(sister1DiffuseMap);
    delete(sister1NormalMap);
    delete(supportShader);
    delete(groundShader);
    delete(barkShader);
    delete(rockShader);
    delete(campShader);
    delete(treeShader);
    delete(detailShader);
    delete(propShader);
    delete(leavesShader);
    delete(bush1Shader);
    delete(bush2Shader);
    delete(bush3Shader);
    delete(bush4Shader);
    delete(bush5Shader);
    delete(cowboyHandsShader);
    delete(cowboyBodyShader);
    delete(guitarMetalShader);
    delete(guitarPlasticShader);
    delete(guitarWood1Shader);
    delete(guitarWood2Shader);
    delete(SWAT1Shader);
    delete(SWAT2Shader);
    delete(SWAT3Shader);
    delete(girlShader);
    delete(hairShader);
    delete(zombie1Shader);
    delete(zombie2Shader);
    delete(sisterShader);
    delete(baseNode);
    delete(logoUnitNode);
    delete(logoNode);
    delete(logoLabelNode);
    delete(loadingLabelNode);
    delete(instructionsLabelNode);
    delete(backgroundNode);
    delete(curtainNode);
    delete(sceneNode);
    delete(moonlightNode);
    delete(bonfireNode);
    delete(bonfireLightNode);
    delete(fireNode);
    delete(smokeNode);
    delete(sparkNode);
    delete(cowboyNode);
    delete(guitarNode);
    delete(SWATNode);
    delete(girlNode);
    delete(zombieNode);
    delete(sisterNode);
    delete(skybox);
    
    
    
    // Delete the engine:
    delete(engine);
    return(0);
}
