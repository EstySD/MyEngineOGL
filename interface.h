#include "ext/imgui/imgui.h"
#include "ext/imgui/imgui_impl_glfw.h"
#include "ext/imgui/imgui_impl_opengl3.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "scene.h"
#include "ext/tinyfiledialogs/tinyfiledialogs.h"

class Editor {
private:
	enum ObjectType {
		None,
		ObjectTp,
		PointLightTp,
		DirLightTp
	};
	void SliderAppend(const char* name, glm::vec3& value, float min = -1.0, float max = 1.0);
	void ColorAppend(const char* name, glm::vec3& value);
	void DragAppend(const char* name, glm::vec3& value, float step = 0.1);
public:
	ImGuiIO& io = ImGui::GetIO();
	Editor(GLFWwindow* window);
	void Framerate();
	void ScenePanels(Scene& scene);
};