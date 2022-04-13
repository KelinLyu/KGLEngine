// Developed by Kelin Lyu.
#ifndef Animation_hpp
#define Animation_hpp
#include "../Engine.hpp"
struct AnimationBoneNode {
    string name;
    mat4 transform;
    vector<AnimationBoneNode*> children;
};
class Bone;
class Geometry;
class Animator;
class Animation final {
private:
    Animator* animator;
    AnimationBoneNode* rootAnimationBoneNode;
    vector<AnimationBoneNode*> animationBoneNodes;
    vector<Bone*> bones;
public:
    Animation(const aiScene* scene, Animator* animator, Geometry* geometry);
    ~Animation();
    void engineProcessNode(AnimationBoneNode* target, aiNode* node);
    Animator* engineGetAnimator();
    AnimationBoneNode* engineGetRootAnimationBoneNode();
    Bone* engineGetBone(string name);
    void engineEraseAnimator();
};
#endif