#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
class Transform {
protected:
    /*SPACE INFORMATION*/
    //Local space information

    //Global space information concatenate in matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
public:
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
    void computeModelMatrix();
    void setLocalPos(glm::vec3 pos);
    void setLocalRot(glm::vec3 rot);
    void setLocalScale(glm::vec3 scale);
    void setLocalScale(float scale);
    glm::vec3 getLocalPos();
    glm::vec3 getLocalRot();
    glm::vec3 getLocalScale();
    const glm::mat4& getModelMatrix();
    const glm::mat4 getTransformMatrix(glm::mat4 projection, glm::mat4 view);
protected:
    glm::mat4 getLocalModelMatrix();
};
#endif