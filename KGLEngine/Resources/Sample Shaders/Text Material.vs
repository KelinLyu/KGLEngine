// Developed by Kelin Lyu.
#version 330 core
layout (location = 0) in vec4 vertexData;
out vec2 UV;
uniform mat4 modelProjectionTransform;
void main() {
    UV = vertexData.zw;
    gl_Position = modelProjectionTransform * vec4(vertexData.xy, 0.0f, 1.0f);
}
