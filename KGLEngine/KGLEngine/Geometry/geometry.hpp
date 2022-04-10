// Developed by Kelin.Lyu.
#ifndef geometry_hpp
#define geometry_hpp
#include "../core.hpp"
#define MAX_BONE_INFLUENCE 4
#define BONES_LIMIT 120
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};
struct BoneInfo;
struct AssimpNode;
class Shader;
class Animation;
class LightNode;
class Geometry {
protected:
    Shader* shader;
    unsigned int vertexArrays;
    unsigned int vertexBuffers;
    unsigned int elementBuffers;
    unsigned int indiceCount;
    vector<Animation*> animations;
    int bonesCount;
    map<string, BoneInfo> bonesInfoMap;
    vector<mat4> boneTransforms;
    void calculateBoneTransforms(AssimpNode* node, mat4 parentTransform, bool first);
    mat4 worldTransform;
    int cullMode;
public:
    bool isHidden;
    int renderingOrder;
    unsigned int lightMask;
    unsigned int shadowMask;
    Geometry() = default;
    Geometry(aiMesh* mesh);
    void cullBack();
    void cullFront();
    void doubleSided();
    void setShader(Shader* shader);
    void addAnimation(Animation* animation);
    void update(mat4 worldTransform);
    void prepareForRendering();
    void render(vector<LightNode*>* lights);
    bool hasBones();
    map<string, BoneInfo>& getBonesInfoMap();
    int& getBonesCount();
    mat4 getBoneWorldTransform(string name);
    ~Geometry();
};
class UnitCube: public Geometry {
public:
    UnitCube();
};
#endif
