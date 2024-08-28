
#include "shader.h"

    Shader::Shader(const char* filepath) {

        m_filepath = filepath;
        ShaderSource shadersource = ParseShader();
        unsigned int vertexShader = CompileShader(shadersource.VertexSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = CompileShader(shadersource.FragmentSource, GL_FRAGMENT_SHADER);
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        int  success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAMLINK\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    ShaderSource Shader::ParseShader()
    {
        std::ifstream stream(m_filepath);
        if (stream.is_open() == false) std::cout << "couldn't find shader: " << m_filepath << std::endl;
        std::string line;
        std::stringstream ss[2];
        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;

            }
            else {
                ss[(int)type] << line << '\n';
            }
        }
        return { ss[0].str(), ss[1].str() };
    }
    unsigned int Shader::CompileShader(std::string ShaderSource, unsigned int GLShaderType) {
        const char* source = ShaderSource.c_str();
        unsigned int shader = glCreateShader(GLShaderType);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        int  success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return shader;
    }
    void Shader::use() {
        glUseProgram(id);
    }
    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    void Shader::setMat4(const std::string& name, glm::mat4 value) const{
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
            1, GL_FALSE, glm::value_ptr(value));
    }
    void Shader::setVec3(const std::string& name, glm::vec3 value) const{
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }
////////// Basic Shader
 void BasicShader::setDirLight(DirectLight dirLight) {
        setVec3("dirLight.direction", dirLight.transform.getRot());
        setVec3("dirLight.color", dirLight.color);
    }

    void BasicShader::updatePointLight(PointLight pointLight, int num) {
        pointLightCount = glm::max(pointLightCount, num);
        setInt("pointLightsCount", pointLightCount+1);
        std::string count = std::to_string(num);
        setVec3("pointLights[" + count + "].position", pointLight.transform.getPos());
        setVec3("pointLights[" + count + "].color", pointLight.color);
        setFloat("pointLights[" + count + "].constant", pointLight.constant);
        setFloat("pointLights[" + count + "].linear", pointLight.linear);
        setFloat("pointLights[" + count + "].quadratic", pointLight.quadratic);
    }
    void BasicShader::setSpotLight(SpotLight spotLight) {
        use();
    }