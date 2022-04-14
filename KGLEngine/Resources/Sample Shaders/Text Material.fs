// Developed by Kelin Lyu.
#version 330 core
in vec2 UV;
out vec4 color;
uniform float alpha;
uniform vec4 defaultColor;
uniform bool useTextureMap;
uniform sampler2D textureMap;
uniform vec3 defaultMultiplyColor;
uniform bool useMultiplyTextureMap;
uniform sampler2D multiplyTextureMap;
uniform float multiplyIntensity;
uniform vec3 defaultEmissionColor;
uniform bool useEmissionTextureMap;
uniform sampler2D emissionTextureMap;
uniform float emissionIntensity;
void main() {
    color = defaultColor;
    if(useTextureMap) {
        color = texture(textureMap, UV);
    }
    vec3 multiplyColor = defaultMultiplyColor;
    if(useMultiplyTextureMap) {
        multiplyColor = texture(multiplyTextureMap, UV).rgb;
    }
    vec3 inverseMultiplyColor = vec3(1.0f) - multiplyColor;
    multiplyColor = vec3(1.0f) - inverseMultiplyColor * multiplyIntensity;
    color.rgb *= multiplyColor;
    vec3 emissionColor = defaultEmissionColor;
    if(useEmissionTextureMap) {
        emissionColor = texture(emissionTextureMap, UV).rgb;
    }
    emissionColor *= emissionIntensity;
    color.rgb += emissionColor;
    color.a *= alpha;
}
