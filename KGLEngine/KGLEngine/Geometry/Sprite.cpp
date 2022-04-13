// Developed by Kelin Lyu.
#include "Geometry.hpp"
Sprite::Sprite() {
    this->engineInitializeGeometry();
    
    float vertices[] = {
        // pos      // tex
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
void Sprite::render() {
    
    /*
    
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    */
    
    
}
