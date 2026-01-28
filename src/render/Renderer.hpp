#pragma once

#include <glad/glad.h>
#include "../sim/Grid.hpp"
#include "Camera.hpp"
#include "../utils/Shader.hpp"
#include <vector>
#include <glm/glm.hpp>

class Renderer
{
public:
    /// \brief 3D voxel rendering
    Renderer();
    ~Renderer();

    /// \brief Initialize voxel render grid and OpenGL buffers/settings
    bool initialize();

    /// \brief Render voxel environment
    /// \param grid Voxel grid
    /// \param camera Camera to render from
    void render(const Grid& grid, const Camera& camera);

    /// \brief Reshape the render to fit new window size
    /// \param width New window width
    /// \param height New window height
    void reshape(int width, int height);

private:
    // OpenGL variables
    unsigned int cubeVAO, cubeVBO;
    unsigned int instanceVAO, instancePosVBO, instanceColorVBO;
    Shader shader;

    // Set up voxel grid
    void setupCube();

    // Update render buffers
    void updateInstanceData(const Grid& grid);
};
