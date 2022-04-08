// Developed by Kelin.Lyu.
#include "geometry.hpp"
Geometry::Geometry() {
    this->shader = NULL;
    this->bonesCount = 0;
    this->isHidden = false;
    this->renderingOrder = 0;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Vertex vertex1 = Vertex();
    vertex1.position = vec3(0.5f, -0.5f, 0.5f);
    vertex1.normal = vec3(0.0f, -1.0f, 0.0f);
    vertex1.uv = vec2(0.0f, 1.0f);
    vertex1.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex1.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex1);
    Vertex vertex2 = Vertex();
    vertex2.position = vec3(-0.5f, -0.5f, 0.5f);
    vertex2.normal = vec3(0.0f, -1.0f, 0.0f);
    vertex2.uv = vec2(1.0f, 1.0f);
    vertex2.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex2.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex2);
    Vertex vertex3 = Vertex();
    vertex3.position = vec3(-0.5f, -0.5f, -0.5f);
    vertex3.normal = vec3(0.0f, -1.0f, 0.0f);
    vertex3.uv = vec2(1.0f, 0.0f);
    vertex3.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex3.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex3);
    Vertex vertex4 = Vertex();
    vertex4.position = vec3(0.5f, -0.5f, -0.5f);
    vertex4.normal = vec3(0.0f, -1.0f, 0.0f);
    vertex4.uv = vec2(0.0f, 0.0f);
    vertex4.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex4.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex4);
    Vertex vertex5 = Vertex();
    vertex5.position = vec3(0.5f, -0.5f, -0.5f);
    vertex5.normal = vec3(0.0f, 0.0f, -1.0f);
    vertex5.uv = vec2(0.0f, 1.0f);
    vertex5.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex5.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex5);
    Vertex vertex6 = Vertex();
    vertex6.position = vec3(-0.5f, -0.5f, -0.5f);
    vertex6.normal = vec3(0.0f, 0.0f, -1.0f);
    vertex6.uv = vec2(1.0f, 1.0f);
    vertex6.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex6.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex6);
    Vertex vertex7 = Vertex();
    vertex7.position = vec3(-0.5f, 0.5f, -0.5f);
    vertex7.normal = vec3(0.0f, 0.0f, -1.0f);
    vertex7.uv = vec2(1.0f, 0.0f);
    vertex7.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex7.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex7);
    Vertex vertex8 = Vertex();
    vertex8.position = vec3(0.5f, 0.5f, -0.5f);
    vertex8.normal = vec3(0.0f, 0.0f, -1.0f);
    vertex8.uv = vec2(0.0f, 0.0f);
    vertex8.tangent = vec3(-1.0f, 0.0f, 0.0f);
    vertex8.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex8);
    Vertex vertex9 = Vertex();
    vertex9.position = vec3(-0.5f, -0.5f, 0.5f);
    vertex9.normal = vec3(0.0f, 0.0f, 1.0f);
    vertex9.uv = vec2(0.0f, 1.0f);
    vertex9.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex9.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex9);
    Vertex vertex10 = Vertex();
    vertex10.position = vec3(0.5f, -0.5f, 0.5f);
    vertex10.normal = vec3(0.0f, 0.0f, 1.0f);
    vertex10.uv = vec2(1.0f, 1.0f);
    vertex10.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex10.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex10);
    Vertex vertex11 = Vertex();
    vertex11.position = vec3(0.5f, 0.5f, 0.5f);
    vertex11.normal = vec3(0.0f, 0.0f, 1.0f);
    vertex11.uv = vec2(1.0f, 0.0f);
    vertex11.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex11.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex11);
    Vertex vertex12 = Vertex();
    vertex12.position = vec3(-0.5f, 0.5f, 0.5f);
    vertex12.normal = vec3(0.0f, 0.0f, 1.0f);
    vertex12.uv = vec2(0.0f, 0.0f);
    vertex12.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex12.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex12);
    Vertex vertex13 = Vertex();
    vertex13.position = vec3(-0.5f, 0.5f, 0.5f);
    vertex13.normal = vec3(0.0f, 1.0f, 0.0f);
    vertex13.uv = vec2(0.0f, 1.0f);
    vertex13.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex13.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex13);
    Vertex vertex14 = Vertex();
    vertex14.position = vec3(0.5f, 0.5f, 0.5f);
    vertex14.normal = vec3(0.0f, 1.0f, 0.0f);
    vertex14.uv = vec2(1.0f, 1.0f);
    vertex14.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex14.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex14);
    Vertex vertex15 = Vertex();
    vertex15.position = vec3(0.5f, 0.5f, -0.5f);
    vertex15.normal = vec3(0.0f, 1.0f, 0.0f);
    vertex15.uv = vec2(1.0f, 0.0f);
    vertex15.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex15.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex15);
    Vertex vertex16 = Vertex();
    vertex16.position = vec3(-0.5f, 0.5f, -0.5f);
    vertex16.normal = vec3(0.0f, 1.0f, 0.0f);
    vertex16.uv = vec2(0.0f, 0.0f);
    vertex16.tangent = vec3(1.0f, 0.0f, 0.0f);
    vertex16.bitangent = vec3(0.0f, 0.0f, 1.0f);
    vertices.push_back(vertex16);
    Vertex vertex17 = Vertex();
    vertex17.position = vec3(0.5f, -0.5f, 0.5f);
    vertex17.normal = vec3(1.0f, 0.0f, 0.0f);
    vertex17.uv = vec2(0.0f, 1.0f);
    vertex17.tangent = vec3(0.0f, 0.0f, -1.0f);
    vertex17.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex17);
    Vertex vertex18 = Vertex();
    vertex18.position = vec3(0.5f, -0.5f, -0.5f);
    vertex18.normal = vec3(1.0f, 0.0f, 0.0f);
    vertex18.uv = vec2(1.0f, 1.0f);
    vertex18.tangent = vec3(0.0f, 0.0f, -1.0f);
    vertex18.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex18);
    Vertex vertex19 = Vertex();
    vertex19.position = vec3(0.5f, 0.5f, -0.5f);
    vertex19.normal = vec3(1.0f, 0.0f, 0.0f);
    vertex19.uv = vec2(1.0f, 0.0f);
    vertex19.tangent = vec3(0.0f, 0.0f, -1.0f);
    vertex19.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex19);
    Vertex vertex20 = Vertex();
    vertex20.position = vec3(0.5f, 0.5f, 0.5f);
    vertex20.normal = vec3(1.0f, 0.0f, 0.0f);
    vertex20.uv = vec2(0.0f, 0.0f);
    vertex20.tangent = vec3(0.0f, 0.0f, -1.0f);
    vertex20.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex20);
    Vertex vertex21 = Vertex();
    vertex21.position = vec3(-0.5f, -0.5f, -0.5f);
    vertex21.normal = vec3(-1.0f, 0.0f, 0.0f);
    vertex21.uv = vec2(0.0f, 1.0f);
    vertex21.tangent = vec3(0.0f, 0.0f, 1.0f);
    vertex21.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex21);
    Vertex vertex22 = Vertex();
    vertex22.position = vec3(-0.5f, -0.5f, 0.5f);
    vertex22.normal = vec3(-1.0f, 0.0f, 0.0f);
    vertex22.uv = vec2(1.0f, 1.0f);
    vertex22.tangent = vec3(0.0f, 0.0f, 1.0f);
    vertex22.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex22);
    Vertex vertex23 = Vertex();
    vertex23.position = vec3(-0.5f, 0.5f, 0.5f);
    vertex23.normal = vec3(-1.0f, 0.0f, 0.0f);
    vertex23.uv = vec2(1.0f, 0.0f);
    vertex23.tangent = vec3(0.0f, 0.0f, 1.0f);
    vertex23.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex23);
    Vertex vertex24 = Vertex();
    vertex24.position = vec3(-0.5f, 0.5f, -0.5f);
    vertex24.normal = vec3(-1.0f, 0.0f, 0.0f);
    vertex24.uv = vec2(0.0f, 0.0f);
    vertex24.tangent = vec3(0.0f, 0.0f, 1.0f);
    vertex24.bitangent = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(vertex24);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(10);
    indices.push_back(8);
    indices.push_back(10);
    indices.push_back(11);
    indices.push_back(12);
    indices.push_back(13);
    indices.push_back(14);
    indices.push_back(12);
    indices.push_back(14);
    indices.push_back(15);
    indices.push_back(16);
    indices.push_back(17);
    indices.push_back(18);
    indices.push_back(16);
    indices.push_back(18);
    indices.push_back(19);
    indices.push_back(20);
    indices.push_back(21);
    indices.push_back(22);
    indices.push_back(20);
    indices.push_back(22);
    indices.push_back(23);
    this->vertices = vertices;
    this->indices = indices;
    // create buffers:
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glGenBuffers(1, &this->elementBuffers);
    glBindVertexArray(this->vertexArrays);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
    glBindVertexArray(0);
}
Geometry::Geometry(aiMesh* mesh) {
    this->shader = NULL;
    this->bonesCount = 0;
    this->isHidden = false;
    this->renderingOrder = 0;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh->mNumVertices; i += 1) {
        Vertex vertex;
        for(unsigned int k = 0; k < MAX_BONE_INFLUENCE; k= k + 1) {
            vertex.boneIDs[k] = -1;
            vertex.weights[k] = 0.0f;
        }
        vertex.position = assimp_helper::getVec3(mesh->mVertices[i]);
        if(mesh->HasNormals()) {
            vertex.normal = assimp_helper::getVec3(mesh->mNormals[i]);
        }
        if(mesh->mTextureCoords[0]) {
            vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;
            vertex.tangent = assimp_helper::getVec3(mesh->mTangents[i]);
            vertex.bitangent = assimp_helper::getVec3(mesh->mBitangents[i]);
        }else{
            vertex.uv = vec2(0.0f);
        }
        vertices.push_back(vertex);
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i += 1) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j += 1) {
            indices.push_back(face.mIndices[j]);
        }
    }
    if(mesh->mNumBones > 0) {
        for(unsigned int i = 0; i < mesh->mNumBones; i += 1) {
            int boneID = -1;
            string boneName = mesh->mBones[i]->mName.C_Str();
            if(this->bonesInfoMap.find(boneName) == this->bonesInfoMap.end()) {
                BoneInfo newBoneInfo;
                newBoneInfo.id = this->bonesCount;
                newBoneInfo.offset = assimp_helper::getMat4(mesh->mBones[i]->mOffsetMatrix);
                this->bonesInfoMap[boneName] = newBoneInfo;
                boneID = this->bonesCount;
                this->bonesCount = this->bonesCount + 1;
            }else{
                boneID = this->bonesInfoMap[boneName].id;
            }
            aiVertexWeight* weights = mesh->mBones[i]->mWeights;
            int numWeights = mesh->mBones[i]->mNumWeights;
            for(unsigned int j = 0; j < numWeights; j += 1) {
                int vertexId = weights[j].mVertexId;
                float weight = weights[j].mWeight;
                for(unsigned int k = 0; k < MAX_BONE_INFLUENCE; k += 1) {
                    if(vertices[vertexId].boneIDs[k] < 0) {
                        vertices[vertexId].boneIDs[k] = boneID;
                        vertices[vertexId].weights[k] = weight;
                        break;
                    }
                }
            }
        }
        this->boneTransforms.reserve(BONES_LIMIT);
        for(unsigned int i = 0; i < BONES_LIMIT; i += 1) {
            this->boneTransforms.push_back(mat4(1.0f));
        }
    }
    this->vertices = vertices;
    this->indices = indices;
    // create buffers:
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glGenBuffers(1, &this->elementBuffers);
    glBindVertexArray(this->vertexArrays);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
    glBindVertexArray(0);
}
void Geometry::setShader(Shader* shader) {
    this->shader = shader;
}
void Geometry::addAnimation(Animation* animation) {
    this->animations.push_back(animation);
}
void Geometry::update(mat4 worldTransform) {
    this->worldTransform = worldTransform;
    Engine::main->prepareGeometryForRendering(this);
}
void Geometry::render(vector<LightNode*>* lights) {
    if(this->isHidden) {
        return;
    }
    if(this->shader == NULL) {
        this->setShader(new Shader(this));
    }
    this->shader->setActivate();
    if(this->hasBones()) {
        this->shader->setInt("hasBones", 1);
        if(this->animations.size() > 0) {
            this->calculateBoneTransforms(this->animations[0]->assimpRootNode, mat4(1.0f), true);
        }
        for(unsigned int i = 0; i < BONES_LIMIT; i += 1) {
            this->shader->setMat4("boneTransforms[" + to_string(i) + "]", boneTransforms[i]);
        }
    }else{
        this->shader->setInt("hasBones", 0);
    }
    for(unsigned int i = 0; i < LIGHTS_LIMIT; i += 1) {
        if(i < lights->size()) {
            (*lights)[i]->configurateShader(this->shader, i, this->worldTransform);
        }else{
            this->shader->setInt("lights[" + to_string(i) + "].type", -1);
        }
    }
    this->shader->render(this->worldTransform);
    glBindVertexArray(this->vertexArrays);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Geometry::calculateBoneTransforms(AssimpNode *node, mat4 parentTransform, bool first) {
    string nodeName = node->name;
    vector<mat4> nodeTransforms;
    mat4 finalTransform = node->transform;
    for(unsigned int i = 0; i < this->animations.size(); i += 1) {
        Bone* bone = this->animations[i]->getBone(nodeName);
        if(bone == NULL || this->animations[i]->animator == NULL) {
            if(first) {
                nodeTransforms.push_back(node->transform);
            }else{
                nodeTransforms.push_back(mat4(0.0f));
            }
        }else{
            bone->update(this->animations[i]->animator->getTime());
            nodeTransforms.push_back(bone->getTransform());
        }
    }
    for(unsigned int i = 0; i < nodeTransforms.size(); i += 1) {
        if(this->animations[i]->animator == NULL) {
            continue;
        }
        if(nodeTransforms[i] == mat4(0.0f)) {
            continue;
        }
        float blendFactor = this->animations[i]->animator->getBlendFactor();
        if(blendFactor == 0.0f) {
            continue;
        }
        quat rotation = quat_cast(finalTransform);
        quat newRotation = quat_cast(nodeTransforms[i]);
        quat blendedRotation = slerp(rotation, newRotation, blendFactor);
        mat4 matrix = mat4_cast(blendedRotation);
        matrix[3] = finalTransform[3] * (1.0f - blendFactor) + nodeTransforms[i][3] * blendFactor;
        finalTransform = matrix;
    }
    mat4 globalTransform = parentTransform * finalTransform;
    if(this->bonesInfoMap.find(nodeName) != this->bonesInfoMap.end()) {
        int index = this->bonesInfoMap[nodeName].id;
        mat4 offset = this->bonesInfoMap[nodeName].offset;
        this->boneTransforms[index] = globalTransform * offset;
    }
    for(unsigned int i = 0; i < node->children.size(); i += 1) {
        this->calculateBoneTransforms(node->children[i], globalTransform, false);
    }
}
bool Geometry::hasBones() {
    return(this->bonesCount > 0);
}
map<string, BoneInfo>& Geometry::getBonesInfoMap() {
    return(this->bonesInfoMap);
}
int& Geometry::getBonesCount() {
    return(this->bonesCount);
}
void Geometry::setRenderingOrder(int renderingOrder) {
    this->renderingOrder = renderingOrder;
}
int Geometry::getRenderingOrder() {
    return(this->renderingOrder);
}
Geometry::~Geometry() {
    glDeleteVertexArrays(1, &this->vertexArrays);
    glDeleteBuffers(1, &this->vertexBuffers);
    glDeleteBuffers(1, &this->elementBuffers);
    this->vertices.clear();
    this->indices.clear();
    for(unsigned int i = 0; i < this->animations.size(); i += 1) {
        delete(this->animations[i]);
    }
    this->animations.clear();
}
