// Developed by Kelin Lyu.
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec2 birthTimeAndDuration;
layout (location = 3) in vec3 initialPosition;
layout (location = 4) in vec3 initialSpeed;
layout (location = 5) in vec4 accelerationData;
layout (location = 6) in vec4 rotationData;
layout (location = 7) in vec4 scaleData;
layout (location = 8) in vec2 spriteSheetAnimationData;
out fragment_data {
    float progress;
    vec2 UV;
    vec2 spriteSheetAnimationData;
} fragment;
uniform float time;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelViewTransform;
uniform bool useLocalSpace;
void main() {
    float progress = time - birthTimeAndDuration.x;
    if(progress > birthTimeAndDuration.y) {
        gl_Position = vec4(0.0f);
        return;
    }
    float accelerationFactor = progress * progress * 0.5f;
    vec4 particlePosition = vec4(initialPosition, 1.0f);
    particlePosition.xyz += initialSpeed * progress;
    particlePosition.xyz += initialSpeed * accelerationData.w * accelerationFactor;
    particlePosition.xyz += accelerationData.xyz * accelerationFactor;
    vec4 localVertexPosition = vec4(0.0f);
    float rotation = rotationData.x + rotationData.y * progress;
    localVertexPosition.x = cos(rotation) * vertexPosition.x - sin(rotation) * vertexPosition.y;
    localVertexPosition.y = sin(rotation) * vertexPosition.x + cos(rotation) * vertexPosition.y;
    localVertexPosition.xy *= max(0.0f, scaleData.x + scaleData.y * progress);
    if(useLocalSpace) {
        gl_Position = projectionTransform * (modelViewTransform * particlePosition + localVertexPosition);
    }else{
        gl_Position = projectionTransform * (viewTransform * particlePosition + localVertexPosition);
    }
    fragment.progress = progress / birthTimeAndDuration.y;
    fragment.UV = vertexUV;
    fragment.spriteSheetAnimationData = spriteSheetAnimationData;
}
