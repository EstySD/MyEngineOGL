#include "light.h"
void Light::Update(glm::vec3 position, glm::vec3 direction, glm::vec3 color) {
    this->direction = direction;
    this->position = position;
    this->color = color;
}
glm::vec3 Light::GetColor() { return color; };
glm::vec3 Light::GetPosition() { return position; };
glm::vec3 Light::GetDirection() { return direction; };
PointLight::PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) {
    this->position = position;
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
    this->position = position;
    this->direction = direction;
    this->color = color;
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}
void SpotLight::UpdateCut(float cutOff, float outercutOff) {
    this->cutOff = cutOff;
    this->outercutOff = outercutOff;
}

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 color) {
    this->direction = direction;
    this->color = color;
}