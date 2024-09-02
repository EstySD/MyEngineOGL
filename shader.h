#include <glad/glad.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "light.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef SHADER_H_
#define SHADER_H_
struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};
class Shader {
private:
    unsigned int id;
public:
    const char* filepath;
    Shader(const char* filepath);
    Shader():id(-1), filepath("-1") {};
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 trans) const;
    void setVec3(const std::string& name, glm::vec3 vec) const;
    
protected:
    ShaderSource ParseShader();
    unsigned int CompileShader(std::string ShaderSource, unsigned int GLShaderType);
};
class BasicShader:public Shader {
public:
    using Shader::Shader;
    void setDirLight(DirectLight dirLight);
    void updatePointLight(PointLight pointLight, int num);
    void setSpotLight(SpotLight spotLight);
private:
    int pointLightCount=0;
};

#endif