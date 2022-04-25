// Developed by Kelin Lyu.
#include "Geometry.hpp"
ParticleRenderer::ParticleRenderer(unsigned int amount) {
    this->engineInitializeGeometry();
    this->particleAmount = amount;
    struct ParticleVertex {
        vec3 position;
        vec2 uv;
    };
    vector<ParticleVertex> vertices;
    vector<unsigned int> indices;
    ParticleVertex vertex1 = ParticleVertex();
    vertex1.position = vec3(-0.5f, -0.5f, 0.0f);
    vertex1.uv = vec2(0.0f, 1.0f);
    vertices.push_back(vertex1);
    ParticleVertex vertex2 = ParticleVertex();
    vertex2.position = vec3(0.5f, -0.5f, 0.0f);
    vertex2.uv = vec2(1.0f, 1.0f);
    vertices.push_back(vertex2);
    ParticleVertex vertex3 = ParticleVertex();
    vertex3.position = vec3(0.5f, 0.5f, 0.0f);
    vertex3.uv = vec2(1.0f, 0.0f);
    vertices.push_back(vertex3);
    ParticleVertex vertex4 = ParticleVertex();
    vertex4.position = vec3(-0.5f, 0.5f, 0.0f);
    vertex4.uv = vec2(0.0f, 0.0f);
    vertices.push_back(vertex4);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    this->indiceCount = (unsigned int)indices.size();
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glGenBuffers(1, &this->elementBuffers);
    glBindVertexArray(this->vertexArrays);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ParticleVertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, uv));
    for(unsigned int i = 0; i < this->particleAmount; i += 1) {
        ParticleData data;
        data.birthTimeAndDuration = vec2(0.0f);
        data.initialPosition = vec3(0.0f);
        data.initialSpeed = vec3(0.0f);
        data.accelerationData = vec4(0.0f);
        data.rotationData = vec2(0.0f);
        data.scaleData = vec2(0.0f);
        data.spriteSheetAnimationData = vec2(0.0f);
        this->dataVector.push_back(data);
    }
    glGenBuffers(1, &this->dataBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, this->dataBuffers);
    glBufferData(GL_ARRAY_BUFFER, this->particleAmount * sizeof(ParticleData), &this->dataVector[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, initialPosition));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, initialSpeed));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, accelerationData));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, rotationData));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, scaleData));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, spriteSheetAnimationData));
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
ParticleRenderer::~ParticleRenderer() {
    glDeleteBuffers(1, &this->dataBuffers);
    this->dataVector.clear();
}
void ParticleRenderer::engineResetAllParticleData() {
    for(unsigned int i = 0; i < this->particleAmount; i += 1) {
        this->dataVector[i].birthTimeAndDuration = vec2(0.0f);
    }
}
void ParticleRenderer::engineRenderGeometry(unsigned int renderingMode) {
    glBindBuffer(GL_ARRAY_BUFFER, this->dataBuffers);
    glBufferData(GL_ARRAY_BUFFER, this->particleAmount * sizeof(ParticleData), &this->dataVector[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->shader->engineRenderShader(this, renderingMode);
    this->updated = false;
    this->prepared = false;
}
ParticleData* ParticleRenderer::engineGetParticleData(bool front) {
    if(front) {
        ParticleData data = this->dataVector[0];
        this->dataVector.erase(this->dataVector.begin());
        this->dataVector.push_back(data);
        return(&this->dataVector[(unsigned int)this->dataVector.size() - 1]);
    }
    ParticleData data = this->dataVector[(unsigned int)this->dataVector.size() - 1];
    this->dataVector.erase(this->dataVector.end() - 1);
    this->dataVector.insert(this->dataVector.begin(), data);
    return(&this->dataVector[0]);
}
unsigned int ParticleRenderer::engineGetGeometryInstanceCount() {
    return(this->particleAmount);
}
