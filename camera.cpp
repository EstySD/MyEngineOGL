#include "camera.h"

Camera::Camera() {
	
}
void Camera::SetSpawn(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov) {
	m_position = position;
	m_direction = direction;
	m_up = up;
	m_fov = fov;
}
glm::vec3 Camera::GetPos() {
	return m_position;
}
glm::vec3 Camera::GetDir() {
	return m_direction;
}
void Camera::Update(float deltaTime, float width, float height) {
	m_deltaTime = deltaTime;
	m_width = width;
	m_height = height;
}
void Camera::Rotate(float xoffset, float yoffset) {
	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	m_direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	m_direction.y = glm::sin(glm::radians(pitch));
	m_direction.z= glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	m_direction = glm::normalize(m_direction);
}
void Camera::Move(glm::vec4 wasd) {
	m_position += speed *m_deltaTime* (
		wasd.x * m_direction+
		-wasd.y* glm::normalize(glm::cross(m_direction, m_up))
		-wasd.z* m_direction
		+ wasd.w*glm::normalize(glm::cross(m_direction, m_up))
		);
}
void Camera::Zoom(float yoffset) {
	m_fov -= (float)yoffset;
	if (m_fov < 1.0f)
		m_fov = 1.0f;
	if (m_fov > 45.0f)
		m_fov = 45.0f;
}
glm::mat4 Camera::GetViewMat() {
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(m_position, m_position + m_direction, m_up);
	return view;
}
glm::mat4 Camera::GetProj() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(m_fov), (float)m_width / (float)m_height, 0.1f, 100.0f);
	return projection;
}
