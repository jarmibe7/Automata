#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../sim/Grid.hpp"
#include "../render/Renderer.hpp"
#include "../render/Camera.hpp"
#include <memory>

class App
{
public:
    /// \brief Application for running simulator, constructor and destructor
    App();
    ~App();

    /// \brief Initialize simulation and rendering
    bool initialize();

    /// \brief Run the main simulation loop
    void run();

private:       
    GLFWwindow* window;                     // GLFW interactable window
    std::unique_ptr<Grid> grid;             // Voxel grid
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;

    int windowWidth;                        // Window params
    int windowHeight;
    bool running;
    bool paused;

    double lastMouseX, lastMouseY;          // Mouse control
    bool mousePressed;

    void handleInput();
    void update(float deltaTime);
    void render();

    // GLFW callbacks
    static void mouseButtonCallback(GLFWwindow* w, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* w, double x, double y);
    static void scrollCallback(GLFWwindow* w, double xOffset, double yOffset);
    static void framebufferSizeCallback(GLFWwindow* w, int width, int height);

    Material currentBrush = Material::SAND;
    glm::vec3 screenToWorldRay(double mouseX, double mouseY);
    bool raycastGrid(const glm::vec3& rayOrigin, const glm::vec3& rayDir, glm::ivec3& hitCell);
    void placeMaterial(double mouseX, double mouseY);
};
