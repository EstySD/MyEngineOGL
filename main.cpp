#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "scene.h"
#include "light.h"
#include "interface.h"

void framebuffer_size_callback(GLFWwindow* window, int n_width, int n_height);
unsigned int width = 1280, height = 720;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float lastX = (float)width/2, lastY = (float)height/2;
void processInput(GLFWwindow* window);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;
Camera camera = Camera();
bool firstMouse = true;
bool interfaceToggle = true;
int main()
{
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);
    //callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    //gl enable
    glEnable(GL_DEPTH_TEST);
    //imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    Interface interface(window);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    //scene setup
    
    //draw setup

    Scene scene("scene/main.xml");


    camera.SetSpawn(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //input
        processInput(window);
        //camera
        camera.Update(deltaTime, (float)width, (float)height);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //IMGUI SETUP
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        //imgui frame
        ImGui::NewFrame();
        ImGui::Begin("Position");
        ImGui::End();
        interface.Framerate();
        //rendering

        //light
        scene.updateCamera(camera.GetPos(), camera.GetProj(), camera.GetViewMat());
        scene.Draw();
        ////mesh

        //IMGUI FINISH
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (interface.io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        //call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int n_width, int n_height)
{
    width = n_width;
    height = n_height;
    glViewport(0, 0, width, height);
    lastX = (float)width / 2;
    lastY = (float)height / 2;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (interfaceToggle) return;
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates range from bottom to top
    
    camera.Rotate((float)xoffset, (float)yoffset);
    lastX = (float)xpos;
    lastY = (float)ypos;

    
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (interfaceToggle) return;
    camera.Zoom((float)yoffset);
}
void processInput(GLFWwindow* window) ///all in game inputs
{
    if (interfaceToggle) return;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glm::vec4 wasd = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        wasd.x = 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        wasd.z = 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        wasd.y = 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        wasd.w = 1;
    camera.Move(wasd);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //all editor inputs
{
    if (glfwGetKey(window, GLFW_KEY_ENTER) && glfwGetKey(window, GLFW_KEY_LEFT_ALT) && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR,
            glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        interfaceToggle = !interfaceToggle;
        firstMouse = true;
    }
        
}
