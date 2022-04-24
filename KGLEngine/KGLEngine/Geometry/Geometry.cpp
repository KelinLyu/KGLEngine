// Developed by Kelin Lyu.
#include "Geometry.hpp"
Geometry::Geometry(aiMesh* mesh) {
    this->engineInitializeGeometry();
    bool boundingBoxInitialized = false;
    vec3 minPosition = vec3(0.0f);
    vec3 maxPosition = vec3(0.0f);
    vector<GeometryVertex> vertices;
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh->mNumVertices; i += 1) {
        GeometryVertex vertex;
        for(unsigned int k = 0; k < MAX_BONE_INFLUENCE; k= k + 1) {
            vertex.boneIDs[k] = -1;
            vertex.weights[k] = 0.0f;
        }
        vertex.position = assimp_helper::getVec3(mesh->mVertices[i]);
        if(boundingBoxInitialized) {
            if(vertex.position.x < minPosition.x) {
                minPosition.x = vertex.position.x;
            }
            if(vertex.position.y < minPosition.y) {
                minPosition.y = vertex.position.y;
            }
            if(vertex.position.z < minPosition.z) {
                minPosition.z = vertex.position.z;
            }
            if(vertex.position.x > maxPosition.x) {
                maxPosition.x = vertex.position.x;
            }
            if(vertex.position.y > maxPosition.y) {
                maxPosition.y = vertex.position.y;
            }
            if(vertex.position.z > maxPosition.z) {
                maxPosition.z = vertex.position.z;
            }
        }else{
            boundingBoxInitialized = true;
            minPosition = vertex.position;
            maxPosition = vertex.position;
        }
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
            bool found = false;
            for(unsigned int j = 0; j < this->boneNames.size(); j += 1) {
                if(boneName == this->boneNames[j]) {
                    boneID = this->boneInfos[j].id;
                }
            }
            if(!found) {
                BoneInfo newBoneInfo;
                newBoneInfo.id = this->boneCount;
                newBoneInfo.offset = assimp_helper::getMat4(mesh->mBones[i]->mOffsetMatrix);
                newBoneInfo.index = -1;
                this->boneNames.push_back(boneName);
                this->boneInfos.push_back(newBoneInfo);
                boneID = this->boneCount;
                this->boneCount = this->boneCount + 1;
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
            this->boneTransforms.push_back(mat4(0.0f));
        }
    }
    this->indiceCount = (unsigned int)indices.size();
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glGenBuffers(1, &this->elementBuffers);
    glBindVertexArray(this->vertexArrays);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GeometryVertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, bitangent));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, boneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, weights));
    glBindVertexArray(0);
    this->hasBoundingSphereInformation = true;
    this->boundingSpherePosition = (maxPosition + minPosition) * 0.5f;
    this->boundingSphereRadius = glm::length(maxPosition - minPosition);
}
Geometry* Geometry::copy(vector<Animator*>* animators) {
    Geometry* geometry = new Geometry();
    geometry->engineInitializeGeometry();
    geometry->cullMode = this->cullMode;
    geometry->indiceCount = this->indiceCount;
    geometry->boneCount = this->boneCount;
    geometry->boneNames = this->boneNames;
    geometry->boneInfos = this->boneInfos;
    geometry->boneTransforms = this->boneTransforms;
    geometry->isHidden = this->isHidden;
    geometry->renderingOrder = this->renderingOrder;
    geometry->lightingBitMask = this->lightingBitMask;
    geometry->hasBoundingSphereInformation = this->hasBoundingSphereInformation;
    geometry->boundingSpherePosition = this->boundingSpherePosition;
    geometry->boundingSphereRadius = this->boundingSphereRadius;
    glGenVertexArrays(1, &geometry->vertexArrays);
    glGenBuffers(1, &geometry->vertexBuffers);
    glGenBuffers(1, &geometry->elementBuffers);
    glBindVertexArray(geometry->vertexArrays);
    int size = 0;
    glBindBuffer(GL_COPY_READ_BUFFER, this->vertexBuffers);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, size);
    glBindBuffer(GL_COPY_READ_BUFFER, this->elementBuffers);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, size);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, bitangent));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, boneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GeometryVertex), (void*)offsetof(GeometryVertex, weights));
    glBindVertexArray(0);
    geometry->setShader(this->shader);
    return(geometry);
}
void Geometry::cullBack() {
    this->cullMode = 0;
}
void Geometry::cullFront() {
    this->cullMode = 1;
}
void Geometry::setDoubleSided() {
    this->cullMode = 2;
}
void Geometry::setShader(Shader* shader) {
    this->shader = shader;
}
Geometry::~Geometry() {
    glDeleteVertexArrays(1, &this->vertexArrays);
    glDeleteBuffers(1, &this->vertexBuffers);
    glDeleteBuffers(1, &this->elementBuffers);
    glDeleteBuffers(1, &this->modelTransformBuffers);
    glDeleteBuffers(1, &this->normalTransformBuffers);
    this->modelTransforms.clear();
    this->normalTransforms.clear();
    this->shader = NULL;
}
void Geometry::engineInitializeGeometry() {
    this->updated = false;
    this->prepared = false;
    this->cullMode = 0;
    this->shader = NULL;
    this->boneCount = 0;
    this->skeletalAnimationUpdated = false;
    this->modelTransform = mat4(0.0f);
    this->isHidden = false;
    this->renderingOrder = 0.0f;
    this->lightingBitMask = -1;
    this->instanceCount = 0;
    this->requiresInstanceUpdate = false;
    this->hasBoundingSphereInformation = false;
    this->boundingSpherePosition = vec3(0.0f);
    this->boundingSphereRadius = 0.0f;
}
mat4 Geometry::engineGetGeometryModelTransform() {
    return(this->modelTransform);
}
unsigned int Geometry::engineGetGeometryVertexArrays() {
    return(this->vertexArrays);
}
unsigned int Geometry::engineGetGeometryIndiceCount() {
    return(this->indiceCount);
}
bool Geometry::engineCheckWhetherGeometryHasBones() {
    return(this->boneCount > 0);
}
vector<mat4>* Geometry::engineGetGeometryBoneTransforms() {
    return(&this->boneTransforms);
}
mat4 Geometry::engineGetGeometryBoneTransform(string name) {
    if(this->engineCheckWhetherGeometryHasBones()) {
        for(unsigned int i = 0; i < this->boneNames.size(); i += 1) {
            if(name == this->boneNames[i]) {
                int index = this->boneInfos[i].id;
                mat4 transform = this->boneTransforms[index];
                if(transform != mat4(0.0f)) {
                    mat4 offset = this->boneInfos[i].offset;
                    return(transform * glm::inverse(offset));
                }
            }
        }
    }
    return(mat4(0.0f));
}
void Geometry::engineUpdateGeometryBoneIndices(vector<string>* boneNames) {
    for(unsigned int i = 0; i < (*boneNames).size(); i += 1) {
        for(unsigned int j = 0; j < this->boneNames.size(); j += 1) {
            if((*boneNames)[i] == this->boneNames[j]) {
                this->boneInfos[j].index = i;
            }
        }
    }
}
void Geometry::engineUpdateGeometrySkeletalAnimations(vector<mat4> boneTransforms) {
    if(this->updated) {
        return;
    }
    if(this->isHidden) {
        return;
    }
    if(this->boneCount > 0) {
        for(unsigned int i = 0; i < this->boneInfos.size(); i += 1) {
            int index = this->boneInfos[i].index;
            if(index > -1) {
                int id = this->boneInfos[i].id;
                mat4 offset = this->boneInfos[i].offset;
                this->boneTransforms[id] = boneTransforms[index] * offset;
            }
        }
        this->skeletalAnimationUpdated = true;
    }
    this->updated = true;
}
bool Geometry::engineCheckWhetherGeometryHasUpdatedSkeletalAnimations() {
    return(this->skeletalAnimationUpdated);
}
void Geometry::enginePrepareGeometryForRendering(mat4 worldTransform) {
    if(this->prepared) {
        return;
    }
    if(this->isHidden) {
        return;
    }
    this->prepared = true;
    this->modelTransform = worldTransform;
    if(this->shader == NULL) {
        this->setShader(new Shader());
    }
    unsigned int size = (unsigned int)this->modelTransforms.size();
    if(size > 0 && this->requiresInstanceUpdate) {
        if(this->instanceCount != size) {
            if(this->instanceCount == 0) {
                glBindVertexArray(this->vertexArrays);
                glGenBuffers(1, &this->modelTransformBuffers);
                glBindBuffer(GL_ARRAY_BUFFER, this->modelTransformBuffers);
                glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), &this->modelTransforms[0], GL_STATIC_DRAW);
                glEnableVertexAttribArray(7);
                glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
                glEnableVertexAttribArray(8);
                glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
                glEnableVertexAttribArray(9);
                glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(10);
                glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
                glVertexAttribDivisor(7, 1);
                glVertexAttribDivisor(8, 1);
                glVertexAttribDivisor(9, 1);
                glVertexAttribDivisor(10, 1);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glGenBuffers(1, &this->normalTransformBuffers);
                glBindBuffer(GL_ARRAY_BUFFER, this->normalTransformBuffers);
                glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), &this->normalTransforms[0], GL_STATIC_DRAW);
                glEnableVertexAttribArray(11);
                glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
                glEnableVertexAttribArray(12);
                glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
                glEnableVertexAttribArray(13);
                glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(14);
                glVertexAttribPointer(14, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
                glVertexAttribDivisor(11, 1);
                glVertexAttribDivisor(12, 1);
                glVertexAttribDivisor(13, 1);
                glVertexAttribDivisor(14, 1);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            this->instanceCount = size;
        }
    }
    if(this->renderingOrder <= 0.0f) {
        Engine::main->preparedGeometries.insert(Engine::main->preparedGeometries.begin(), this);
        return;
    }
    for(unsigned int i = 0; i < Engine::main->preparedGeometries.size(); i += 1) {
        if(Engine::main->preparedGeometries[i]->renderingOrder > this->renderingOrder) {
            Engine::main->preparedGeometries.insert(Engine::main->preparedGeometries.begin() + i, this);
            return;
        }
    }
    Engine::main->preparedGeometries.push_back(this);
}
void Geometry::engineRenderGeometry(bool shadowMap) {
    if(this->cullMode == 0) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }else if(this->cullMode == 1) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }else if(this->cullMode == 2) {
        glDisable(GL_CULL_FACE);
    }
    if(this->instanceCount > 0 && this->requiresInstanceUpdate) {
        glBindBuffer(GL_ARRAY_BUFFER, this->modelTransformBuffers);
        glBufferData(GL_ARRAY_BUFFER, this->instanceCount * sizeof(mat4), &this->modelTransforms[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, this->normalTransformBuffers);
        glBufferData(GL_ARRAY_BUFFER, this->instanceCount * sizeof(mat4), &this->normalTransforms[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        this->requiresInstanceUpdate = false;
    }
    this->shader->engineRenderShader(this, shadowMap);
    this->updated = false;
    this->prepared = false;
}
unsigned int Geometry::engineGeometryAddInstance() {
    this->frozenTransforms.push_back(false);
    this->modelTransforms.push_back(mat4(0.0f));
    this->normalTransforms.push_back(mat4(0.0f));
    this->requiresInstanceUpdate = true;
    return((unsigned int)this->modelTransforms.size() - 1);
}
void Geometry::engineUpdateGeometryInstanceTransform(unsigned int index, mat4 modelTransform, bool freeze) {
    this->frozenTransforms[index] = freeze;
    this->modelTransforms[index] = modelTransform;
    this->normalTransforms[index] = transpose(inverse(modelTransform));
    this->requiresInstanceUpdate = true;
}
unsigned int Geometry::engineGetGeometryInstanceCount() {
    return((unsigned int)this->modelTransforms.size());
}
bool Geometry::engineCheckWhetherGeometryIsAffectedByLightNode(LightNode* lightNode) {
    if((this->lightingBitMask & lightNode->lightingBitMask) == 0) {
        return(false);
    }
    if(this->hasBoundingSphereInformation == true && this->instanceCount == 0) {
        if(lightNode->engineGetLightType() > 1) {
            mat4 transform = glm::translate(mat4(1.0f), this->boundingSpherePosition);
            transform = this->modelTransform * transform;
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(transform, scale, rotation, translation, skew, perspective);
            float distance = glm::length(lightNode->getWorldPosition() - translation);
            float radius = glm::max(scale.x, glm::max(scale.y, scale.z)) * this->boundingSphereRadius;
            if(distance > radius + lightNode->range) {
                return(false);
            }
        }
    }
    return(true);
}
