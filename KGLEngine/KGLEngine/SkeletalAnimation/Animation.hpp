// Developed by Kelin Lyu.
#ifndef Animation_hpp
#define Animation_hpp
#include "../Engine.hpp"
class Bone;
class Geometry;
class Animator;
struct AnimationBoneNode final {
    string name;
    mat4 transform;
    vector<AnimationBoneNode*> children;
};
class Animation final {
private:
    Animator* animator;
    AnimationBoneNode* rootAnimationBoneNode;
    map<string, Bone*> bones;
public:
    Animation() = default;
    Animation(const aiScene* scene, Animator* animator, Geometry* geometry);
    ~Animation();
    void engineAnimationProcessNode(AnimationBoneNode* targetNode, aiNode* node, map<string, BoneInfo>* bonesInfoMap);
    Animator* engineAnimationGetAnimator();
    AnimationBoneNode* engineGetRootAnimationBoneNode();
    Bone* engineAnimationGetBone(string name);
    void engineAnimationEraseAnimator();
    Animation* engineCopyAnimation(Animator* newAnimator);
};
#endif
