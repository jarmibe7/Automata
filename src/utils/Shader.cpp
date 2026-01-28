#include "Shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : program(0) {}

Shader::~Shader()
{
    if (program) {
        glDeleteProgram(program);
    }
}

std::string Shader::readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }

    return id;
}

bool Shader::compile(const std::string& vertPath, const std::string& fragPath)
{
    std::string vertSrc = readFile(vertPath);
    std::string fragSrc = readFile(fragPath);

    if (vertSrc.empty() || fragSrc.empty()) {
        return false;
    }

    unsigned int vert = compileShader(vertSrc, GL_VERTEX_SHADER);
    unsigned int frag = compileShader(fragSrc, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return true;
}

void Shader::use() const
{
    glUseProgram(program);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1,
                 glm::value_ptr(vec));
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
