// Developed by Kelin.Lyu.
#include "material.hpp"
Material::Material(float metallic, float roughness) {
    string vertexShaderCode = R""""(
#version 330 core
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
uniform frame_data frame;
uniform node_data node;
uniform bool hasBones;
const int MAX_BONE_INFLUENCE = 4;
const int BONES_LIMIT = 120;
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
)"""";
    string fragmentShaderCode = R""""(
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
const float pi = 3.1416f;
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
uniform bool reverseMetallic;
uniform float defaultRoughness;
uniform bool useRoughnessMap;
uniform sampler2D roughnessMap;
uniform float roughnessIntensity;
uniform bool reverseRoughness;
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
        if(reverseMetallic) {
            metallic = 1.0f - metallic;
        }
        metallic *= metallicIntensity;
    }
    metallic = max(0.0f, min(metallic, 1.0f));
    metallic = 0.2f + 0.6f * metallic;
    float roughness = defaultRoughness;
    if(useRoughnessMap) {
        roughness = texture(roughnessMap, UV).r;
        if(reverseRoughness) {
            roughness = 1.0f - roughness;
        }
        roughness *= roughnessIntensity;
    }
    roughness = max(0.0f, min(roughness, 1.0f));
    roughness = 0.2f + 0.6f * roughness;
    vec3 viewVector = normalize(frame.cameraPosition - fragment.position);
    vec3 F0 = mix(vec3(0.1f), diffuseColor.rgb, metallic);
    vec3 lightingColor = vec3(0.0f);
    for(int i = 0; i < LIGHTS_LIMIT; i += 1) {
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
            float theta = dot(lightVector, normalize(-lights[i].direction));
            float epsilon = lights[i].innerAngle - lights[i].outerAngle;
            attenuation *= clamp((theta - lights[i].outerAngle) / epsilon, 0.0f, 1.0f);
            lightFactor = max(dot(normal, lightVector), 0.0f) * attenuation;
        }
        vec3 halfwayVector = normalize(viewVector + lightVector);
        float roughness4 = roughness * roughness * roughness * roughness;
        float NdotH = max(dot(normal, halfwayVector), 0.0f);
        float NdotH2 = NdotH * NdotH;
        float denominator = (NdotH2 * (roughness4 - 1.0f) + 1.0f);
        denominator = pi * denominator * denominator;
        float NDF = roughness4 / denominator;
        float factor = clamp(1.0f - max(dot(halfwayVector, viewVector), 0.0f), 0.0f, 1.0f);
        factor = factor * factor * factor * factor * factor;
        vec3 F = (F0 + (1.0 - F0) * factor);
        lightingColor += ((vec3(1.0f) - F) * diffuseColor.rgb / pi) * lightFactor * lights[i].colorFactor;
        lightingColor += NDF * diffuseColor.rgb * lightFactor * lights[i].highlightFactor;
    }
    float ambientOcclusion = 1.0f;
    if(useAmbientOcclusionMap) {
        ambientOcclusion = texture(ambientOcclusionMap, UV).r;
        float inverseAmbientOcclusion = 1.0f - ambientOcclusion;
        ambientOcclusion = 1.0f - inverseAmbientOcclusion * ambientOcclusionIntensity;
    }
    color = vec4(lightingColor * ambientOcclusion, 1.0);
    if(useReflectionMap) {
        vec3 reflectionVector = reflect(-viewVector, normal);
        vec2 uv = vec2(atan(reflectionVector.z, reflectionVector.x), -asin(reflectionVector.y));
        uv = uv * vec2(0.1592f, 0.3183f) + 0.5f;
        vec3 reflection = texture(ReflectionMap, uv).rgb * reflectionIntensity;
        color.rgb += reflection * metallic * (1.0 - roughness) * ambientOcclusion;
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
    color.a *= diffuseColor.a * opacity;
}
)"""";
    this->shader = new Shader(vertexShaderCode, fragmentShaderCode);
    this->opacity = 1.0f;
    this->diffuseColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    this->diffuseIntensity = 1.0f;
    this->alphaCutThreshold = 0.5f;
    this->normalIntensity = 1.0f;
    this->heightIntensity = 1.0f;
    this->heightLayerRange = vec2(4.0f, 16.0f);
    this->metallic = metallic;
    this->metallicIntensity = 1.0f;
    this->reverseMetallic = false;
    this->roughness = roughness;
    this->roughnessIntensity = 1.0f;
    this->reverseRoughness = false;
    this->reflectionIntensity = 1.0f;
    this->ambientOcclusionIntensity = 1.0f;
    this->multiplyColor = vec4(1.0f);
    this->multiplyIntensity = 1.0f;
    this->emissionColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->emissionIntensity = 1.0f;
}
void Material::setOpaque() {
    this->shader->setOpaque();
}
void Material::setAdditive() {
    this->shader->setAdditive();
}
void Material::setSemitransparent() {
    this->shader->setSemitransparent();
}
void Material::setDiffuseMap(Texture* texture) {
    this->shader->setInt("useDiffuseMap", 1);
    this->shader->addTexture(texture, "diffuseMap");
}
void Material::setNormalMap(Texture* texture) {
    this->shader->setInt("useNormalMap", 1);
    this->shader->addTexture(texture, "normalMap");
}
void Material::setHeightMap(Texture* texture) {
    this->shader->setInt("useHeightMap", 1);
    this->shader->addTexture(texture, "heightMap");
}
void Material::setMetallicMap(Texture* texture) {
    this->shader->setInt("useMetallicMap", 1);
    this->shader->addTexture(texture, "metallicMap");
}
void Material::setRoughnessMap(Texture* texture) {
    this->shader->setInt("useRoughnessMap", 1);
    this->shader->addTexture(texture, "roughnessMap");
}
void Material::setReflectionMap(Texture* texture) {
    this->shader->setInt("useReflectionMap", 1);
    this->shader->addTexture(texture, "ReflectionMap");
}
void Material::setAmbientOcclusionMap(Texture* texture) {
    this->shader->setInt("useAmbientOcclusionMap", 1);
    this->shader->addTexture(texture, "ambientOcclusionMap");
}
void Material::setMultiplyMap(Texture* texture) {
    this->shader->setInt("useMultiplyMap", 1);
    this->shader->addTexture(texture, "multiplyMap");
}
void Material::setEmissionMap(Texture* texture) {
    this->shader->setInt("useEmissionMap", 1);
    this->shader->addTexture(texture, "emissionMap");
}
void Material::render() {
    this->shader->setFloat("opacity", this->opacity);
    this->shader->setVec4("defaultDiffuseColor", this->diffuseColor);
    this->shader->setFloat("diffuseIntensity", this->diffuseIntensity);
    this->shader->setFloat("alphaCutThreshold", this->alphaCutThreshold);
    this->shader->setFloat("normalIntensity", this->normalIntensity);
    this->shader->setFloat("heightIntensity", this->heightIntensity);
    this->shader->setVec2("heightLayerRange", this->heightLayerRange);
    this->shader->setFloat("defaultMetallic", this->metallic);
    this->shader->setFloat("metallicIntensity", this->metallicIntensity);
    if(this->reverseMetallic) {
        this->shader->setInt("reverseMetallic", 1);
    }else{
        this->shader->setInt("reverseMetallic", 0);
    }
    this->shader->setFloat("defaultRoughness", this->roughness);
    this->shader->setFloat("roughnessIntensity", this->roughnessIntensity);
    if(this->reverseRoughness) {
        this->shader->setInt("reverseRoughness", 1);
    }else{
        this->shader->setInt("reverseRoughness", 0);
    }
    this->shader->setFloat("reflectionIntensity", this->reflectionIntensity);
    this->shader->setFloat("ambientOcclusionIntensity", this->ambientOcclusionIntensity);
    this->shader->setVec4("defaultMultiplyColor", this->multiplyColor);
    this->shader->setFloat("multiplyIntensity", this->multiplyIntensity);
    this->shader->setVec4("defaultEmissionColor", this->emissionColor);
    this->shader->setFloat("emissionIntensity", this->emissionIntensity);
}
Material::~Material() {
    delete(this->shader);
}
