// Developed by Kelin Lyu.
#version 330 core
const int LIGHTS_LIMIT = 10;
in fragment_data {
    vec3 position;
    vec3 normal;
    vec2 UV;
    mat3 TBN;
    mat3 inverseTBN;
} fragment;
out vec4 color;
struct frame_data {
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
    float innerAngle;
    float outerAngle;
};
uniform frame_data frame;
uniform node_data node;
uniform light_data lights[LIGHTS_LIMIT];
uniform int lightCount;
uniform float opacity;
uniform vec4 defaultDiffuseColor;
uniform bool useDiffuseMap;
uniform sampler2D diffuseMap;
uniform float diffuseIntensity;
uniform float alphaCutThreshold;
uniform bool useNormalMap;
uniform sampler2D normalMap;
uniform float normalIntensity;
uniform bool useHeightMap;
uniform sampler2D heightMap;
uniform float heightIntensity;
uniform vec2 heightLayerRange;
uniform float defaultMetallic;
uniform bool useMetallicMap;
uniform sampler2D metallicMap;
uniform float metallicIntensity;
uniform bool invertMetallic;
uniform float defaultRoughness;
uniform bool useRoughnessMap;
uniform sampler2D roughnessMap;
uniform float roughnessIntensity;
uniform bool invertRoughness;
uniform bool useReflectionMap;
uniform sampler2D ReflectionMap;
uniform float reflectionIntensity;
uniform bool useAmbientOcclusionMap;
uniform sampler2D ambientOcclusionMap;
uniform float ambientOcclusionIntensity;
uniform vec3 defaultMultiplyColor;
uniform bool useMultiplyMap;
uniform sampler2D multiplyMap;
uniform float multiplyIntensity;
uniform vec3 defaultEmissionColor;
uniform bool useEmissionMap;
uniform sampler2D emissionMap;
uniform float emissionIntensity;
void main() {
    vec2 UV = fragment.UV;
    if(useHeightMap) {
        vec3 cameraVector = frame.cameraPosition - fragment.position;
        vec3 cameraDirection = normalize(fragment.inverseTBN * cameraVector);
        float factor = abs(dot(vec3(0.0f, 0.0f, 1.0f), cameraDirection));
        float layerCount = mix(heightLayerRange.x, heightLayerRange.y, factor);
        float layerHeight = 1.0f / layerCount;
        float currentLayerHeight = 0.0f;
        factor = -dot(fragment.normal, cameraVector);
        vec2 deltaUV = cameraDirection.xy / cameraDirection.z;
        deltaUV *= 0.1f * heightIntensity * factor / layerCount;
        vec2  currentUV = UV;
        float currentHeight = texture(heightMap, currentUV).r;
        while(currentLayerHeight < currentHeight) {
            currentUV -= deltaUV;
            currentHeight = texture(heightMap, currentUV).r;
            currentLayerHeight += layerHeight;
        }
        vec2 previousUV = currentUV + deltaUV;
        float previousHeight = texture(heightMap, previousUV).r - currentLayerHeight + layerHeight;
        float newHeight  = currentHeight - currentLayerHeight;
        float weight = newHeight / (newHeight - previousHeight);
        UV = previousUV * weight + currentUV * (1.0f - weight);
    }
    vec4 diffuseColor = defaultDiffuseColor;
    if(useDiffuseMap) {
        diffuseColor = texture(diffuseMap, UV);
        if(diffuseColor.a < alphaCutThreshold) {
            discard;
        }
    }
    diffuseColor *= diffuseIntensity;
    vec3 normal;
    if(useNormalMap) {
        normal = texture(normalMap, UV).rgb;
        normal = normal * 2.0f - 1.0f;
        normal.xy *= normalIntensity;
        normal = normalize(fragment.TBN * normal);
    }else{
        normal = normalize(fragment.TBN * vec3(0.0f, 0.0f, 1.0f));
    }
    float metallic = defaultMetallic;
    if(useMetallicMap) {
        metallic = texture(metallicMap, UV).r;
        if(invertMetallic) {
            metallic = 1.0f - metallic;
        }
        metallic *= metallicIntensity;
    }
    metallic = max(0.0f, min(metallic, 1.0f));
    metallic = 0.2f + 0.6f * metallic;
    float roughness = defaultRoughness;
    if(useRoughnessMap) {
        roughness = texture(roughnessMap, UV).r;
        if(invertRoughness) {
            roughness = 1.0f - roughness;
        }
        roughness *= roughnessIntensity;
    }
    roughness = max(0.0f, min(roughness, 1.0f));
    roughness = 0.2f + 0.6f * roughness;
    vec3 viewVector = normalize(frame.cameraPosition - fragment.position);
    vec3 lightingColor = vec3(0.0f);
    float roughness4 = roughness * roughness * roughness * roughness;
    float roughness4OverPi = roughness4 / 3.1416f;
    float roughness4MinusOne = roughness4 - 1.0f;
    for(int i = 0; i < lightCount; i += 1) {
        if(lights[i].type == 0) {
            lightingColor += diffuseColor.rgb * lights[i].colorFactor;
            continue;
        }
        vec3 lightVector;
        float lightFactor;
        if(lights[i].type == 1) {
            lightVector = -lights[i].direction;
            lightFactor = max(dot(normal, lightVector), 0.0f);
        }else if(lights[i].type == 2) {
            lightVector = lights[i].position - fragment.position;
            float lightDistance = length(lightVector);
            float attenuationProgress = max((lights[i].range - lightDistance) / lights[i].range, 0.0f);
            float attenuation = pow(attenuationProgress, lights[i].attenuationExponent);
            lightVector = normalize(lightVector);
            lightFactor = max(dot(normal, lightVector), 0.0f) * attenuation;
        }else if(lights[i].type == 3) {
            lightVector = lights[i].position - fragment.position;
            float lightDistance = length(lightVector);
            float attenuationProgress = max((lights[i].range - lightDistance) / lights[i].range, 0.0f);
            float attenuation = pow(attenuationProgress, lights[i].attenuationExponent);
            lightVector = normalize(lightVector);
            float theta = dot(lightVector, -lights[i].direction);
            float epsilon = lights[i].innerAngle - lights[i].outerAngle;
            float intensity = (theta - lights[i].outerAngle) / epsilon;
            attenuation *= clamp(intensity, 0.0f, 1.0f);
            lightFactor = max(dot(normal, lightVector), 0.0f) * attenuation;
        }
        if(lightFactor <= 0.0f) {
            continue;
        }
        vec3 halfwayVector = normalize(viewVector + lightVector);
        float NdotH = max(dot(normal, halfwayVector), 0.0f);
        float NdotH2 = NdotH * NdotH;
        float denominator = (NdotH2 * roughness4MinusOne + 1.0f);
        denominator = denominator * denominator;
        float NDF = roughness4OverPi / denominator;
        float maxHdotV = max(dot(halfwayVector, viewVector), 0.0f);
        float factor = clamp(1.0f - maxHdotV, 0.0f, 1.0f);
        float F = (metallic + (1.0 - metallic) * factor);
        lightingColor += ((1.0 - F) * diffuseColor.rgb / 3.1416f) * lightFactor * lights[i].colorFactor;
        lightingColor += NDF * diffuseColor.rgb * lightFactor * lights[i].highlightFactor;
    }
    vec3 F0 = mix(vec3(0.1f), diffuseColor.rgb, metallic);
    float cosTheta = max(dot(normal, viewVector), 0.0f);
    float factor = clamp(1.0f - cosTheta, 0.0f, 1.0f);
    vec3 F = F0 + (max(vec3(1.0f - roughness), F0) - F0) * factor;
    vec3 kD = (1.0f - F) * (1.0f - metallic);
    color = vec4(kD * diffuseColor.rgb + lightingColor, diffuseColor.a * opacity);
    if(useReflectionMap) {
        vec3 reflectionVector = reflect(-viewVector, normal);
        vec2 uv = vec2(atan(reflectionVector.z, reflectionVector.x), -asin(reflectionVector.y));
        uv = uv * vec2(0.1592f, 0.3183f) + 0.5f;
        vec3 reflection = texture(ReflectionMap, uv).rgb * reflectionIntensity;
        color.rgb += reflection * (metallic) * (1.0 - roughness);
    }
    if(useAmbientOcclusionMap) {
        float ambientOcclusion = texture(ambientOcclusionMap, UV).r;
        float inverseAmbientOcclusion = 1.0f - ambientOcclusion;
        ambientOcclusion = 1.0f - inverseAmbientOcclusion * ambientOcclusionIntensity;
        color.rgb *= ambientOcclusion;
    }
    color.rgb = color.rgb / (color.rgb + vec3(1.0f));
    vec3 multiplyColor = defaultMultiplyColor;
    if(useMultiplyMap) {
        multiplyColor = texture(multiplyMap, UV).rgb;
    }
    vec3 inverseMultiplyColor = vec3(1.0f) - multiplyColor;
    multiplyColor = vec3(1.0f) - inverseMultiplyColor * multiplyIntensity;
    color.rgb *= multiplyColor;
    vec3 emissionColor = defaultEmissionColor;
    if(useEmissionMap) {
        emissionColor = texture(emissionMap, UV).rgb * emissionIntensity;
    }
    color.rgb += emissionColor;
}
