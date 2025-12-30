#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;

    Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = std::string());
    ~Shader();
    // 使用/激活程序
    void use();
    // set uniform
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat4(const std::string& name, float u, float v, float w, float x) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, float u, float v, float w) const;
    void setVec3(const std::string& name, glm::vec3 v) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

