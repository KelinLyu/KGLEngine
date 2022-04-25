// Developed by Kelin Lyu.
#version 330 core
const int LIGHTS_LIMIT = 30;
const int SHADOWS_LIMIT = 6;
const int BONES_LIMIT = 120;
const int MAX_BONE_INFLUENCE = 4;
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;
layout (location = 7) in mat4 instancingModelTransform;
layout (location = 11) in mat4 instancingNormalTransform;
out fragment_data {
    vec3 position;
    vec3 normal;
    vec2 UV;
    mat3 TBN;
    mat3 inverseTBN;
    vec4 lightSpacePositions[SHADOWS_LIMIT];
} fragment;
struct frame_data {
    float time;
    mat4 viewProjectionTransform;
    vec3 cameraPosition;
    vec3 cameraDirection;
};
struct node_data {
    mat4 modelTransform;
    mat4 normalTransform;
    mat4 modelViewProjectionTransform;
};
struct light_data {
    int type;
    vec3 colorFactor;
    vec3 highlightFactor;
    vec3 position;
    vec3 direction;
    float attenuationExponent;
    float range;
    float penetrationRange;
    float innerAngle;
    float outerAngle;
    int shadowIndex;
};
struct shadow_data {
    int type;
    sampler2D shadowMap;
    mat4 lightSpaceMatrix;
    float bias;
    int samples;
};
uniform frame_data frame;
uniform node_data node;
uniform light_data lights[LIGHTS_LIMIT];
uniform int lightCount;
uniform shadow_data shadows[SHADOWS_LIMIT];
uniform int shadowCount;
uniform bool hasBones;
uniform mat4 boneTransforms[BONES_LIMIT];
uniform bool enableInstancing;
uniform int renderingMode;
uniform mat4 lightSpaceMatrix;
void main() {
    mat4 modelTransform = node.modelTransform;
    mat4 normalTransform = node.normalTransform;
    mat4 modelViewProjectionTransform = node.modelViewProjectionTransform;
    if(enableInstancing) {
        modelTransform = instancingModelTransform;
        normalTransform = instancingNormalTransform;
        modelViewProjectionTransform = frame.viewProjectionTransform * modelTransform;
    }
    if(renderingMode == 1) {
        if(hasBones) {
            mat4 boneTransform = mat4(0.0f);
            for(int i = 0; i < MAX_BONE_INFLUENCE; i += 1) {
                boneTransform += boneTransforms[boneIDs[i]] * weights[i];
            }
            vec4 localPosition = boneTransform * vec4(vertexPosition, 1.0f);
            gl_Position = lightSpaceMatrix * modelTransform * localPosition;
        }else{
            vec4 localPosition = vec4(vertexPosition, 1.0f);
            gl_Position = lightSpaceMatrix * modelTransform * localPosition;
        }
        return;
    }
    if(hasBones) {
        mat4 boneTransform = mat4(0.0f);
        for(int i = 0; i < MAX_BONE_INFLUENCE; i += 1) {
            boneTransform += boneTransforms[boneIDs[i]] * weights[i];
        }
        vec4 localPosition = boneTransform * vec4(vertexPosition, 1.0f);
        gl_Position = modelViewProjectionTransform * localPosition;
        fragment.position = vec3(modelTransform * localPosition);
        vec4 localNormal = boneTransform * vec4(vertexNormal, 0.0f);
        fragment.normal = normalize(vec3(normalTransform * localNormal));
        vec3 T = normalize(vec3(modelTransform * boneTransform * vec4(vertexTangent, 0.0f)));
        vec3 B = normalize(vec3(modelTransform * boneTransform * vec4(vertexBitangent, 0.0f)));
        vec3 N = normalize(vec3(modelTransform * boneTransform * vec4(vertexNormal, 0.0f)));
        fragment.TBN = mat3(T, B, N);
    }else{
        vec4 localPosition = vec4(vertexPosition, 1.0f);
        gl_Position = modelViewProjectionTransform * localPosition;
        fragment.position = vec3(modelTransform * vec4(vertexPosition, 1.0f));
        fragment.normal = normalize(mat3(normalTransform) * vertexNormal);
        vec3 T = normalize(vec3(modelTransform * vec4(vertexTangent, 0.0f)));
        vec3 B = normalize(vec3(modelTransform * vec4(vertexBitangent, 0.0f)));
        vec3 N = normalize(vec3(modelTransform * vec4(vertexNormal, 0.0f)));
        fragment.TBN = mat3(T, B, N);
    }
    fragment.inverseTBN = inverse(fragment.TBN);
    fragment.UV = vertexUV;
    for(int i = 0; i < shadowCount; i += 1) {
        if(shadows[i].type == 0) {
            fragment.lightSpacePositions[i] = shadows[i].lightSpaceMatrix * vec4(fragment.position, 1.0f);
        }
    }
}
