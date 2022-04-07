// Developed by Kelin.Lyu.
#include "geometry.hpp"
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
