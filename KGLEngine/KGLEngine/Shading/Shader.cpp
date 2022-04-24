// Developed by Kelin Lyu.
#include "Shader.hpp"
int Shader::currentProgramID = -1;
int Shader::currentBlendMode = -1;
int Shader::currentWriteToDepthBuffer = -1;
int Shader::currentDepthFunction = -1;
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
    this->engineInitializeShader(vertexShaderCode, fragmentShaderCode);
}
Shader::Shader(string shaderFile) {
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
        cout << "\nFailed to load the shaders files: "
             << Engine::main->workingDirectory + shaderFile << "!\n" << endl;
        exit(1);
    }
    this->engineInitializeShader(vertexShaderCode, fragmentShaderCode);
}
Shader::Shader(string vertexShaderCode, string fragmentShaderCode) {
    this->engineInitializeShader(vertexShaderCode, fragmentShaderCode);
}
Shader* Shader::copy() {
    Shader* shader = new Shader(this->vertexShaderSourceCode, this->fragmentShaderSourceCode);
    return(shader);
}
void Shader::activateShader() {
    if(Shader::currentProgramID != this->programID) {
        Shader::currentProgramID = this->programID;
        glUseProgram(this->programID);
    }
}
void Shader::setUIShader() {
    this->isUIShader = true;
    this->blendMode = 2;
    this->writeToDepthBuffer = false;
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
    this->activateShader();
    if(value) {
        glUniform1i(glGetUniformLocation(this->programID, name.c_str()), 1);
    }else{
        glUniform1i(glGetUniformLocation(this->programID, name.c_str()), 0);
    }
}
void Shader::setInt(string name, int value) {
    this->activateShader();
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setFloat(string name, float value) {
    this->activateShader();
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}
void Shader::setVec2(string name, vec2 value) {
    this->activateShader();
    glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(string name, vec3 value) {
    this->activateShader();
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(string name, vec4 value) {
    this->activateShader();
    glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}
void Shader::setMat2(string name, mat2 mat) {
    this->activateShader();
    glUniformMatrix2fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(string name, mat3 mat) {
    this->activateShader();
    glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(string name, mat4 mat) {
    this->activateShader();
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setTexture(string name, Texture* texture) {
    for(unsigned int i = 0; i < this->uniformTextureNames.size(); i += 1) {
        if(this->uniformTextureNames[i] == name) {
            this->textures[i] = texture;
            return;
        }
    }
    this->textures.push_back(texture);
    this->uniformTextureNames.push_back(name);
    this->activateShader();
    int i = (int)this->textures.size() - 1;
    this->setInt(name, i);
}
Shader::~Shader() {
    glDeleteProgram(this->programID);
    this->textures.clear();
}
void Shader::engineInitializeShader(string vertexShaderCode, string fragmentShaderCode) {
    this->blendMode = 0;
    this->vertexShaderSourceCode = vertexShaderCode;
    this->fragmentShaderSourceCode = fragmentShaderCode;
    this->currentModelTransform = mat4(-1.0f);
    this->isUIShader = false;
    this->isParticleShader = false;
    this->writeToDepthBuffer = true;
    this->clearDepthBuffer = false;
    const char* vertexShaderString = vertexShaderCode.c_str();
    const char* fragmentShaderString = fragmentShaderCode.c_str();
    int result = 0;
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);
    result += this->engineCheckCompileErrors(vertexShader, "VERTEX");
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);
    result += this->engineCheckCompileErrors(fragmentShader, "FRAGMENT");
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);
    result += this->engineCheckCompileErrors(this->programID, "PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
        this->engineInitializeShader(newVertexShaderCode, newFragmentShaderCode);
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
void Shader::engineRenderShader(Geometry* geometry, bool shadowMap) {
    if(this->clearDepthBuffer) {
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    if(Shader::currentBlendMode != this->blendMode) {
        Shader::currentBlendMode = this->blendMode;
        if(this->blendMode == 0) {
            glDisable(GL_BLEND);
        }else if(this->blendMode == 1) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }else if(this->blendMode == 2) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    if(Shader::currentWriteToDepthBuffer != this->writeToDepthBuffer) {
        Shader::currentWriteToDepthBuffer = this->writeToDepthBuffer;
        if(this->writeToDepthBuffer) {
            glDepthMask(GL_TRUE);
        }else{
            glDepthMask(GL_FALSE);
        }
    }
    this->activateShader();
    mat4 modelTransform = geometry->engineGetGeometryModelTransform();
    this->setBool("renderingShadow", shadowMap);
    if(shadowMap) {
        this->setMat4("lightSpaceMatrix", Engine::main->mainCameraNode->getDirectionalLightSpaceMatrix());
    }
    if(this->isUIShader) {
        if(Shader::currentDepthFunction != 1) {
            Shader::currentDepthFunction = 1;
            glDepthFunc(GL_ALWAYS);
        }
        mat4 projectionTransform = Engine::main->mainCameraNode->getOrthogonalProjectionTransform();
        this->setMat4("projectionTransform", projectionTransform);
        this->setMat4("modelProjectionTransform", projectionTransform * modelTransform);
    }else if(this->isParticleShader) {
        if(Shader::currentDepthFunction != 0) {
            Shader::currentDepthFunction = 0;
            glDepthFunc(GL_LESS);
        }
        this->setFloat("time", Engine::main->getTime());
        mat4 viewTransform = Engine::main->mainCameraNode->getViewTransform();
        mat4 projectionTransform = Engine::main->mainCameraNode->getProjectionTransform();
        this->setMat4("projectionTransform", projectionTransform);
        this->setMat4("viewTransform", viewTransform);
        this->setMat4("modelViewTransform", viewTransform * modelTransform);
    }else{
        if(Shader::currentDepthFunction != 0) {
            Shader::currentDepthFunction = 0;
            glDepthFunc(GL_LESS);
        }
        mat4 viewTransform = Engine::main->mainCameraNode->getViewTransform();
        mat4 projectionTransform = Engine::main->mainCameraNode->getProjectionTransform();
        mat4 viewProjectionTransform = projectionTransform * viewTransform;
        this->setMat4("frame.viewProjectionTransform", viewProjectionTransform);
        this->setVec3("frame.cameraPosition", Engine::main->mainCameraNode->getWorldPosition());
        this->setVec3("frame.cameraDirection", Engine::main->mainCameraNode->getFrontVectorInWorld());
        if(this->currentModelTransform != modelTransform) {
            this->currentModelTransform = modelTransform;
            this->setMat4("node.modelTransform", modelTransform);
            this->setMat4("node.normalTransform", transpose(inverse(modelTransform)));
        }
        this->setMat4("node.modelViewProjectionTransform", viewProjectionTransform * modelTransform);
        if(geometry->engineCheckWhetherGeometryHasBones() && geometry->engineCheckWhetherGeometryHasUpdatedSkeletalAnimations()) {
            this->setBool("hasBones", true);
            vector<mat4>* boneTransforms = geometry->engineGetGeometryBoneTransforms();
            for(unsigned int i = 0; i < BONES_LIMIT; i += 1) {
                this->setMat4("boneTransforms[" + to_string(i) + "]", (*boneTransforms)[i]);
            }
        }else{
            this->setBool("hasBones", false);
        }
        this->setBool("useShadowMap", false);
        unsigned int i = 0;
        unsigned int count = 0;
        while(count < LIGHTS_LIMIT && i < Engine::main->preparedLightNodes.size()) {
            if(geometry->engineCheckWhetherGeometryIsAffectedByLightNode(Engine::main->preparedLightNodes[i])) {
                Engine::main->preparedLightNodes[i]->engineConfigurateShader(this, count);
                count += 1;
            }
            i += 1;
        }
        this->setInt("lightCount", count);
        geometry->affectedLightCount = count;
    }
    for(unsigned int i = 0; i < this->textures.size(); i += 1) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->data);
    }
    unsigned int amount = (unsigned int)geometry->engineGetGeometryInstanceCount();
    if(amount > 0) {
        this->setBool("enableInstancing", true);
        if(this->isUIShader) {
            glBindVertexArray(geometry->engineGetGeometryVertexArrays());
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, amount);
            glBindVertexArray(0);
        }else{
            glBindVertexArray(geometry->engineGetGeometryVertexArrays());
            glDrawElementsInstanced(GL_TRIANGLES, geometry->engineGetGeometryIndiceCount(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
    }else{
        this->setBool("enableInstancing", false);
        if(this->isUIShader) {
            glBindVertexArray(geometry->engineGetGeometryVertexArrays());
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }else{
            glBindVertexArray(geometry->engineGetGeometryVertexArrays());
            glDrawElements(GL_TRIANGLES, geometry->engineGetGeometryIndiceCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}
