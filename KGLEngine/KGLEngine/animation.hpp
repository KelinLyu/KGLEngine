// Developed by Kelin.Lyu.
#ifndef animation_hpp
#define animation_hpp
#include "Toolkit/common.h"
#include "core.hpp"
struct AssimpNode {
    string name;
    mat4 transform;
    vector<AssimpNode*> children;
};
class Bone;
class Geometry;
class Animator;
class Animation {
private:
    void processNode(AssimpNode* target, aiNode* node);
public:
    Animator* animator;
    Geometry* geometry;
    Animation(const aiScene* scene, Animator* animator, Geometry* geometry);
    Bone* getBone(string name);
    void update(float deltaTime, float blendFactor);
    vector<Bone*> bones;
    map<string, BoneInfo> bonesInfoMap;
    vector<mat4> boneTransforms;
    AssimpNode* assimpRootNode;
};
#endif
