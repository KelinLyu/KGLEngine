// Developed by Kelin Lyu.
#ifndef Animation_hpp
#define Animation_hpp
#include "../Engine.hpp"
class Bone;
class Geometry;
class Animator;
struct AnimationBoneNode final {
    int boneIndex;
    string name;
    mat4 transform;
    vector<AnimationBoneNode*> children;
};
class Animation final {
private:
    Animator* animator;
    AnimationBoneNode* rootAnimationBoneNode;
    vector<Bone*> bones;
public:
    Animation() = default;
    Animation(const aiScene* scene, Animator* animator, Geometry* geometry);
    ~Animation();
    void engineProcessNode(AnimationBoneNode* targetNode, aiNode* node);
    void engineMatchAnimationBoneNodeWithBones(AnimationBoneNode* targetNode);
    Animator* engineGetAnimator();
    AnimationBoneNode* engineGetRootAnimationBoneNode();
    Bone* engineGetBone(int index);
    void engineEraseAnimator();
    Animation* engineCopyAnimation(Animator* newAnimator);
};
#endif
