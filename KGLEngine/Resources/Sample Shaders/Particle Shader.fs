// Developed by Kelin Lyu.
#version 330 core
const int ANIMATION_KEY_LIMIT = 10;
in fragment_data {
    float progress;
    vec2 UV;
    vec2 spriteSheetAnimationData;
} fragment;
out vec4 color;
struct animation_key {
    vec4 color;
    float progress;
};
uniform animation_key keys[ANIMATION_KEY_LIMIT];
uniform int keyCount;
uniform float time;
uniform bool useTextureMap;
uniform sampler2D textureMap;
uniform vec4 particleColor;
uniform bool hasSpriteSheetAnimation;
uniform int spriteSheetAnimationRows;
uniform int spriteSheetAnimationColumns;
void main() {
    if(useTextureMap) {
        if(hasSpriteSheetAnimation) {
            float data = fragment.spriteSheetAnimationData.x + fragment.progress * fragment.spriteSheetAnimationData.y;
            int frame = int(floor(data));
            if(frame >= (spriteSheetAnimationRows * spriteSheetAnimationColumns)) {
                color = vec4(0.0f);
            }else{
                float x = float(frame % spriteSheetAnimationColumns);
                float y = float(frame / spriteSheetAnimationColumns);
                vec2 UV;
                UV.x = x / spriteSheetAnimationColumns + fragment.UV.x / spriteSheetAnimationColumns;
                UV.y = y / spriteSheetAnimationRows + fragment.UV.y / spriteSheetAnimationRows;
                color = texture(textureMap, UV);
            }
        }else{
            color = texture(textureMap, fragment.UV);
        }
    }else{
        color = vec4(1.0f);
    }
    color *= particleColor;
    if(keyCount == 1) {
        color *= keys[0].color;
    }else if(keyCount > 1) {
        int i = 0;
        while(i < keyCount) {
            if(i == 0 && fragment.progress <= keys[i].progress) {
                color *= keys[i].color;
                break;
            }else if(i == (keyCount - 1) && keys[i].progress <= fragment.progress) {
                color *= keys[i].color;
                break;
            }
            if(i + 1 < keyCount) {
                if(keys[i].progress <= fragment.progress && fragment.progress <= keys[i + 1].progress) {
                    float factor = fragment.progress - keys[i].progress;
                    factor = factor / (keys[i + 1].progress - keys[i].progress);
                    color *= mix(keys[i].color, keys[i + 1].color, factor);
                    break;
                }
            }
            i += 1;
        }
    }
}
