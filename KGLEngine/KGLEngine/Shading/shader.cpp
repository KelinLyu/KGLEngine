// Developed by Kelin.Lyu.
#include "shader.hpp"
void Shader::compileShaders(string vertexShaderCode, string fragmentShaderCode) {
    this->blendMode = 0;
    const char* vertexShader = vertexShaderCode.c_str();
    const char* fragmentShader = fragmentShaderCode.c_str();
    int result = 0;
    // compile the vertex shader:
    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    result += this->checkCompileErrors(vertex, "VERTEX");
    // compile the fragment shader:
    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    result += this->checkCompileErrors(fragment, "FRAGMENT");
    // create the shader Program:
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);
    result += this->checkCompileErrors(this->programID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(result > 0) {
        string newVertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec3 vertexPosition;
struct node_data {
    mat4 modelViewProjectionTransform;
};
uniform node_data node;
void main() {
    gl_Position = node.modelViewProjectionTransform * vec4(vertexPosition, 1.0f);
}
)"""";
        string newFragmentShaderCode = R""""(
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
)"""";
        this->compileShaders(newVertexShaderCode, newFragmentShaderCode);
    }
}
Shader::Shader(string vertexShaderCode, string fragmentShaderCode) {
    this->compileShaders(vertexShaderCode, fragmentShaderCode);
}
Shader::Shader(string shaderFile) {
    // read the files:
    string vertexShaderCode;
    string fragmentShaderCode;
    ifstream vertexShaderStream;
    ifstream fragmentShaderStream;
    vertexShaderStream.exceptions(ifstream::failbit | ifstream::badbit);
    fragmentShaderStream.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        string vertexShaderFile = Engine::main->getProgramDirectory() + shaderFile + ".vs";
        string fragmentShaderFile = Engine::main->getProgramDirectory() + shaderFile + ".fs";
        vertexShaderStream.open(vertexShaderFile.c_str());
        fragmentShaderStream.open(fragmentShaderFile.c_str());
        stringstream vertexShader, fragmentShader;
        vertexShader << vertexShaderStream.rdbuf();
        fragmentShader << fragmentShaderStream.rdbuf();
        vertexShaderStream.close();
        fragmentShaderStream.close();
        vertexShaderCode = vertexShader.str();
        fragmentShaderCode = fragmentShader.str();
    }catch(ifstream::failure error) {
        cout << "\nFailed to load the shaders: "
             << Engine::main->getProgramDirectory() + shaderFile << "!\n" << endl;
        exit(1);
    }
    this->compileShaders(vertexShaderCode, fragmentShaderCode);
}
Shader::Shader(Geometry* geometryWithMissingShader) {
    string vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec3 vertexPosition;
struct node_data {
    mat4 modelViewProjectionTransform;
};
uniform node_data node;
void main() {
    gl_Position = node.modelViewProjectionTransform * vec4(vertexPosition, 1.0f);
}
)"""";
    if(geometryWithMissingShader->hasBones()) {
        vertexShaderCode = R""""(
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout(location = 5) in ivec4 boneIDs;
layout(location = 6) in vec4 weights;
struct node_data {
    mat4 modelViewProjectionTransform;
};
uniform node_data node;
const int MAX_BONE_INFLUENCE = 4;
const int BONES_LIMIT = 120;
uniform mat4 boneTransforms[BONES_LIMIT];
void main() {
    vec4 position = vec4(0.0f);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i += 1) {
        if(boneIDs[i] == -1) {
            continue;
        }
        if(boneIDs[i] >= BONES_LIMIT) {
            position = vec4(vertexPosition, 1.0f);
            break;
        }
        vec4 localPosition = boneTransforms[boneIDs[i]] * vec4(vertexPosition, 1.0f);
        position += localPosition * weights[i];
    }
    gl_Position = node.modelViewProjectionTransform * position;
}
)"""";
    }
    string fragmentShaderCode = R""""(
#version 330 core
out vec4 color;
struct frame_data {
    float time;
};
uniform frame_data frame;
void main() {
    int factor = int(frame.time * 2.0f);
    if(factor % 3 == 0) {
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }else if(factor % 3 == 1) {
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }else{
        color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }
}
)"""";
    this->compileShaders(vertexShaderCode, fragmentShaderCode);
}
void Shader::addTexture(Texture* texture, string uniformName) {
    this->textures.push_back(texture);
    this->uniformNames.push_back(uniformName);
    this->setActivate();
    int i = (int)this->textures.size() - 1;
    this->setInt(this->uniformNames[i], i);
}
void Shader::render(mat4 modelTransform) {
    if(this->blendMode == 0) {
        glDisable(GL_BLEND);
    }else if(this->blendMode == 1) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }else if(this->blendMode == 2) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    mat4 viewTransform = Engine::main->camera->getViewTransform();
    mat4 projectionTransform = Engine::main->camera->getProjectionTransform();
    float time = Engine::main->getTime();
    this->setMat4("frame.viewTransform", viewTransform);
    this->setMat4("frame.inverseViewTransform", inverse(viewTransform));
    this->setMat4("frame.projectionTransform", projectionTransform);
    this->setMat4("frame.inverseProjectionTransform", inverse(projectionTransform));
    this->setMat4("frame.viewProjectionTransform", projectionTransform * viewTransform);
    this->setMat4("frame.inverseViewProjectionTransform", inverse(projectionTransform * viewTransform));
    this->setFloat("frame.time", time);
    this->setFloat("frame.random", linearRand(0.0f, 1.0f));
    this->setVec3("frame.cameraPosition", Engine::main->camera->getWorldPosition());
    this->setVec3("frame.cameraDirection", Engine::main->camera->getFrontVectorInWorld());
    this->setMat4("frame.cameraTransform", Engine::main->camera->getWorldTransform());
    this->setFloat("frame.cameraNear", Engine::main->camera->near);
    this->setFloat("frame.cameraFar", Engine::main->camera->far);
    this->setMat4("node.modelTransform", modelTransform);
    this->setMat4("node.inverseModelTransform", inverse(modelTransform));
    this->setMat4("node.normalTransform", transpose(inverse(modelTransform)));
    this->setMat4("node.modelViewTransform", viewTransform * modelTransform);
    this->setMat4("node.inverseModelViewTransform", inverse(viewTransform * modelTransform));
    this->setMat4("node.modelViewProjectionTransform", projectionTransform * viewTransform * modelTransform);
    this->setMat4("node.inverseModelViewProjectionTransform", inverse(projectionTransform * viewTransform * modelTransform));
    for(unsigned int i = 0; i < this->textures.size(); i += 1) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->data);
    }
}
void Shader::setOpaque() {
    this->blendMode = 0;
}
void Shader::setAdditive() {
    this->blendMode = 1;
}
void Shader::setSemitransparent() {
    this->blendMode = 2;
}
void Shader::setActivate() const {
    if(Engine::main->currentShaderProgramID != this->programID) {
        Engine::main->currentShaderProgramID = this->programID;
        glUseProgram(this->programID);
    }
}
void Shader::setInt(const string &name, int value) const {
    this->setActivate();
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value) const {
    this->setActivate();
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, const vec2 &value) const {
    this->setActivate();
    glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, const vec3 &value) const {
    this->setActivate();
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, const vec4 &value) const {
    this->setActivate();
    glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setMat2(const std::string &name, const mat2 &mat) const {
    this->setActivate();
    glUniformMatrix2fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const mat3 &mat) const {
    this->setActivate();
    glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const mat4 &mat) const {
    this->setActivate();
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
bool Shader::checkCompileErrors(unsigned int shader, string type) {
    int result = 0;
    int success;
    char info[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info);
            cout << "\nShader compilation error!\n"
                 << "Type: " << type << "\n" << info << endl;
            result += 1;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info);
            cout << "\nProgram linking error!\n"
                 << "Type: " << type << "\n" << info << endl;
            result += 1;
        }
    }
    return(result);
}
Shader::~Shader() {
    glDeleteProgram(this->programID);
    this->textures.clear();
    this->uniformNames.clear();
}
