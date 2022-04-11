// Developed by Kelin.Lyu.
#ifndef node_hpp
#define node_hpp
#include "../core.hpp"
#define LIGHTS_LIMIT 10
struct Vertex;
class Shader;
class Geometry;
class Animator;
class Node {
private:
    void processNode(aiNode* node, const aiScene* scene);
    mat4 worldTransform;
protected:
    string classType;
public:
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
    vector<Geometry*> geometries;
    vector<Animator*> animators;
    vector<Node*> children;
    Node* parent;
    bool isHidden;
    Node();
    void loadUnitCube();
    void loadFile(string file);
    Animator* loadAnimator(string file);
    void addChild(Node* node);
    void update(mat4 parentTransform, float deltaTime);
    void updateTransform();
    void prepareForRendering(mat4 parentTransform);
    void calculateWorldTransform(mat4 parentTransform);
    vec3 getWorldPosition();
    mat4 getWorldTransform();
    vec3 convertLocalPositionToWorld(vec3 localPosition);
    vec3 convertLocalVectorToWorld(vec3 localVector);
    vec3 getFrontVectorInWorld();
    vec3 getBackVectorInWorld();
    vec3 getLeftVectorInWorld();
    vec3 getRightVectorInWorld();
    vec3 getUpVectorInWorld();
    vec3 getDownVectorInWorld();
    mat4 getBoneWorldTransform(string boneName);
    void stickToBoneOfNode(string boneName, Node* node);
    ~Node();
};
class CameraNode: public Node {
public:
    float field;
    float near;
    float far;
    CameraNode(float field, float near, float far);
    mat4 getProjectionTransform();
    mat4 getViewTransform();
};
class LightNode: public Node {
public:
    int type;
    vec3 color;
    float attenuationExponent;
    float range;
    float innerAngle;
    float outerAngle;
    unsigned int lightMask;
    unsigned int shadowMask;
    LightNode(vec3 color);
    void setAmbientLight();
    void setDirectionalLight();
    void setPointLight(float attenuationExponent, float range);
    void setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle);
    void configurateShader(Shader* shader, int index, mat4 worldTransform);
};
#endif
