#include <glm/glm.hpp>
#include <glad/glad.h> 
#include <glm/gtc/matrix_transform.hpp>
#include "transform.h"

#ifndef LIGHT_H_
#define LIGHT_H_
class Light {
public:
	glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
	Transform transform;
};
class PointLight:public Light {
public:
	glm::vec3 clq;
	

	PointLight(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0),
		glm::vec3 clq=glm::vec3(1.0f, 0.7f, 1.8f)); //constant linear quadratic
	void UpdateDistance(glm::vec3 clq = glm::vec3(1.0f, 0.7f, 1.8f));
	
};
class SpotLight :public Light {
public:
	float cutOff, outercutOff;
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outercutOff);
	void UpdateCut(float cutOff, float outercutOff);
};

class DirectLight :public Light {
public:
	DirectLight() {};
	DirectLight(glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif