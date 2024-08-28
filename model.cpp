#include "model.h"
Model::Model(const char* path)
{
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
    directory = path.substr(0, path.find_last_of('/')+1);
    processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        materials_loaded.push_back(loadMaterial(scene->mMaterials[mesh->mMaterialIndex]));
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
    if (mesh->mMaterialIndex < 0) return Mesh(vertices, indices);
    return Mesh(vertices, indices);
}
Mat Model::loadMaterial(aiMaterial* mat) {
    aiString str;
    std::string s = directory;
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
    s.append(str.data);
    unsigned int diffuseI = texturePush(s.c_str(), TextureType::specular);
    s = directory;
    mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
    s.append(str.data);
    unsigned int specularI = texturePush(s.c_str(), TextureType::specular);
    return Mat(textures_loaded[diffuseI], textures_loaded[specularI]);
}

unsigned int Model::texturePush(const char* texturePath, TextureType type) {
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), texturePath) == 0)
        {
            return j;
        }
    }
    Texture texture(texturePath, type);
    textures_loaded.push_back(texture);
    return textures_loaded.size()-1;
}
void Model::Draw(Shader& shader) {
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader, materials_loaded[i]);
    }
}
void Model::Draw(Shader& shader, Mat customMat) {
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader, customMat);
    }
}