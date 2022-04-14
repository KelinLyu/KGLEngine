// Developed by Kelin Lyu.
#include "Geometry.hpp"
Geometry::Geometry(aiMesh* mesh) {
    this->engineInitializeGeometry();
    vector<GeometryVertex> vertices;
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh->mNumVertices; i += 1) {
        GeometryVertex vertex;
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
            this->boneTransforms.push_back(mat4(0.0f));
        }
    }
    this->indiceCount = (unsigned int)indices.size();
    // create buffers:
    glGenVertexArrays(1, &this->vertexArrays);
    glGenBuffers(1, &this->vertexBuffers);
    glGenBuffers(1, &this->elementBuffers);
    glBindVertexArray(this->vertexArrays);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GeometryVertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffers);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers:
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
}
void Geometry::cullBack() {
    this->cullMode = 0;
}
void Geometry::cullFront() {
    this->cullMode = 1;
}
void Geometry::doubleSided() {
    this->cullMode = 2;
}
void Geometry::setShader(Shader* shader) {
    this->shader = shader;
}
void Geometry::setMaterial(Material* material) {
    this->material = material;
    this->shader = material->engineGetMaterialShader();
}
void Geometry::setUIMaterial(UIMaterial* material) {
    this->uiMaterial = material;
    this->shader = material->engineGetUIMaterialShader();
}
Geometry::~Geometry() {
    glDeleteVertexArrays(1, &this->vertexArrays);
    glDeleteBuffers(1, &this->vertexBuffers);
    glDeleteBuffers(1, &this->elementBuffers);
    for(unsigned int i = 0; i < this->animations.size(); i += 1) {
        delete(this->animations[i]);
    }
    this->animations.clear();
    this->shader = NULL;
    this->material = NULL;
    this->uiMaterial = NULL;
}
void Geometry::engineInitializeGeometry() {
    this->cullMode = 0;
    this->shader = NULL;
    this->material = NULL;
    this->uiMaterial = NULL;
    this->bonesCount = 0;
    this->modelTransform = mat4(0.0f);
    this->isHidden = false;
    this->renderingOrder = 0.0f;
    this->lightMask = -1;
    this->clearDepthBuffer = false;
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
bool Geometry::engineCheckIfGeometryHasBones() {
    return(this->bonesCount > 0);
}
int& Geometry::engineGetGeometryBonesCountReference() {
    return(this->bonesCount);
}
map<string, BoneInfo>& Geometry::engineGetGeometryBonesInfoMapReference() {
    return(this->bonesInfoMap);
}
vector<mat4>& Geometry::engineGetGeometryBoneTransformsReference() {
    return(this->boneTransforms);
}
void Geometry::engineCalculateGeometryBoneTransforms(AnimationBoneNode *node, mat4 parentTransform, bool first) {
    string nodeName = node->name;
    vector<mat4> nodeTransforms;
    mat4 finalTransform = node->transform;
    for(unsigned int i = 0; i < this->animations.size(); i += 1) {
        Bone* bone = this->animations[i]->engineGetBone(nodeName);
        if(bone == NULL || this->animations[i]->engineGetAnimator() == NULL) {
            if(first) {
                nodeTransforms.push_back(node->transform);
            }else{
                nodeTransforms.push_back(mat4(0.0f));
            }
        }else{
            bone->engineUpdateBoneAnimation(this->animations[i]->engineGetAnimator()->getTime());
            nodeTransforms.push_back(bone->engineGetTransform());
        }
    }
    for(unsigned int i = 0; i < nodeTransforms.size(); i += 1) {
        if(this->animations[i]->engineGetAnimator() == NULL) {
            continue;
        }
        if(nodeTransforms[i] == mat4(0.0f)) {
            continue;
        }
        float blendFactor = this->animations[i]->engineGetAnimator()->enginegetCurrentBlendFactor();
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
        this->engineCalculateGeometryBoneTransforms(node->children[i], globalTransform, false);
    }
}
mat4 Geometry::engineGetGeometryBoneTransform(string name) {
    if(this->engineCheckIfGeometryHasBones()) {
        if(this->bonesInfoMap.find(name) != this->bonesInfoMap.end()) {
            int index = this->bonesInfoMap[name].id;
            mat4 transform = this->boneTransforms[index];
            if(transform != mat4(0.0f)) {
                mat4 offset = this->bonesInfoMap[name].offset;
                return(transform * inverse(offset));
            }
        }
    }
    return(mat4(0.0f));
}
void Geometry::engineAddAnimationToGeometry(Animation* animation) {
    this->animations.push_back(animation);
}
void Geometry::engineUpdateGeometryAnimations() {
    if(this->engineCheckIfGeometryHasBones()) {
        if(this->animations.size() > 0) {
            this->engineCalculateGeometryBoneTransforms(this->animations[0]->engineGetRootAnimationBoneNode(), mat4(1.0f), true);
        }
    }
}
void Geometry::enginePrepareGeometryForRendering(mat4 worldTransform) {
    this->modelTransform = worldTransform;
    if(this->isHidden) {
        return;
    }
    if(this->shader == NULL) {
        this->setShader(new Shader());
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
void Geometry::engineRenderGeometry() {
    if(this->clearDepthBuffer) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    if(this->cullMode == 0) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }else if(this->cullMode == 1) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }else if(this->cullMode == 2) {
        glDisable(GL_CULL_FACE);
    }
    if(this->material != NULL) {
        this->material->engineRenderMaterial();
    }
    if(this->uiMaterial != NULL) {
        this->uiMaterial->engineRenderUIMaterial();
    }
    this->shader->engineRenderShader(this);
}
