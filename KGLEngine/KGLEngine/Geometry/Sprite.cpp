// Developed by Kelin Lyu.
#include "Geometry.hpp"
Sprite::Sprite() {
    this->engineInitializeGeometry();
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
}
void Sprite::engineRenderGeometry() {
    if(this->clearDepthBuffer) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    glDisable(GL_CULL_FACE);
    this->shader->engineRenderShader(this);
    this->updated = false;
    this->prepared = false;
}
