// Developed by Kelin.Lyu.
#include "bone.hpp"
Bone::Bone(string name, aiNodeAnim* channel) {
    this->name = name;
    this->transform = mat4(1.0f);
    for(unsigned int i = 0; i < channel->mNumPositionKeys; i += 1) {
        aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
        float timeStamp = channel->mPositionKeys[i].mTime;
        KeyFramePosition data;
        data.position = assimp_helper::getVec3(aiPosition);
        data.timeStamp = timeStamp;
        this->keyPositions.push_back(data);
    }
    for(unsigned int i = 0; i < channel->mNumRotationKeys; i += 1) {
        aiQuaternion aiOrientation = channel->mRotationKeys[i].mValue;
        float timeStamp = channel->mRotationKeys[i].mTime;
        KeyFrameRotation data;
        data.orientation = assimp_helper::getQuat(aiOrientation);
        data.timeStamp = timeStamp;
        this->keyRotations.push_back(data);
    }
    for(unsigned int i = 0; i < channel->mNumScalingKeys; i += 1) {
        aiVector3D scale = channel->mScalingKeys[i].mValue;
        float timeStamp = channel->mScalingKeys[i].mTime;
        KeyFrameScale data;
        data.scale = assimp_helper::getVec3(scale);
        data.timeStamp = timeStamp;
        this->keyScales.push_back(data);
    }
}
void Bone::update(float animationTime) {
    mat4 translationTransform = mat4(1.0f);
    if(this->keyPositions.size() == 1) {
        translationTransform = translate(mat4(1.0f), this->keyPositions[0].position);
    }else{
        int index = -1;
        for(unsigned int i = 0; i < keyPositions.size() - 1; i += 1) {
            if (animationTime < keyPositions[i + 1].timeStamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyPositions[index + 1].timeStamp - this->keyPositions[index].timeStamp;
            float factor = (animationTime - this->keyPositions[index].timeStamp) / base;
            vec3 p = mix(this->keyPositions[index].position, this->keyPositions[index + 1].position, factor);
            translationTransform = translate(mat4(1.0f), p);
        }
    }
    mat4 rotationTransform = mat4(1.0f);
    if(this->keyRotations.size() == 1) {
        rotationTransform = toMat4(normalize(this->keyRotations[0].orientation));
    }else{
        int index = -1;
        for(unsigned int i = 0; i < keyPositions.size() - 1; i += 1) {
            if (animationTime < keyPositions[i + 1].timeStamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyRotations[index + 1].timeStamp - this->keyRotations[index].timeStamp;
            float factor = (animationTime - this->keyRotations[index].timeStamp) / base;
            quat r = slerp(keyRotations[index].orientation, keyRotations[index + 1].orientation, factor);
            rotationTransform = toMat4(normalize(r));
        }
    }
    mat4 scaleTransform = mat4(1.0f);
    if(this->keyScales.size() == 1) {
        scaleTransform = scale(mat4(1.0f), this->keyScales[0].scale);
    }else{
        int index = -1;
        for(unsigned int i = 0; i < keyPositions.size() - 1; i += 1) {
            if (animationTime < keyScales[i + 1].timeStamp) {
                index = i;
                break;
            }
        }
        if(index != -1) {
            float base = this->keyScales[index + 1].timeStamp - this->keyScales[index].timeStamp;
            float factor = (animationTime - this->keyScales[index].timeStamp) / base;
            vec3 s = mix(this->keyScales[index].scale, this->keyScales[index + 1].scale, factor);
            scaleTransform = scale(mat4(1.0f), s);
        }
    }
    this->transform = translationTransform * rotationTransform * scaleTransform;
}
mat4 Bone::getTransform() {
    return(this->transform);
}
string Bone::getName() const {
    return(this->name);
}
