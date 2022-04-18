// Developed by Kelin Lyu.
#version 330 core


in vec2 UV;

out vec4 color;



uniform float time;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelViewTransform;
uniform vec3 baseWorldPosition;


uniform sampler2D diffuseMap;

void main() {
    
    
    color = vec4(sin(time) * 0.5f + 0.5f, 0.0f, 0.0f, 1.0f);
    
}
