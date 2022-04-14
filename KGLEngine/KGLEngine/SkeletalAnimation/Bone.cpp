// Developed by Kelin Lyu.
#include "Bone.hpp"
Bone::Bone(string name, aiNodeAnim* channel) {
    this->name = name;
    this->transform = mat4(1.0f);
    for(unsigned int i = 0; i < channel->mNumPositionKeys; i += 1) {
        aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
        float timestamp = channel->mPositionKeys[i].mTime;
        BoneKeyFramePosition data;
        data.position = assimp_helper::getVec3(aiPosition);
        data.timestamp = timestamp;
        this->keyPositions.push_back(data);
    }
    for(unsigned int i = 0; i < channel->mNumRotationKeys; i += 1) {
        aiQuaternion aiOrientation = channel->mRotationKeys[i].mValue;
        float timestamp = channel->mRotationKeys[i].mTime;
        BoneKeyFrameRotation data;
        data.orientation = assimp_helper::getQuat(aiOrientation);
        data.timestamp = timestamp;
        this->keyRotations.push_back(data);
    }
    for(unsigned int i = 0; i < channel->mNumScalingKeys; i += 1) {
        aiVector3D scale = channel->mScalingKeys[i].mValue;
        float timestamp = channel->mScalingKeys[i].mTime;
        BoneKeyFrameScale data;
        data.scale = assimp_helper::getVec3(scale);
        data.timestamp = timestamp;
        this->keyScales.push_back(data);
    }
}
Bone::~Bone() {
    this->keyPositions.clear();
    this->keyRotations.clear();
    this->keyScales.clear();
}
string Bone::engineGetName() const {
    return(this->name);
}
mat4 Bone::engineGetTransform() {
    return(this->transform);
}
void Bone::engineUpdateBoneAnimation(float animationTime) {
    mat4 translationTransform = mat4(1.0f);
    if(this->keyPositions.size() == 1) {
        translationTransform = glm::translate(mat4(1.0f), this->keyPositions[0].position);
    }else{
        int index = -1;
        for(unsigned int i = 0; i < this->keyPositions.size() - 1; i += 1) {
            if(animationTime < this->keyPositions[i + 1].timestamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyPositions[index + 1].timestamp - this->keyPositions[index].timestamp;
            float factor = (animationTime - this->keyPositions[index].timestamp) / base;
            vec3 p = mix(this->keyPositions[index].position, this->keyPositions[index + 1].position, factor);
            translationTransform = glm::translate(mat4(1.0f), p);
        }
    }
    mat4 rotationTransform = mat4(1.0f);
    if(this->keyRotations.size() == 1) {
        rotationTransform = glm::toMat4(normalize(this->keyRotations[0].orientation));
    }else{
        int index = -1;
        for(unsigned int i = 0; i < this->keyPositions.size() - 1; i += 1) {
            if(animationTime < this->keyPositions[i + 1].timestamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyRotations[index + 1].timestamp - this->keyRotations[index].timestamp;
            float factor = (animationTime - this->keyRotations[index].timestamp) / base;
            quat r = slerp(keyRotations[index].orientation, keyRotations[index + 1].orientation, factor);
            rotationTransform = toMat4(normalize(r));
        }
    }
    mat4 scaleTransform = mat4(1.0f);
    if(this->keyScales.size() == 1) {
        scaleTransform = glm::scale(mat4(1.0f), this->keyScales[0].scale);
    }else{
        int index = -1;
        for(unsigned int i = 0; i < this->keyPositions.size() - 1; i += 1) {
            if(animationTime < this->keyScales[i + 1].timestamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyScales[index + 1].timestamp - this->keyScales[index].timestamp;
            float factor = (animationTime - this->keyScales[index].timestamp) / base;
            vec3 s = mix(this->keyScales[index].scale, this->keyScales[index + 1].scale, factor);
            scaleTransform = glm::scale(mat4(1.0f), s);
        }
    }
    this->transform = translationTransform * rotationTransform * scaleTransform;
}
