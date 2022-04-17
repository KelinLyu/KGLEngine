// Developed by Kelin Lyu.
#ifndef Bone_hpp
#define Bone_hpp
#include "../Engine.hpp"
struct BoneInfo final {
    int id;
    mat4 offset;
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
    mat4 transform;
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
    mat4 engineGetTransform();
    void engineUpdateBoneAnimation(float animationTime);
};
#endif
