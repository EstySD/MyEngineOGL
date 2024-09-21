#include "model.h"
Model::Model(const char* path)
{
    this->path = path;
    loadModel(path);
}
void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of("\\") + 1);
    LoadMaterials(scene);
    processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        //materials_loaded.push_back(loadMaterial(scene->mMaterials[mesh->mMaterialIndex]));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    return Mesh(vertices, indices, mesh->mMaterialIndex-1);
}


void Model::LoadMaterials(const aiScene* scene)
{
    for (unsigned int i = 1; i < scene->mNumMaterials; i++) {
        aiMaterial* aimat = scene->mMaterials[i];
        aiString str;
        std::string s = directory;
        aimat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        s.append(str.data);
        unsigned int diffuseIndex = texturePush(s.c_str(), TextureType::diffuse);
        s = directory;
        aimat->GetTexture(aiTextureType_SPECULAR, 0, &str);
        s.append(str.data);
        unsigned int specularIndex = texturePush(s.c_str(), TextureType::specular);
        Mat mat(textures_loaded[diffuseIndex], textures_loaded[specularIndex]);
        materials_loaded.push_back(mat);
    }
}

unsigned int Model::texturePush(const char* texturePath, TextureType type) {
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path, texturePath) == 0)
        {
            return j;
        }
    }
    Texture texture(texturePath, type);
    textures_loaded.push_back(texture);
    return (unsigned int)textures_loaded.size()-1;
}
void Model::Draw(Shader& shader) {
    for (int i = 0; i < meshes.size(); i++) {
        materials_loaded[meshes[i].materialIndex].assign(shader);
        meshes[i].Draw();
    }
}
void Model::Draw(Shader& shader, Mat& customMat) {
    for (int i = 0; i < meshes.size(); i++) {
        customMat.assign(shader);
        meshes[i].Draw();
    }
}