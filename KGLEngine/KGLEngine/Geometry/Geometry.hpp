// Developed by Kelin Lyu.
#ifndef Geometry_hpp
#define Geometry_hpp
#include "../Engine.hpp"
#define MAX_BONE_INFLUENCE 4
#define BONES_LIMIT 120
struct BoneInfo;
class Texture;
class Shader;
class Animator;
class Font;
class Node;
class LightNode;
struct GeometryVertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};
struct ParticleData {
    vec2 birthTimeAndDuration;
    vec3 initialPosition;
    vec3 initialSpeed;
    vec4 accelerationData;
    vec2 rotationData;
    vec2 scaleData;
    vec2 spriteSheetAnimationData;
};
class Geometry {
protected:
    bool updated;
    bool prepared;
    unsigned int cullMode;
    Shader* shader;
    unsigned int vertexArrays;
    unsigned int vertexBuffers;
    unsigned int elementBuffers;
    unsigned int indiceCount;
    unsigned int boneCount;
    vector<string> boneNames;
    vector<BoneInfo> boneInfos;
    vector<mat4> boneTransforms;
    bool skeletalAnimationUpdated;
    mat4 modelTransform;
    unsigned int modelTransformBuffers;
    unsigned int normalTransformBuffers;
    unsigned int instanceCount;
    vector<bool> frozenTransforms;
    vector<mat4> modelTransforms;
    vector<mat4> normalTransforms;
    bool requiresInstanceUpdate;
    bool hasBoundingSphereInformation;
public:
    bool isHidden;
    float renderingOrder;
    unsigned int lightingBitMask;
    vec3 boundingSpherePosition;
    float boundingSphereRadius;
    unsigned int affectedLightCount;
    Geometry() = default;
    Geometry(aiMesh* mesh);
    Geometry* copy(vector<Animator*>* animators);
    void cullBack();
    void cullFront();
    void setDoubleSided();
    void setShader(Shader* shader);
    ~Geometry();
    void engineInitializeGeometry();
    mat4 engineGetGeometryModelTransform();
    unsigned int engineGetGeometryVertexArrays();
    unsigned int engineGetGeometryIndiceCount();
    bool engineCheckWhetherGeometryHasBones();
    vector<mat4>* engineGetGeometryBoneTransforms();
    mat4 engineGetGeometryBoneTransform(string name);
    void engineUpdateGeometryBoneIndices(vector<string>* boneNames);
    void engineUpdateGeometrySkeletalAnimations(vector<mat4> boneTransforms);
    bool engineCheckWhetherGeometryHasUpdatedSkeletalAnimations();
    void enginePrepareGeometryForRendering(mat4 worldTransform);
    virtual void engineRenderGeometry(bool shadowMap);
    unsigned int engineGeometryAddInstance();
    void engineUpdateGeometryInstanceTransform(unsigned int index, mat4 modelTransform, bool freeze);
    virtual unsigned int engineGetGeometryInstanceCount();
    bool engineCheckWhetherGeometryIsAffectedByLightNode(LightNode* lightNode);
};
class UnitCube final: public Geometry {
public:
    UnitCube();
};
class ParticleRenderer final: public Geometry {
private:
    unsigned int particleAmount;
    unsigned int dataBuffers;
    vector<ParticleData> dataVector;
public:
    ParticleRenderer(unsigned int amount);
    ~ParticleRenderer();
    void engineResetAllParticleData();
    void engineRenderGeometry(bool shadowMap) override;
    ParticleData* engineGetParticleData(bool front);
    unsigned int engineGetGeometryInstanceCount() override;
};
class Skybox final: public Geometry {
private:
    Texture* texture;
public:
    Skybox(string right, string left,
           string top, string bottom,
           string front, string back,
           float maxAnisotropy);
    ~Skybox();
    void engineRenderGeometry(bool shadowMap) override;
};
class Sprite final: public Geometry {
public:
    Sprite();
    ~Sprite() = default;
};
class TextRenderer final: public Geometry {
private:
    vector<Texture*> textures;
    vector<mat4> transforms;
    mat4 mainTransform;
public:
    TextRenderer();
    ~TextRenderer();
    void engineRenderGeometry(bool shadowMap) override;
    void engineSetTextRendererAlpha(float alpha);
    void engineSetTextRendererColor(vec4 color);
    void engineSetTextRendererTexturesAndTransforms(vector<Texture*> textures, vector<mat4> transforms);
    void engineClearTextRendererTexturesAndTransforms();
    void engineSetTextRendererMainTransform(mat4 mainTransform);
};
#endif
