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
#include "model.h"
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
    //draw setup
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    Model model1("res/backpack/backpack.obj");
    BasicShader basicShader = BasicShader("shaders/basicShader.glsl");

    LightMesh lightMesh;
    Shader lightShader("shaders/lightShader.glsl");
    PointLight pointLight1(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f),1.0, 0.07f,0.017f);
    PointLight pointLight2(glm::vec3(-4.0f, 0.0f, -14.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0, 0.07f, 0.017f);

    DirectLight directLight(glm::vec3(-0.5f, -0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

    basicShader.use();
    basicShader.setDirLight(directLight);
    basicShader.appendPointLight(pointLight1);
    basicShader.appendPointLight(pointLight2);

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
        interface.NewFrame();
        interface.MainFrame();
        interface.MainFrame();
        //rendering
        glm::mat4 projection;
        projection = camera.GetProj();
        glm::mat4 view = camera.GetViewMat();

        //light
        glm::vec3 lpos1 = glm::vec3(glm::sin(glfwGetTime()) * 2, 0.0, glm::cos(glfwGetTime()) * 2);
        pointLight1.transform.setLocalPos(lpos1);
        lightShader.use();
        pointLight1.transform.setLocalScale(0.2);
        lightShader.setMat4("transform", pointLight1.transform.getTransformMatrix(projection, view));
        lightShader.setVec3("color", pointLight1.color);
        lightMesh.Draw(lightShader);

        pointLight2.transform.setLocalScale(0.2);
        lightShader.setMat4("transform", pointLight2.transform.getTransformMatrix(projection, view));
        lightShader.setVec3("color", pointLight2.color);
        lightMesh.Draw(lightShader);

        //mesh

        basicShader.use();
        //dynamic light
        basicShader.updatePointLight(pointLight1, 1);
        basicShader.updatePointLight(pointLight2, 2);

        basicShader.setVec3("viewPos", camera.GetPos());
        for (unsigned int i = 0; i < 5; i++)
        {
            model1.transform.setLocalPos(cubePositions[i]);
            model1.transform.setLocalRot(glm::vec3(1.0f, 0.3f, 0.5f));
            basicShader.setMat4("transform", model1.transform.getTransformMatrix(projection, view));
            basicShader.setMat4("model", model1.transform.getModelMatrix());
            model1.Draw(basicShader);
        }
        //IMGUI FINISH
        interface.Render();
        //call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    interface.Destroy();
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
        firstMouse = !firstMouse;
        std::cout << interfaceToggle << std::endl;
    }
        
}
