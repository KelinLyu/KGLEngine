// Developed by Kelin Lyu.
#ifndef Geometry_hpp
#define Geometry_hpp
#include "../Engine.hpp"
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
struct AnimationBoneNode;
class Shader;
class Material;
class Animation;
class Geometry {
protected:
    unsigned int cullMode;
    Shader* shader;
    Material* material;
    unsigned int vertexArrays;
    unsigned int vertexBuffers;
    unsigned int elementBuffers;
    unsigned int indiceCount;
    vector<Animation*> animations;
    int bonesCount;
    map<string, BoneInfo> bonesInfoMap;
    vector<mat4> boneTransforms;
    mat4 modelTransform;
public:
    bool isHidden;
    int renderingOrder;
    unsigned int lightMask;
    Geometry() = default;
    Geometry(aiMesh* mesh);
    void cullBack();
    void cullFront();
    void doubleSided();
    void setShader(Shader* shader);
    void setMaterial(Material* material);
    ~Geometry();
    void engineInitializeGeometry();
    mat4 engineGetGeometryModelTransform();
    bool engineCheckIfGeometryHasBones();
    int& engineGetGeometryBonesCountReference();
    map<string, BoneInfo>& engineGetGeometryBonesInfoMapReference();
    vector<mat4>& engineGetGeometryBoneTransformsReference();
    void engineCalculateGeometryBoneTransforms(AnimationBoneNode* node, mat4 parentTransform, bool first);
    mat4 engineGetGeometryBoneTransform(string name);
    void engineAddAnimationToGeometry(Animation* animation);
    void engineUpdateGeometryAnimations();
    void enginePrepareGeometryForRendering(mat4 worldTransform);
    void engineRenderGeometry();
};
class UnitCube: public Geometry {
public:
    UnitCube();
};
#endif
