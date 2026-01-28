#include "Renderer.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

// Constructor and destructor
Renderer::Renderer() : cubeVAO(0), cubeVBO(0), instanceVAO(0),
                       instancePosVBO(0), instanceColorVBO(0) {}

Renderer::~Renderer()
{
    if (cubeVAO) glDeleteVertexArrays(1, &cubeVAO);
    if (cubeVBO) glDeleteBuffers(1, &cubeVBO);
    if (instanceVAO) glDeleteVertexArrays(1, &instanceVAO);
    if (instancePosVBO) glDeleteBuffers(1, &instancePosVBO);
    if (instanceColorVBO) glDeleteBuffers(1, &instanceColorVBO);
}

// Initialize shaders and voxel render grid
bool Renderer::initialize()
{
    if (!shader.compile("shaders/voxel.vert", "shaders/voxel.frag")) {
        return false;
    }

    setupCube();
    return true;
}

// Setup voxel render grid
void Renderer::setupCube()
{
    // Unit cube vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 6, 5, 6, 4, 7,
        0, 4, 5, 5, 1, 0,
        2, 6, 7, 7, 3, 2,
        0, 3, 7, 7, 4, 0,
        1, 5, 6, 6, 2, 1
    };

    unsigned int EBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instancePosVBO);
    glGenBuffers(1, &instanceColorVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Position attribute
    glBindBuffer(GL_ARRAY_BUFFER, instancePosVBO);
    glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    // Color attribute
    glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
    glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Update OpenGL buffers
void Renderer::updateInstanceData(const Grid& grid)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;

    const auto& buffer = grid.getCurrentBuffer();
    for (int z = 1; z < Grid::SIZE - 1; ++z) {
        for (int y = 1; y < Grid::SIZE - 1; ++y) {
            for (int x = 1; x < Grid::SIZE - 1; ++x) {
                Material m = buffer[grid.index(x, y, z)];
                if (m != Material::EMPTY && m != Material::WALL) {
                    positions.push_back(glm::vec3(x, y, z));
                    colors.push_back(getMaterialInfo(m).color);
                }
            }
        }
    }

    glBindBuffer(GL_COPY_WRITE_BUFFER, instancePosVBO);
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());

    glBindBuffer(GL_COPY_WRITE_BUFFER, instanceColorVBO);
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, colors.size() * sizeof(glm::vec3), colors.data());
}

// Render the voxel environment to screen
void Renderer::render(const Grid& grid, const Camera& camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    shader.use();
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("model", glm::mat4(1.0f));

    updateInstanceData(grid);

    const auto& buffer = grid.getCurrentBuffer();
    int count = 0;
    for (Material m : buffer) {
        if (m != Material::EMPTY && m != Material::WALL) count++;
    }

    if (count > 0) {
        glBindVertexArray(cubeVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(0);
    }
}

// Reshape window
void Renderer::reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}
