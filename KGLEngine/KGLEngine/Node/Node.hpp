// Developed by Kelin Lyu.
#ifndef Node_hpp
#define Node_hpp
#include "../Engine.hpp"
#define LIGHTS_LIMIT 10
class Geometry;
class Animator;
class LightNode;
class Node {
private:
    vector<Node*> childNodes;
    LightNode* lightNode;
    mat4 worldTransform;
    map<string, Node*> boneNodes;
public:
    string name;
    unsigned int tags;
    Node* parent;
    bool isHidden;
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
    vector<Geometry*> geometries;
    vector<Animator*> animators;
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
    ~Node();
    void engineInitializeNode();
    void engineNodeSetLightNode(LightNode* node);
    void engineProcessNode(aiNode* node, const aiScene* scene);
    void engineUpdateNodeAnimators(mat4 parentWorldTransform);
    void enginePrepareNodeForRendering(mat4 parentWorldTransform);
    void engineCalculateNodeWorldTransform(mat4 parentWorldTransform);
};
class CameraNode: public Node {
public:
    float field;
    float near;
    float far;
    CameraNode(float field, float near, float far);
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
#endif
