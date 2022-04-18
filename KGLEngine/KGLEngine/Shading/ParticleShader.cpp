// Developed by Kelin Lyu.
#include "Shader.hpp"
ParticleShader::ParticleShader() {
    
    string shaderFile = "/Resources/Sample Shaders/Particle Shader";
    
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
    this->isParticleShader = true;
    this->blendMode = 2;
    
    
    
    
}







void ParticleShader::engineRenderShader(Geometry* geometry) {
    
    
    
    
    this->Shader::engineRenderShader(geometry);
    
    
    
    
}
