#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef CAMERA_H_
#define CAMERA_H_
class Camera
{
public:
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f), m_direction = glm::vec3(0.0f, 0.0f, -1.0f), m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec2 resolution;
	float yaw = -90.0f, pitch = 0.0f;
	const float speed = 2.5f;
	float m_fov = 45.0f;
	float m_deltaTime = 0.0f;
	Camera();
	void SetSpawn(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov);
	glm::vec3 GetPos();
	glm::vec3 GetDir();
	void Rotate(glm::vec2 offset); //yaw and pitch
	void Move(glm::vec4 wasd);
	void Update(float deltaTime, glm::vec2 resolution);
	glm::mat4 GetViewMat();
	glm::mat4 GetProj();
	void Zoom(float yoffset);
};


#endif // Camera

