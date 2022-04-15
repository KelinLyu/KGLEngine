// Developed by Kelin Lyu.
#ifndef Geometry_hpp
#define Geometry_hpp
#include "../Engine.hpp"
#define MAX_BONE_INFLUENCE 4
#define BONES_LIMIT 120
struct BoneInfo;
struct AnimationBoneNode;
class Texture;
class Shader;
class Animation;
class Font;
struct GeometryVertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};
class Geometry {
protected:
    unsigned int cullMode;
    Shader* shader;
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
    float renderingOrder;
    unsigned int lightMask;
    bool clearDepthBuffer;
    Geometry() = default;
    Geometry(aiMesh* mesh);
    void cullBack();
    void cullFront();
    void doubleSided();
    void setShader(Shader* shader);
    ~Geometry();
    void engineInitializeGeometry();
    mat4 engineGetGeometryModelTransform();
    unsigned int engineGetGeometryVertexArrays();
    unsigned int engineGetGeometryIndiceCount();
    bool engineCheckIfGeometryHasBones();
    int& engineGetGeometryBonesCountReference();
    map<string, BoneInfo>& engineGetGeometryBonesInfoMapReference();
    vector<mat4>& engineGetGeometryBoneTransformsReference();
    void engineCalculateGeometryBoneTransforms(AnimationBoneNode* node, mat4 parentTransform, bool first);
    mat4 engineGetGeometryBoneTransform(string name);
    void engineAddAnimationToGeometry(Animation* animation);
    void engineUpdateGeometryAnimations();
    void enginePrepareGeometryForRendering(mat4 worldTransform);
    virtual void engineRenderGeometry();
};
class UnitCube final: public Geometry {
public:
    UnitCube();
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
    void engineRenderGeometry() override;
};
class Sprite final: public Geometry {
public:
    Sprite();
    ~Sprite() = default;
    void engineRenderGeometry() override;
};
class TextRenderer final: public Geometry {
private:
    vector<Texture*> textures;
    vector<mat4> transforms;
    mat4 mainTransform;
public:
    TextRenderer();
    ~TextRenderer();
    void engineRenderGeometry() override;
    void engineSetTextRendererAlpha(float alpha);
    void engineSetTextRendererColor(vec4 color);
    void engineSetTextRendererTexturesAndTransforms(vector<Texture*> textures, vector<mat4> transforms);
    void engineClearTextRendererTexturesAndTransforms();
    void engineSetTextRendererMainTransform(mat4 mainTransform);
};
#endif
