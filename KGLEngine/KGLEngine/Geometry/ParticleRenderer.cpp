// Developed by Kelin Lyu.
#include "Geometry.hpp"
ParticleRenderer::ParticleRenderer() {
    this->engineInitializeGeometry();
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
    glBindVertexArray(0);
    
    
    
    
    
    
    
    
    
    
}
