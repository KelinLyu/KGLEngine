// Developed by Kelin Lyu.
#ifndef Bone_hpp
#define Bone_hpp
#include "../Engine.hpp"
struct BoneInfo final {
    unsigned int id;
    mat4 offset;
    int index;
};
struct BoneKeyFramePosition final {
    vec3 position;
    float timestamp;
};
struct BoneKeyFrameRotation final {
    quat orientation;
    float timestamp;
};
struct BoneKeyFrameScale final {
    vec3 scale;
    float timestamp;
};
class Bone final {
private:
    string name;
    vec3 position;
    quat rotation;
    vec3 scale;
    unsigned int currentKeyPositionIndex;
    unsigned int currentKeyRotationIndex;
    unsigned int currentKeyScaleIndex;
    vector<BoneKeyFramePosition> keyPositions;
    vector<BoneKeyFrameRotation> keyRotations;
    vector<BoneKeyFrameScale> keyScales;
public:
    Bone() = default;
    Bone(string name, aiNodeAnim* channel);
    ~Bone();
    string engineGetName() const;
    vec3 engineGetBonePosition();
    quat engineGetBoneRotation();
    vec3 engineGetBoneScale();
    void engineUpdateBoneAnimation(float animationTime);
};
#endif
