// Developed by Kelin Lyu.
#include "Shader.hpp"
int Shader::currentProgramID = -1;
Shader::Shader() {
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
    string fragmentShaderCode = R""""(
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
)"""";
    this->engineCompileShaders(vertexShaderCode, fragmentShaderCode);
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
        string vertexShaderFile = Engine::main->workingDirectory + shaderFile + ".vs";
        string fragmentShaderFile = Engine::main->workingDirectory + shaderFile + ".fs";
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
             << Engine::main->workingDirectory + shaderFile << "!\n" << endl;
        exit(1);
    }
    this->engineCompileShaders(vertexShaderCode, fragmentShaderCode);
}
Shader::Shader(string vertexShaderCode, string fragmentShaderCode) {
    this->engineCompileShaders(vertexShaderCode, fragmentShaderCode);
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
void Shader::setBool(string name, bool value) {
    this->engineActiveShader();
    if(value) {
        glUniform1i(glGetUniformLocation(this->programID, name.c_str()), 1);
    }else{
        glUniform1i(glGetUniformLocation(this->programID, name.c_str()), 0);
    }
}
void Shader::setInt(string name, int value) {
    this->engineActiveShader();
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setFloat(string name, float value) {
    this->engineActiveShader();
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setVec2(string name, vec2 value) {
    this->engineActiveShader();
    glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(string name, vec3 value) {
    this->engineActiveShader();
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(string name, vec4 value) {
    this->engineActiveShader();
    glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setMat2(string name, mat2 mat) {
    this->engineActiveShader();
    glUniformMatrix2fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(string name, mat3 mat) {
    this->engineActiveShader();
    glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(string name, mat4 mat) {
    this->engineActiveShader();
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setTexture(string name, Texture* texture) {
    this->textures.push_back(texture);
    this->engineActiveShader();
    int i = (int)this->textures.size() - 1;
    this->setInt(name, i);
}
Shader::~Shader() {
    glDeleteProgram(this->programID);
    this->textures.clear();
}
void Shader::engineCompileShaders(string vertexShaderCode, string fragmentShaderCode) {
    this->blendMode = 0;
    const char* vertexShader = vertexShaderCode.c_str();
    const char* fragmentShader = fragmentShaderCode.c_str();
    int result = 0;
    // compile the vertex shader:
    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    result += this->engineCheckCompileErrors(vertex, "VERTEX");
    // compile the fragment shader:
    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    result += this->engineCheckCompileErrors(fragment, "FRAGMENT");
    // create the shader Program:
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);
    result += this->engineCheckCompileErrors(this->programID, "PROGRAM");
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
        this->engineCompileShaders(newVertexShaderCode, newFragmentShaderCode);
    }
}
bool Shader::engineCheckCompileErrors(unsigned int shader, string type) {
    int result = 0;
    int success;
    char info[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info);
            cout << "\nShader compilation error!\n" << "Type: " << type << "\n" << info << endl;
            result += 1;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info);
            cout << "\nProgram linking error!\n" << "Type: " << type << "\n" << info << endl;
            result += 1;
        }
    }
    return(result);
}
void Shader::engineActiveShader() {
    if(Shader::currentProgramID != this->programID) {
        Shader::currentProgramID = this->programID;
        glUseProgram(this->programID);
    }
}
void Shader::engineRenderShader(Geometry* geometry) {
    if(this->blendMode == 0) {
        glDisable(GL_BLEND);
    }else if(this->blendMode == 1) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }else if(this->blendMode == 2) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    this->setFloat("frame.time", Engine::main->getTime());
    this->setFloat("frame.random", linearRand(0.0f, 1.0f));
    this->setVec3("frame.cameraPosition", Engine::main->camera->getWorldPosition());
    this->setVec3("frame.cameraDirection", Engine::main->camera->getFrontVectorInWorld());
    mat4 modelTransform = geometry->engineGetGeometryModelTransform();
    mat4 viewTransform = Engine::main->camera->getViewTransform();
    mat4 projectionTransform = Engine::main->camera->getProjectionTransform();
    this->setMat4("node.modelTransform", modelTransform);
    this->setMat4("node.normalTransform", transpose(inverse(modelTransform)));
    this->setMat4("node.modelViewProjectionTransform", projectionTransform * viewTransform * modelTransform);
    if(geometry->engineCheckIfGeometryHasBones()) {
        this->setInt("hasBones", 1);
        vector<mat4>& boneTransforms = geometry->engineGetGeometryBoneTransformsReference();
        for(unsigned int i = 0; i < BONES_LIMIT; i += 1) {
            this->setMat4("boneTransforms[" + to_string(i) + "]", boneTransforms[i]);
        }
    }else{
        this->setInt("hasBones", 0);
    }
    unsigned int count = 0;
    while(count < LIGHTS_LIMIT && count < Engine::main->preparedLightNodes.size()) {
        if((geometry->lightMask & Engine::main->preparedLightNodes[count]->lightMask) > 0) {
            Engine::main->preparedLightNodes[count]->engineConfigurateShader(this, count);
        }
        count += 1;
    }
    this->setInt("lightCount", count);
    for(unsigned int i = 0; i < this->textures.size(); i += 1) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->data);
    }
}