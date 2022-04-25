// Developed by Kelin Lyu.
#include "Geometry.hpp"
TextRenderer::TextRenderer() {
    this->engineInitializeGeometry();
    this->cullMode = 2;
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glBindVertexArray(this->vertexArrays);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->indiceCount = 6;
    string vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec4 vertexData;
out vec2 UV;
uniform mat4 modelProjectionTransform;
void main() {
    UV = vertexData.zw;
    gl_Position = modelProjectionTransform * vec4(vertexData.xy, 0.0f, 1.0f);
}
)"""";
    string fragmentShaderCode = R""""(
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
)"""";
    this->shader = new Shader(vertexShaderCode, fragmentShaderCode);
    this->shader->setUIShader();
}
TextRenderer::~TextRenderer() {
    delete(this->shader);
    this->shader = NULL;
    this->textures.clear();
    this->transforms.clear();
}
void TextRenderer::engineRenderGeometry(unsigned int renderingMode) {
    for(unsigned int i = 0; i < this->transforms.size(); i += 1) {
        this->modelTransform = this->mainTransform * this->transforms[i];
        this->shader->setTexture("textureMap", this->textures[i]);
        this->shader->engineRenderShader(this, renderingMode);
    }
    this->updated = false;
    this->prepared = false;
}
void TextRenderer::engineSetTextRendererAlpha(float alpha) {
    this->shader->setFloat("alpha", alpha);
}
void TextRenderer::engineSetTextRendererColor(vec4 color) {
    this->shader->setVec4("defaultColor", color);
}
void TextRenderer::engineSetTextRendererTexturesAndTransforms(vector<Texture*> textures, vector<mat4> transforms) {
    this->textures = textures;
    this->transforms = transforms;
}
void TextRenderer::engineClearTextRendererTexturesAndTransforms() {
    this->textures.clear();
    this->transforms.clear();
}
void TextRenderer::engineSetTextRendererMainTransform(mat4 mainTransform) {
    this->mainTransform = mainTransform;
}
