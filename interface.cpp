#include "interface.h"

Editor::Editor(GLFWwindow* window)
{
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

}


void Editor::Framerate()
{
    ImGui::Begin("FramerateWindow");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    //ImGui::ShowDemoWindow();
}

void Editor::SliderAppend(const char* name, glm::vec3& value, float min, float max)
{
    ImGui::SliderFloat3(name, (float*)&value, min, max);
}

void Editor::ColorAppend(const char* name, glm::vec3& value)
{
    ImGui::ColorEdit3(name, (float*)&value);
}

void Editor::DragAppend(const char* name, glm::vec3& value, float step)
{
    ImGui::DragFloat3(name, (float*)&value, step);
}

void Editor::ScenePanels(Scene& scene)
{
    ImGui::Begin("Scene");
    if(ImGui::Button("Save")) scene.Save();
    ImGui::End();

    static ObjectType selected;
    //
    ImGui::Begin("ObjectPanel");
    static int selectedObject = 0;
    for (unsigned int i = 0; i < scene.GetModels().size(); i++)
    {
        ImGui::PushID(i);
        if (ImGui::Selectable(scene.GetModels()[i].path,
            selectedObject == i&& selected == ObjectType::ObjectTp,0, ImVec2(ImGui::GetWindowWidth() - 30,15)))
        {
            selectedObject = i;
            selected = ObjectType::ObjectTp;
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 15);
        if (ImGui::Button("-", ImVec2(15, 15))) {
            scene.DeleteObject(i);
            selected = ObjectType::None;
        }
        ImGui::PopID();
    }
    if (ImGui::Button("+")) {
        char const* fileTypes[2] = {"*.obj","*.fbx"};
        char const* filePath = tinyfd_openFileDialog("",NULL,2, fileTypes,NULL,0);
        scene.AppendObject(Model(filePath));
    }
    ImGui::End();
    //
    ImGui::Begin("PointLights");
    static int selectedPointLight=0;
    for (unsigned int i =0; i < scene.GetPointLights().size(); i++) {
        ImGui::PushID(i);
        if (ImGui::Selectable(("PointLight"+std::to_string(i)).c_str(), 
            selectedPointLight == i&& selected == ObjectType::PointLightTp,
            0, 
            ImVec2(ImGui::GetWindowWidth() - 30, 15))) {
            selectedPointLight = i;
            selected = ObjectType::PointLightTp;
        }
        ImGui::SameLine(ImGui::GetWindowWidth()-15);

        if (ImGui::Button("-", ImVec2(15, 15))) {
            scene.DeletePointLight(i);
            selected = ObjectType::None;
        }
        ImGui::PopID();
    }
    if(scene.GetPointLights().size() <= 4)
        if (ImGui::Button("+")) 
            scene.AppendPointLight(PointLight(glm::vec3(0.0f),glm::vec3(1.0f), glm::vec3(0.2f)));
    ImGui::End();
    //dirlight
    ImGui::Begin("Environment");
    if (ImGui::Selectable("Direct Light", selected == ObjectType::DirLightTp)) {
        selected = ObjectType::DirLightTp;
    }
    ImGui::End();
    //

    ImGui::Begin("Transform");
    switch (selected) {
    case ObjectType::None:
        break;
    case ObjectType::ObjectTp:
        DragAppend("position", scene.GetModels()[selectedObject].transform.pos);
        DragAppend("rotation", scene.GetModels()[selectedObject].transform.eulerRot);
        DragAppend("scale", scene.GetModels()[selectedObject].transform.scale);
        break;
    case ObjectType::PointLightTp:
        DragAppend("position", scene.GetPointLights()[selectedPointLight].transform.pos);
        DragAppend("scale", scene.GetPointLights()[selectedPointLight].transform.scale);
        ColorAppend("color", scene.GetPointLights()[selectedPointLight].color);
        DragAppend("clq", scene.GetPointLights()[selectedPointLight].clq, 0.001f);

        break;
    case ObjectType::DirLightTp:
        SliderAppend("direction", scene.GetDirLight().transform.eulerRot);
        ColorAppend("color", scene.GetDirLight().color);
        break;
    }
    ImGui::End();
}





