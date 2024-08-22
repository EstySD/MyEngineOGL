#include "light.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) {
    transform.setLocalPos(position);
    this->color = color;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}
void PointLight::UpdateDistance(float constant, float linear, float quadratic) {
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outercutOff) {
    transform.setLocalPos(position);
    transform.setLocalRot(direction);
    this->color = color;
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}
void SpotLight::UpdateCut(float cutOff, float outercutOff) {
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 color) {
    transform.setLocalRot(direction);
    this->color = color;
}