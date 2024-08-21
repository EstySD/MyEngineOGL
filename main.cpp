
#include <iostream>
#include <vector>
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

void framebuffer_size_callback(GLFWwindow* window, int n_width, int n_height);
unsigned int width = 1280, height = 720;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float lastX = (float)width/2, lastY = (float)height/2;
void processInput(GLFWwindow* window);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;
Camera camera = Camera();
bool firstMouse = true;
int main()
{
    
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //gl enable
    glEnable(GL_DEPTH_TEST);
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

    Model lightModel("res/light_sphere.fbx");
    Shader lightShader("shaders/lightShader.glsl");
    PointLight pointLight1(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f),1.0, 0.07f,0.017f);
    PointLight pointLight2(glm::vec3(-4.0f, 0.0f, -14.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0, 0.07f, 0.017f);

    DirectLight directLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f));

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
        //rendering

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection;
        projection = camera.GetProj();
        glm::mat4 view = camera.GetViewMat();

        //light
        lightShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        pointLight1.position = glm::vec3(2*glm::sin(glfwGetTime()),0.0,0.0) + glm::vec3(0.0,0.0, 2*glm::cos(glfwGetTime()));
        model = glm::translate(model, pointLight1.position);
        model = glm::scale(model, glm::vec3(0.2f));
        glm::mat4 transform(1.0f);
        transform = projection * view * model * transform;
        lightShader.setMat4("transform", transform);
        lightShader.setVec3("color", pointLight1.color);
        lightModel.Draw(lightShader);

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, pointLight2.position);
        model = glm::scale(model, glm::vec3(0.2f));
        transform = glm::mat4(1.0f);
        transform = projection * view * model * transform;
        lightShader.setMat4("transform", transform);
        lightShader.setVec3("color", pointLight2.color);
        lightModel.Draw(lightShader);

        //mesh

        basicShader.use();
        basicShader.updatePointLight(pointLight1, 1);
        basicShader.setVec3("viewPos", camera.GetPos());
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glm::mat4 transform(1.0f);
            transform = projection * view *model*transform;
            basicShader.setMat4("transform", transform);
            basicShader.setMat4("model", model);
            model1.Draw(basicShader);
        }
        
        //call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
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
    camera.Zoom((float)yoffset);
}
void processInput(GLFWwindow* window)
{
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
