// Developed by Kelin.Lyu.
#version 330 core
in fragment_data {
    vec3 position;
    vec3 normal;
    vec2 UV;
    mat3 TBN;
    mat3 inverseTBN;
} fragment;
out vec4 color;
struct frame_data {
    mat4 viewTransform;
    mat4 inverseViewTransform;
    mat4 projectionTransform;
    mat4 inverseProjectionTransform;
    mat4 viewProjectionTransform;
    mat4 inverseViewProjectionTransform;
    float time;
    float random;
    vec3 cameraPosition;
    vec3 cameraDirection;
    mat4 cameraTransform;
    float cameraNear;
    float cameraFar;
};
struct node_data {
    mat4 modelTransform;
    mat4 inverseModelTransform;
    mat4 normalTransform;
    mat4 modelViewTransform;
    mat4 inverseModelViewTransform;
    mat4 modelViewProjectionTransform;
    mat4 inverseModelViewProjectionTransform;
};
const int LIGHTS_LIMIT = 10;
struct light_data {
    int type;
    vec3 color;
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
const float pi = 3.14159265359;
const float halfOverPi = 0.15915494309;
const float oneOverPi = 0.31830988618;
uniform float opacity;
uniform bool useFlatMaterial;
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
uniform float defaultRoughness;
uniform bool useRoughnessMap;
uniform sampler2D roughnessMap;
uniform float roughnessIntensity;
uniform bool useReflectionMap;
uniform sampler2D ReflectionMap;
uniform float reflectionIntensity;
uniform bool useAmbientOcclusionMap;
uniform sampler2D ambientOcclusionMap;
uniform float ambientOcclusionIntensity;
uniform vec4 defaultMultiplyColor;
uniform bool useMultiplyMap;
uniform sampler2D multiplyMap;
uniform float multiplyIntensity;
uniform vec4 defaultEmissionColor;
uniform bool useEmissionMap;
uniform sampler2D emissionMap;
uniform float emissionIntensity;
vec2 HDRIUV(vec3 vector) {
    vec2 uv = vec2(atan(vector.z, vector.x), -asin(vector.y));
    uv *= vec2(halfOverPi, oneOverPi);
    return(uv + 0.5f);
}
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float roughness4 = roughness * roughness * roughness * roughness;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
    float denominator = (NdotH2 * (roughness4 - 1.0f) + 1.0f);
    denominator = pi * denominator * denominator;
    return(roughness4 / denominator);
}
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;
    float nom = NdotV;
    float denominator = NdotV * (1.0f - k) + k;
    return(nom / denominator);
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    return(ggx1 * ggx2);
}
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    float factor = clamp(1.0f - cosTheta, 0.0f, 1.0f);
    factor = factor * factor * factor * factor * factor;
    return(F0 + (1.0 - F0) * factor);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    float factor = clamp(1.0f - cosTheta, 0.0f, 1.0f);
    factor = factor * factor * factor * factor * factor;
    return(F0 + (max(vec3(1.0f - roughness), F0) - F0) * factor);
}
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
        float fragmentZ = gl_FragCoord.z * 2.0f - 1.0f;
        float depth = 2.0f * frame.cameraNear * frame.cameraFar;
        depth /= frame.cameraFar + frame.cameraNear - fragmentZ * (frame.cameraFar - frame.cameraNear);
        float newDepth = depth - currentHeight * 0.1f * heightIntensity;
        newDepth = (1.0f / newDepth) - (1.0f / frame.cameraNear);
        newDepth /= (1.0f / frame.cameraFar) - (1.0f / frame.cameraNear);
        gl_FragDepth = newDepth;
    }else{
        gl_FragDepth = gl_FragCoord.z;
    }
    vec4 diffuseColor = defaultDiffuseColor;
    if(useDiffuseMap) {
        diffuseColor = texture(diffuseMap, UV);
        if(diffuseColor.a < alphaCutThreshold) {
            discard;
        }
    }
    diffuseColor *= diffuseIntensity;
    if(useFlatMaterial) {
        color = diffuseColor;
        color.a *= opacity;
        if(color.a < alphaCutThreshold) {
            discard;
        }
        return;
    }
    vec3 normal = fragment.normal;
    if(useNormalMap) {
        normal = texture(normalMap, UV).rgb;
        normal = normal * 2.0f - 1.0f;
        normal.xy *= normalIntensity;
        normal = normalize(fragment.TBN * normal);
    }
    float metallic = defaultMetallic;
    if(useMetallicMap) {
        metallic = texture(metallicMap, UV).r * metallicIntensity;
    }
    metallic = max(0.0f, min(metallic, 1.0f));
    metallic = 0.05f + 0.9f * metallic;
    float roughness = defaultRoughness;
    if(useRoughnessMap) {
        roughness = texture(roughnessMap, UV).r * roughnessIntensity;
    }
    roughness = max(0.0f, min(roughness, 1.0f));
    roughness = 0.05f + 0.9f * roughness;
    vec3 N = normal;
    vec3 V = normalize(frame.cameraPosition - fragment.position);
    float maxDotNV = max(dot(N, V), 0.0f);
    vec3 R = reflect(-V, N);
    vec3 F0 = mix(vec3(0.0f), diffuseColor.rgb, metallic);
    vec3 Lo = vec3(0.0f);
    vec3 lightingColor = vec3(0.0f);
    for(int i = 0; i < LIGHTS_LIMIT; i += 1) {
        if(lights[i].type == 0) {
            lightingColor += diffuseColor.rgb * lights[i].color;
        }else if(lights[i].type == 1) {
            vec3 lightVector = normalize(-lights[i].direction);
            float factor = max(dot(normal, lightVector), 0.0f);
            lightingColor += diffuseColor.rgb * lights[i].color * factor;
            vec3 H = normalize(V + lightVector);
            float NDF = distributionGGX(N, H, roughness);
            float G = geometrySmith(N, V, lightVector, roughness);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
            vec3 numerator = NDF * G * F;
            float denominator = 2.0f * maxDotNV * max(dot(N, lightVector), 0.0f) + 0.0001f;
            vec3 specular = numerator / denominator;
            vec3 kS = F;
            vec3 kD = vec3(1.0f) - kS;
            kD *= 1.0f - metallic;
            float NdotL = max(dot(N, lightVector), 0.0f);
            Lo += (kD * diffuseColor.rgb / pi + specular) * lights[i].color * NdotL;
        }else if(lights[i].type == 2) {
            vec3 lightVector = lights[i].position - fragment.position;
            float lightDistance = length(lightVector);
            float attenuationProgress = max((lights[i].range - lightDistance) / lights[i].range, 0.0f);
            float attenuation = pow(attenuationProgress, lights[i].attenuationExponent);
            lightVector = normalize(lightVector);
            float factor = max(dot(normal, lightVector), 0.0f) * attenuation;
            lightingColor += diffuseColor.rgb * lights[i].color * factor;
            vec3 H = normalize(V + lightVector);
            float NDF = distributionGGX(N, H, roughness);
            float G = geometrySmith(N, V, lightVector, roughness);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
            vec3 numerator = NDF * G * F;
            float denominator = 2.0f * maxDotNV * max(dot(N, lightVector), 0.0f) + 0.0001f;
            vec3 specular = numerator / denominator;
            vec3 kS = F;
            vec3 kD = vec3(1.0f) - kS;
            kD *= 1.0f - metallic;
            float NdotL = max(dot(N, lightVector), 0.0f);
            Lo += (kD * diffuseColor.rgb / pi + specular) * lights[i].color * attenuation * NdotL;
        }else if(lights[i].type == 3) {
            vec3 lightVector = lights[i].position - fragment.position;
            float lightDistance = length(lightVector);
            float attenuationProgress = max((lights[i].range - lightDistance) / lights[i].range, 0.0f);
            float attenuation = pow(attenuationProgress, lights[i].attenuationExponent);
            lightVector = normalize(lightVector);
            float theta = dot(lightVector, normalize(-lights[i].direction));
            float epsilon = lights[i].innerAngle - lights[i].outerAngle;
            float intensity = clamp((theta - lights[i].outerAngle) / epsilon, 0.0f, 1.0f);
            float factor = max(dot(normal, lightVector), 0.0f) * attenuation * intensity;
            lightingColor += diffuseColor.rgb * lights[i].color * factor;
            vec3 H = normalize(V + lightVector);
            float NDF = distributionGGX(N, H, roughness);
            float G = geometrySmith(N, V, lightVector, roughness);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
            vec3 numerator = NDF * G * F;
            float denominator = 2.0f * maxDotNV * max(dot(N, lightVector), 0.0f) + 0.0001f;
            vec3 specular = numerator / denominator;
            vec3 kS = F;
            vec3 kD = vec3(1.0f) - kS;
            kD *= 1.0f - metallic;
            float NdotL = max(dot(N, lightVector), 0.0f);
            attenuation *= intensity * intensity * intensity * intensity * intensity;
            Lo += (kD * diffuseColor.rgb / pi + specular) * lights[i].color * attenuation * NdotL;
        }
    }
    vec3 F = fresnelSchlickRoughness(maxDotNV, F0, roughness);
    vec3 kD = (1.0f - F) * (1.0f - metallic);
    float ambientOcclusion = 1.0f;
    if(useAmbientOcclusionMap) {
        ambientOcclusion = texture(ambientOcclusionMap, UV).r;
        float inverseAmbientOcclusion = 1.0f - ambientOcclusion;
        ambientOcclusion = 1.0f - inverseAmbientOcclusion * ambientOcclusionIntensity;
    }
    color = vec4(kD * lightingColor * ambientOcclusion + Lo, 1.0);
    if(useReflectionMap) {
        vec3 reflection = texture(ReflectionMap, HDRIUV(R)).rgb * reflectionIntensity;
        color.rgb += F * metallic * (1.0 - roughness) * ambientOcclusion * reflection;
    }
    color.rgb = color.rgb / (color.rgb + vec3(1.0f));
    vec3 multiplyColor = defaultMultiplyColor.rgb;
    if(useMultiplyMap) {
        multiplyColor = texture(multiplyMap, UV).rgb;
    }
    vec3 inverseMultiplyColor = vec3(1.0f) - multiplyColor;
    multiplyColor = vec3(1.0f) - inverseMultiplyColor * multiplyIntensity;
    color.rgb *= multiplyColor;
    vec3 emissionColor = defaultEmissionColor.rgb;
    if(useEmissionMap) {
        emissionColor = texture(emissionMap, UV).rgb * emissionIntensity;
    }
    color.rgb += emissionColor;
    color.rgb = pow(color.rgb, vec3(1.0f / 1.2f));
    color.a *= opacity;
}
