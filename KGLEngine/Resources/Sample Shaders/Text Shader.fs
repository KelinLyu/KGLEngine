// Developed by Kelin Lyu.
#version 330 core
in vec2 UV;
out vec4 color;
uniform float alpha;
uniform vec4 defaultColor;
uniform sampler2D textureMap;
void main() {
    color = defaultColor;
    color.a *= texture(textureMap, UV).r * alpha;
}
