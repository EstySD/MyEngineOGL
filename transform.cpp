#include "transform.h"


Transform::Transform(glm::vec3 pos, glm::vec3 eulerRot, glm::vec3 scale )
{
}

void Transform::setPos(glm::vec3 pos)
{
    this->pos = pos;
}

void Transform::setRot(glm::vec3 rot)
{
    this->eulerRot = rot;
}

void Transform::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

void Transform::setScale(float scale)
{
    this->scale = glm::vec3(scale);
}

glm::vec3 Transform::getPos()
{
    return pos;
}

glm::vec3 Transform::getRot()
{
    return eulerRot;
}

glm::vec3 Transform::getScale()
{
    return scale;
}

const glm::mat4 Transform::getModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    glm::mat4 roationMatrix(1.0);
    roationMatrix = transformY * transformX * transformZ;
    
    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), pos) *
        roationMatrix *
        glm::scale(glm::mat4(1.0f), scale);
}



