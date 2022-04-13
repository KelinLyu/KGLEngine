// Developed by Kelin Lyu.
#ifndef Bone_hpp
#define Bone_hpp
#include "../Engine.hpp"
struct BoneInfo {
    int id;
    mat4 offset;
};
struct KeyFramePosition {
    vec3 position;
    float timestamp;
};
struct KeyFrameRotation {
    quat orientation;
    float timestamp;
};
struct KeyFrameScale {
    vec3 scale;
    float timestamp;
};
class Bone final {
private:
    string name;
    mat4 transform;
    vector<KeyFramePosition> keyPositions;
    vector<KeyFrameRotation> keyRotations;
    vector<KeyFrameScale> keyScales;
public:
    Bone(string name, aiNodeAnim* channel);
    ~Bone();
    string engineGetName() const;
    mat4 engineGetTransform();
    void engineUpdateBoneAnimation(float animationTime);
};
#endif
