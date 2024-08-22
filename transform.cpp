#include "transform.h"

void Transform::computeModelMatrix()
{
    modelMatrix = getLocalModelMatrix();
}

void Transform::setLocalPos(glm::vec3 pos)
{
    this->pos = pos;
}

void Transform::setLocalRot(glm::vec3 rot)
{
    this->eulerRot = rot;
}

void Transform::setLocalScale(glm::vec3 scale)
{
    this->scale = scale;
}

void Transform::setLocalScale(float scale)
{
    this->scale = glm::vec3(scale);
}

glm::vec3 Transform::getLocalPos()
{
    return pos;
}

glm::vec3 Transform::getLocalRot()
{
    return eulerRot;
}

glm::vec3 Transform::getLocalScale()
{
    return scale;
}

const glm::mat4& Transform::getModelMatrix()
{
        return modelMatrix;
}

const glm::mat4 Transform::getTransformMatrix(glm::mat4 projection, glm::mat4 view)
{
    computeModelMatrix();
    glm::mat4 transform(1.0f);
    transform = projection * view * modelMatrix * transform;
    return transform;
}

glm::mat4 Transform::getLocalModelMatrix()
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
    const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), pos) *
        roationMatrix *
        glm::scale(glm::mat4(1.0f), scale);
}
