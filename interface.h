#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class Interface {
private:
	ImGuiIO& io = ImGui::GetIO();
public:
	Interface(GLFWwindow* window);
	void NewFrame();
	void MainFrame();
	void Render();
	void Destroy();
};