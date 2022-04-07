// Developed by Kelin.Lyu.
#ifndef bone_hpp
#define bone_hpp
#include "Toolkit/common.h"
#include "core.hpp"
struct BoneInfo {
    int id;
    mat4 offset;
};
struct KeyFramePosition {
    vec3 position;
    float timeStamp;
};
struct KeyFrameRotation {
    quat orientation;
    float timeStamp;
};
struct KeyFrameScale {
    vec3 scale;
    float timeStamp;
};
class Bone {
private:
    string name;
    mat4 transform;
    vector<KeyFramePosition> keyPositions;
    vector<KeyFrameRotation> keyRotations;
    vector<KeyFrameScale> keyScales;
public:
    Bone(string name, aiNodeAnim* channel);
    void update(float animationTime);
    string getName() const;
    mat4 getTransform();
};
#endif
