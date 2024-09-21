#include "mesh.h"
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex) {
	this->vertices = vertices;
	this->indices = indices;
    this->materialIndex = materialIndex;
	setupMesh();
}
void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}
void Mesh::Draw() {

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::Delete() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

BillBoard::BillBoard()
{

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void BillBoard::Draw(Shader& shader, Transform& transform, glm::mat4& view)
{
    glm::vec3 CameraRight_worldspace = glm::vec3(view[0][0], view[1][0], view[2][0]);
    glm::vec3 CameraUp_worldspace = glm::vec3(view[0][1], view[1][1], view[2][1]);
    float newVertices[20];
    std::copy(std::begin(vertices), std::end(vertices), std::begin(newVertices));
    for (int i = 0; i < 4; i++) {
        glm::vec3 v = transform.getPos() + CameraRight_worldspace * vertices[0+5*i]  +
            CameraUp_worldspace * vertices[1+5*i];
        newVertices[5 * i] = v.x;
        newVertices[5 * i+1] = v.y;
        newVertices[5 * i+2] = v.z;
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newVertices), &newVertices[0], GL_STATIC_DRAW);

    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
