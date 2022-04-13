// Developed by Kelin Lyu.
#ifndef Node_hpp
#define Node_hpp
#include "../Engine.hpp"
#define LIGHTS_LIMIT 10
class UIMaterial;
class Geometry;
class Sprite;
class Animator;
class LightNode;
class UINode;
class Node {
private:
    UINode* uiNode;
    LightNode* lightNode;
    vector<Node*> childNodes;
    mat4 worldTransform;
    mat4 uiWorldTransform;
    map<string, Node*> boneNodes;
    vector<Animator*> animators;
public:
    string name;
    unsigned int tags;
    Node* parent;
    bool isDisabled;
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
    vector<Geometry*> geometries;
    Node();
    void addChildNode(Node* node);
    void removeFromParentNode();
    void loadUnitCube();
    void loadModelFile(string file);
    Animator* loadAnimator(string file);
    Node* generateBoneNode(string boneName);
    void updateTransform();
    mat4 getWorldTransform();
    vec3 getWorldPosition();
    vec3 getWorldEulerAngles();
    vec3 getWorldScale();
    vec3 convertLocalPositionToWorld(vec3 localPosition);
    vec3 convertLocalVectorToWorld(vec3 localVector);
    vec3 getFrontVectorInWorld();
    vec3 getBackVectorInWorld();
    vec3 getLeftVectorInWorld();
    vec3 getRightVectorInWorld();
    vec3 getUpVectorInWorld();
    vec3 getDownVectorInWorld();
    vec3 getPositionOnScreen();
    ~Node();
    void engineInitializeNode();
    void engineNodeSetUINode(UINode* node);
    void engineNodeSetLightNode(LightNode* node);
    void engineProcessNode(aiNode* node, const aiScene* scene);
    void engineUpdateNodeAnimators(mat4 parentWorldTransform);
    void enginePrepareNodeForRendering(mat4 parentWorldTransform);
    void engineCalculateNodeWorldTransform(mat4 parentWorldTransform);
    float engineGetUINodeRenderingOrder();
};
class CameraNode: public Node {
public:
    float field;
    float near;
    float far;
    CameraNode(float field, float near, float far);
    mat4 getOrthogonalProjectionTransform();
    mat4 getProjectionTransform();
    mat4 getViewTransform();
    ~CameraNode() = default;
};
class LightNode: public Node {
private:
    unsigned int lightType;
public:
    vec3 colorFactor;
    vec3 highlightFactor;
    float attenuationExponent;
    float range;
    float innerAngle;
    float outerAngle;
    unsigned int lightMask;
    LightNode(vec3 color);
    void setAmbientLight();
    void setDirectionalLight();
    void setPointLight(float attenuationExponent, float range);
    void setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle);
    ~LightNode() = default;
    void engineConfigurateShader(Shader* shader, int index);
};
class UINode: public Node {
private:
    UIMaterial* material;
    float cumulativeRenderingOrder;
public:
    vec2 screenPosition;
    vec2 position;
    float rotation;
    vec2 scale;
    vec2 size;
    float alpha;
    vec4 color;
    Texture* texture;
    vec3 multiplyColor;
    Texture* multiplyTexture;
    float multiplyIntensity;
    vec3 emissionColor;
    Texture* emissionTexture;
    float emissionIntensity;
    float renderingOrder;
    UINode();
    UINode(vec2 size);
    void loadSprite(Sprite* sprite);
    void setAlpha();
    void setAdditive();
    ~UINode();
    void engineInitializeUINode();
    void enginePrepareUINodeForRendering();
    float engineGetUINodeCumulativeRenderingOrder();
};
#endif
