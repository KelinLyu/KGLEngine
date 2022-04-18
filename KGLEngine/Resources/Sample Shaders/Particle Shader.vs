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
    
    
    vec2 localPosition = vertexPosition;
    
    //localPosition.x = cos(time) * vertexPosition.x - sin(time) * vertexPosition.y;
    //localPosition.y = sin(time) * vertexPosition.x + cos(time) * vertexPosition.y;
    
    
    
    //localPosition *= sin(time) * 0.5f + 0.5f;
    
    
    gl_Position = projectionTransform * (viewTransform * vec4(baseWorldPosition, 1.0f) + vec4(localPosition, 0.0f, 0.0f));
    
    
    
}
