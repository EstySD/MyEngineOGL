#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
class Transform {
protected:
   
    glm::mat4 modelMatrix = glm::mat4(1.0f);
public:
    Transform(glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
        glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f },
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f }
    );
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
    void setPos(glm::vec3 pos);
    void setRot(glm::vec3 rot);
    void setScale(glm::vec3 scale);
    void setScale(float scale);
    glm::vec3 getPos();
    glm::vec3 getRot();
    glm::vec3 getScale();
    const glm::mat4 getModelMatrix();
};
#endif