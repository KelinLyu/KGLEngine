// Developed by Kelin Lyu.
#version 330 core
const int MAX_BONE_INFLUENCE = 4;
const int BONES_LIMIT = 120;
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;
layout(location = 5) in ivec4 boneIDs;
layout(location = 6) in vec4 weights;
out fragment_data {
    vec3 position;
    vec3 normal;
    vec2 UV;
    mat3 TBN;
    mat3 inverseTBN;
} fragment;
struct node_data {
    mat4 modelTransform;
    mat4 normalTransform;
    mat4 modelViewProjectionTransform;
};
uniform node_data node;
uniform bool hasBones;
uniform mat4 boneTransforms[BONES_LIMIT];
void main() {
    if(hasBones) {
        mat4 boneTransform = mat4(0.0f);
        for(int i = 0; i < MAX_BONE_INFLUENCE; i += 1) {
            boneTransform += boneTransforms[boneIDs[i]] * weights[i];
        }
        vec4 localPosition = boneTransform * vec4(vertexPosition, 1.0f);
        gl_Position = node.modelViewProjectionTransform * localPosition;
        fragment.position = vec3(node.modelTransform * localPosition);
        vec4 localNormal = boneTransform * vec4(vertexNormal, 0.0f);
        fragment.normal = normalize(vec3(node.normalTransform * localNormal));
        vec3 T = normalize(vec3(node.modelTransform * boneTransform * vec4(vertexTangent, 0.0f)));
        vec3 B = normalize(vec3(node.modelTransform * boneTransform * vec4(vertexBitangent, 0.0f)));
        vec3 N = normalize(vec3(node.modelTransform * boneTransform * vec4(vertexNormal, 0.0f)));
        fragment.TBN = mat3(T, B, N);
    }else{
        vec4 localPosition = vec4(vertexPosition, 1.0f);
        gl_Position = node.modelViewProjectionTransform * localPosition;
        fragment.position = vec3(node.modelTransform * vec4(vertexPosition, 1.0f));
        fragment.normal = normalize(mat3(node.normalTransform) * vertexNormal);
        vec3 T = normalize(vec3(node.modelTransform * vec4(vertexTangent, 0.0f)));
        vec3 B = normalize(vec3(node.modelTransform * vec4(vertexBitangent, 0.0f)));
        vec3 N = normalize(vec3(node.modelTransform * vec4(vertexNormal, 0.0f)));
        fragment.TBN = mat3(T, B, N);
    }
    fragment.inverseTBN = inverse(fragment.TBN);
    fragment.UV = vertexUV;
}
