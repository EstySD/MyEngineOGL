#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "transform.h"
class Model {
    public:
        Model(const char* path);
        void Draw(Shader& shader);
        Transform transform;
    private:
        // model data
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        Mat loadMaterial(aiMaterial* mat);
        unsigned int texturePush(const char* texturePath, TextureType type);
};