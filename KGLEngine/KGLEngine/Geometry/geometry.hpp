// Developed by Kelin.Lyu.
#ifndef geometry_hpp
#define geometry_hpp
#include "../core.hpp"
#define MAX_BONE_INFLUENCE 4
#define BONES_LIMIT 100
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
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int vertexArrays;
    unsigned int vertexBuffers;
    unsigned int elementBuffers;
    vector<Animation*> animations;
    int bonesCount;
    map<string, BoneInfo> bonesInfoMap;
    vector<mat4> boneTransforms;
    void calculateBoneTransforms(AssimpNode* node, mat4 parentTransform, bool first);
    int renderingOrder;
    mat4 worldTransform;
public:
    bool isHidden;
    Geometry() = default;
    Geometry(aiMesh* mesh);
    void setShader(Shader* shader);
    void addAnimation(Animation* animation);
    void update(mat4 worldTransform);
    void prepareForRendering();
    void render(vector<LightNode*>* lights);
    bool hasBones();
    map<string, BoneInfo>& getBonesInfoMap();
    int& getBonesCount();
    void setRenderingOrder(int renderingOrder);
    int getRenderingOrder();
    mat4 getBoneWorldTransform(string name);
    ~Geometry();
};
class UnitCube: public Geometry {
public:
    UnitCube();
};
class SkyBoxCube: public Geometry {
public:
    SkyBoxCube();
};
#endif
