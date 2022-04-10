// Developed by Kelin.Lyu.
#include "skybox.hpp"
Skybox::Skybox(string right, string left,
               string top, string bottom,
               string front, string back,
               float maxAnisotropy) {
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
    string vertexShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec3 vertexPosition;\n"
    "out vec3 cubeUV;\n"
    "struct frame_data {\n"
    "    mat4 viewProjectionTransform;\n"
    "};\n"
    "uniform frame_data frame;\n"
    "void main() {\n"
    "    vec4 destination = frame.viewProjectionTransform * vec4(vertexPosition, 1.0);\n"
    "    gl_Position = destination.xyww;\n"
    "    cubeUV = vertexPosition;\n"
    "}\n";
    string fragmentShaderCode = "#version 330 core\n"
    "in vec3 cubeUV;\n"
    "out vec4 color;\n"
    "uniform samplerCube skybox;\n"
    "void main() {\n"
    "    color = texture(skybox, cubeUV);\n"
    "}\n";
    this->shader = new Shader(vertexShaderCode, fragmentShaderCode);
}
void Skybox::render() {
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    mat4 viewTransform = Engine::main->camera->getViewTransform();
    viewTransform = mat4(mat3(viewTransform));
    mat4 projectionTransform = Engine::main->camera->getProjectionTransform();
    this->shader->setMat4("frame.viewProjectionTransform", projectionTransform * viewTransform);
    glActiveTexture(GL_TEXTURE0);
    this->shader->setInt("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture->data);
    glBindVertexArray(this->vertexArrays);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
Skybox::~Skybox() {
    glDeleteVertexArrays(1, &this->vertexArrays);
    glDeleteBuffers(1, &this->vertexBuffers);
    delete(this->texture);
    delete(this->shader);
}
