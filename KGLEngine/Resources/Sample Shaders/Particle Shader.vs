// Developed by Kelin Lyu.
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

uniform float time;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelViewTransform;
uniform vec3 baseWorldPosition;


void main() {
    
    UV = vertexUV;
    gl_Position = projectionTransform * (viewTransform * vec4(baseWorldPosition, 1.0f) + vec4(vertexPosition.xy, 0, 0));
    
    
    
}
