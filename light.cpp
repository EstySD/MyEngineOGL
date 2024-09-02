#include "light.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color,glm::vec3 scale,
    glm::vec3 clq) {
    transform.setPos(position);
    transform.setScale(scale);
    this->color = color;
    this->clq = clq;
}
void PointLight::UpdateDistance(glm::vec3 clq) {
    this->clq = clq;
}


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outercutOff) {
    transform.setPos(position);
    transform.setRot(direction);
    this->color = color;
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}
void SpotLight::UpdateCut(float cutOff, float outercutOff) {
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 color) {
    transform.setRot(direction);
    this->color = color;
}