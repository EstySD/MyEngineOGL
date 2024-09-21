#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "shader.h"
#include "material.h"
#ifndef MESH_H_
#define MESH_H_
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex = -1);
    void Draw();
    void Delete();
    unsigned int materialIndex;
protected:
    //  render data
    unsigned int VAO, VBO, EBO;
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    void setupMesh();
};
class BillBoard {
private:
    unsigned int VAO, VBO, EBO;
    //pos + tex coords
    float vertices[20] = {
        -0.5f, -0.5f, 0.0f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.0f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.0f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.0f, 0.5f, -0.5f,

    };
    unsigned int indices[6] = {
        0,1,2,
        0,2,3
    };
public:
    BillBoard();

    void Draw(Shader& shader, Transform& transform, glm::mat4& view);
};
#endif // !MESH_H_

