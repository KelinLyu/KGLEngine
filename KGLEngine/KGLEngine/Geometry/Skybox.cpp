// Developed by Kelin Lyu.
#include "Geometry.hpp"
Skybox::Skybox(string right, string left,
               string top, string bottom,
               string front, string back,
               float maxAnisotropy) {
    this->engineInitializeGeometry();
    float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glBindVertexArray(this->vertexArrays);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    this->texture = new Texture(right, left, top, bottom, front, back, maxAnisotropy);
    string vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec3 vertexPosition;
out vec3 cubeUV;
struct frame_data {
    mat4 viewProjectionTransform;
};
uniform frame_data frame;
void main() {
    vec4 destination = frame.viewProjectionTransform * vec4(vertexPosition, 1.0);
    gl_Position = destination.xyww;
    cubeUV = vertexPosition;
}
)"""";
    string fragmentShaderCode = R""""(
#version 330 core
in vec3 cubeUV;
out vec4 color;
uniform samplerCube skybox;
void main() {
    color = texture(skybox, cubeUV);
}
)"""";
    this->shader = new Shader(vertexShaderCode, fragmentShaderCode);
    this->shader->setInt("skybox", 0);
}
Skybox::~Skybox() {
    glDeleteVertexArrays(1, &this->vertexArrays);
    glDeleteBuffers(1, &this->vertexBuffers);
    delete(this->texture);
    delete(this->shader);
}
void Skybox::engineRenderGeometry() {
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    mat4 viewTransform = Engine::main->mainCameraNode->getViewTransform();
    viewTransform = mat4(mat3(viewTransform));
    mat4 projectionTransform = Engine::main->mainCameraNode->getProjectionTransform();
    this->shader->setMat4("frame.viewProjectionTransform", projectionTransform * viewTransform);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture->data);
    glBindVertexArray(this->vertexArrays);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    this->updated = false;
    this->prepared = false;
}
