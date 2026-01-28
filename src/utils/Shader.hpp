#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    /// \brief A Shader object made to hold an OpenGL shader
    Shader();
    ~Shader();

    /// \brief Compile the Shader object given a vertex and fragment shader
    /// \param vertPath Path to the vertex shader
    /// \param fragPath Path to the fragment shader
    bool compile(const std::string& vertPath, const std::string& fragPath);

    /// \brief Activate shader
    void use() const;

    /// \brief Utility function for setting a Mat4 object
    /// \param name Name of the Mat4 object
    /// \param mat Value to set it to
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    /// \brief Utility function for setting a Vec3 object
    /// \param name Name of the Vec3 object
    /// \param vec Value to set it to
    void setVec3(const std::string& name, const glm::vec3& vec) const;

    /// \brief Utility function for setting an Int object
    /// \param name Name of the Int object
    /// \param value Value to set it to
    void setInt(const std::string& name, int value) const;

    /// \brief Get the Shader program
    unsigned int getProgram() const { return program; }

private:
    unsigned int program;   // Shader program

    // Helper functions for reading files and shader compilation
    std::string readFile(const std::string& path);
    unsigned int compileShader(const std::string& source, unsigned int type);
};
