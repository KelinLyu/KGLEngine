// Developed by Kelin.Lyu.
#include "shader.hpp"
Shader::Shader(string shaderFile) {
    this->cullMode = 0;
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
    const char* vertexShader = vertexShaderCode.c_str();
    const char* fragmentShader = fragmentShaderCode.c_str();
    // compile the vertex shader:
    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // compile the fragment shader:
    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // create the shader Program:
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);
    checkCompileErrors(this->programID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Shader::Shader(Geometry* geometryWithMissingShader) {
    this->cullMode = 0;
    string vertexShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec3 vertexPosition;\n"
    "struct node_data {\n"
    "    mat4 modelViewProjectionTransform;\n"
    "};\n"
    "uniform node_data node;\n"
    "void main() {\n"
    "    gl_Position = node.modelViewProjectionTransform * vec4(vertexPosition, 1.0);\n"
    "}\0";
    if(geometryWithMissingShader->hasBones()) {
        vertexShaderCode = "#version 330 core\n"
        "layout (location = 0) in vec3 vertexPosition;\n"
        "layout(location = 5) in ivec4 boneIDs;\n"
        "layout(location = 6) in vec4 weights;\n"
        "struct node_data {\n"
        "    mat4 modelViewProjectionTransform;\n"
        "};\n"
        "uniform node_data node;\n"
        "const int MAX_BONE_INFLUENCE = 4;\n"
        "const int BONES_LIMIT = 100;\n"
        "uniform mat4 boneTransforms[BONES_LIMIT];\n"
        "void main() {\n"
        "    vec4 position = vec4(0.0f);\n"
        "    for(int i = 0; i < MAX_BONE_INFLUENCE; i += 1) {\n"
        "        if(boneIDs[i] == -1) {\n"
        "            continue;\n"
        "        }\n"
        "        if(boneIDs[i] >= BONES_LIMIT) {\n"
        "            position = vec4(vertexPosition, 1.0f);\n"
        "            break;\n"
        "        }\n"
        "        vec4 localPosition = boneTransforms[boneIDs[i]] * vec4(vertexPosition, 1.0f);\n"
        "        position += localPosition * weights[i];\n"
        "    }\n"
        "    gl_Position = node.modelViewProjectionTransform * position;\n"
        "}\0";
    }
    string fragmentShaderCode = "#version 330 core\n"
    "out vec4 color;\n"
    "struct frame_data {\n"
    "    float time;\n"
    "};\n"
    "uniform frame_data frame;\n"
    "void main() {\n"
    "    int factor = int(frame.time * 2.0f);"
    "    if(factor % 3 == 0) {\n"
    "        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "    }else if(factor % 3 == 1) {\n"
    "        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "    }else{\n"
    "        color = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
    "    }\n"
    "}\n\0";
    const char* vertexShader = vertexShaderCode.c_str();
    const char* fragmentShader = fragmentShaderCode.c_str();
    // compile the vertex shader:
    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // compile the fragment shader:
    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // create the shader Program:
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);
    checkCompileErrors(this->programID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::addTexture(Texture* texture, string uniformName) {
    this->textures.push_back(texture);
    this->uniformNames.push_back(uniformName);
}
void Shader::render(mat4 modelTransform) {
    if(this->cullMode == 0) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }else if(this->cullMode == 1) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }else{
        glDisable(GL_CULL_FACE);
    }
    mat4 viewTransform = Engine::main->cameraNode->getViewTransform();
    mat4 projectionTransform = Engine::main->cameraNode->getProjectionTransform();
    float time = Engine::main->getTime();
    this->setMat4("frame.viewTransform", viewTransform);
    this->setMat4("frame.inverseViewTransform", inverse(viewTransform));
    this->setMat4("frame.projectionTransform", projectionTransform);
    this->setMat4("frame.inverseProjectionTransform", inverse(projectionTransform));
    this->setMat4("frame.viewProjectionTransform", projectionTransform * viewTransform);
    this->setMat4("frame.inverseViewProjectionTransform", inverse(projectionTransform * viewTransform));
    this->setFloat("frame.time", time);
    this->setFloat("frame.random", linearRand(0.0f, 1.0f));
    this->setMat4("node.modelTransform", modelTransform);
    this->setMat4("node.inverseModelTransform", inverse(modelTransform));
    this->setMat4("node.normalTransform", transpose(inverse(modelTransform)));
    this->setMat4("node.modelViewTransform", viewTransform * modelTransform);
    this->setMat4("node.inverseModelViewTransform", inverse(viewTransform * modelTransform));
    this->setMat4("node.modelViewProjectionTransform", projectionTransform * viewTransform * modelTransform);
    this->setMat4("node.inverseModelViewProjectionTransform", inverse(projectionTransform * viewTransform * modelTransform));
    for(unsigned int i = 0; i < this->textures.size(); i += 1) {
        glActiveTexture(GL_TEXTURE0 + i);
        this->setInt(this->uniformNames[i], i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->data);
    }
}
void Shader::cullBack() {
    this->cullMode = 0;
}
void Shader::cullFront() {
    this->cullMode = 1;
}
void Shader::doubleSided() {
    this->cullMode = 2;
}
void Shader::setActivate() {
    glUseProgram(this->programID);
}
void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, const vec2 &value) const {
    glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, const vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, const vec4 &value) const {
    glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setMat2(const std::string &name, const mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::checkCompileErrors(unsigned int shader, string type) {
    int success;
    char info[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info);
            cout << "\nShader compilation error!\n"
                 << "Type: " << type << "\n" << info << endl;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info);
            cout << "\nProgram linking error!\n"
                 << "Type: " << type << "\n" << info << endl;
        }
    }
}
Shader::~Shader() {
    glDeleteProgram(this->programID);
    this->textures.clear();
    this->uniformNames.clear();
}
