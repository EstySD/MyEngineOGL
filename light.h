#include <glm/glm.hpp>
#include <glad/glad.h> 
#include <glm/gtc/matrix_transform.hpp>

#ifndef LIGHT_H_
#define LIGHT_H_
class Light {
public:
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	void Update(glm::vec3 position, glm::vec3 direction,glm::vec3 color);
	glm::vec3 GetColor();
	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
};
class PointLight:public Light {
public:
    float constant, linear, quadratic;
	

	PointLight(glm::vec3 position,glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float constant = 1.0f, float linear=0.14f, float quadratic=0.07f);
	void UpdateDistance(float constant, float linear, float quadratic);
	
};
class SpotLight :public Light {
	float cutOff, outercutOff;
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outercutOff);
	void UpdateCut(float cutOff, float outercutOff);
};

class DirectLight :public Light {
public:
	DirectLight(glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif