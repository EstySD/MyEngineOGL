#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "transform.h"
#ifndef MODEL_H_
#define MODEL_H_

class Model {
public:
    const char* path;
    Model(const char* path);
    void Draw(Shader& shader);
    void Draw(Shader& shader, Mat& customMat);
    Transform transform;
    unsigned int MaterialIndex = 0;
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::vector<Mat> materials_loaded;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    Mat loadMaterial(aiMaterial* mat);
    void LoadMaterials(const aiScene* scene);
    unsigned int texturePush(const char* texturePath, TextureType type);
};

#endif // !MODEL_H_
