#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
class Interface {
private:
public:
	ImGuiIO& io = ImGui::GetIO();
	Interface(GLFWwindow* window);
	void Framerate();
	void SliderAppend(const char* name, glm::vec3& value);
	void DragAppend(const char* name, glm::vec3& value);
};