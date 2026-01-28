#include "App.hpp"
#include "../sim/Rules.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <random>

namespace {
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> prob(0.0f, 1.0f);

}

static App* g_app = nullptr;

// Callback for mouse button pressed
void App::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
{
    if (g_app && button == GLFW_MOUSE_BUTTON_LEFT) {
        g_app->mousePressed = (action == GLFW_PRESS);
    }

    if (g_app && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(w, &mx, &my);
        g_app->placeMaterial(mx, my);
    }
}

// Callback for cursor tracking
void App::cursorPosCallback(GLFWwindow* w, double x, double y)
{
    if (g_app && g_app->mousePressed) {
        double deltaX = x - g_app->lastMouseX;
        double deltaY = y - g_app->lastMouseY;
        g_app->camera->rotate((float)deltaX, (float)deltaY);
    }
    if (g_app) {
        g_app->lastMouseX = x;
        g_app->lastMouseY = y;
    }
}

// Callback for zooming with scroll
void App::scrollCallback(GLFWwindow* w, double xOffset, double yOffset)
{
    if (g_app) {
        g_app->camera->zoom((float)yOffset);
    }
}

// Callback for window resizing
void App::framebufferSizeCallback(GLFWwindow* w, int width, int height)
{
    if (g_app) {
        g_app->windowWidth = width;
        g_app->windowHeight = height;
        if (g_app->renderer) {
            g_app->renderer->reshape(width, height);
        }
        if (g_app->camera) {
            g_app->camera->setAspectRatio((float)width / (float)height);
        }
    }
}

// App constructor and destructor
App::App()
    : window(nullptr), windowWidth(1200), windowHeight(800), running(false),
      paused(false), lastMouseX(0), lastMouseY(0), mousePressed(false)
{
    g_app = this;
}

App::~App()
{
    if (window) {
        glfwDestroyWindow(window);  // Kill window when killed
    }
    glfwTerminate();
    g_app = nullptr;
}

// Initialize simulation and rendering
bool App::initialize()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Automata", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Create GLWF context and initialize callbacks
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load OpenGL functions" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create grid and renderer
    grid = std::make_unique<Grid>();
    renderer = std::make_unique<Renderer>();
    camera = std::make_unique<Camera>();

    if (!renderer->initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    camera->setAspectRatio((float)windowWidth / (float)windowHeight);

    // // Water pool
    // for (int x = 5; x < 59; ++x) {
    //     for (int z = 5; z < 59; ++z) {
    //         for (int y = 2; y < 25; ++y) {
    //             grid->set(x, y, z, Material::WATER);
    //         }
    //     }
    // }

    // // Sand pile
    // for (int i = 15; i < 49; ++i) {
    //     for (int j = 15; j < 49; ++j) {
    //         for (int k = 35; k < 55; ++k) {
    //             grid->set(i, k, j, Material::SAND);
    //         }
    //     }
    // }

    // GOL random
    // for (int x = 24; x < 40; ++x) {
    //     for (int y = 32; y < 48; ++y) {
    //         for (int z = 24; z < 40; ++z) {
    //             if (prob(rng) < 0.38f) {  // ← IMPORTANT
    //                 grid->set(x, y, z, Material::GOL);
    //             }
    //         }
    //     }
    // }

    // // GOL cube
    // for (int x = 26; x < 38; ++x)
    // for (int y = 34; y < 46; ++y)
    // for (int z = 26; z < 38; ++z)
    // {
    //     grid->set(x, y, z, Material::GOL);
    // }

    // // Permanent GOL cube
    // int cx = 32, cy = 40, cz = 32;

    // for (int dx = 0; dx < 2; ++dx)
    // for (int dy = 0; dy < 2; ++dy)
    // for (int dz = 0; dz < 2; ++dz)
    // {
    //     grid->set(cx + dx, cy + dy, cz + dz, Material::GOL);
    // }


    // running = true;
    // paused = false;
    // return true;
}

// Handle inputs from mouse/keyboard
void App::handleInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        grid->clear();
    }
    
    // Pause toggle
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spacePressed) {
            paused = !paused;
            spacePressed = true;
        }
    } else {
        spacePressed = false;
    }

    // Camera flying controls
    float panSpeed = 0.5f;
    glm::vec3 panDelta(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        panDelta.y += panSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        panDelta.y -= panSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        panDelta.x -= panSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        panDelta.x += panSpeed;
    }

    if (glm::length(panDelta) > 0.0f) {
        camera->pan(panDelta);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        currentBrush = Material::SAND;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        currentBrush = Material::WATER;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        currentBrush = Material::WALL;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        currentBrush = Material::GOL;
}

// Update material rules
void App::update(float deltaTime)
{
    if (!paused) {
        Rules::update(*grid);
    }
}

void App::render()
{
    renderer->render(*grid, *camera);
    glfwSwapBuffers(window);
}

// Simulation runtime
void App::run()
{
    double lastTime = glfwGetTime();
    const float fixedDeltaTime = 1.0f / 20.0f;  // Simulation timestep
    float accumulator = 0.0f;

    while (running && !glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        if (deltaTime > 0.1f) deltaTime = 0.1f;
        accumulator += deltaTime;

        while (accumulator >= fixedDeltaTime) {
            handleInput();
            update(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        render();
        glfwPollEvents();
    }
}

glm::vec3 App::screenToWorldRay(double mouseX, double mouseY)
{
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
    float z = 1.0f;

    glm::vec3 ray_nds(x, y, z);
    glm::vec4 ray_clip(ray_nds, 1.0f);

    glm::vec4 ray_eye = glm::inverse(camera->getProjectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec3 ray_world =
        glm::normalize(glm::vec3(glm::inverse(camera->getViewMatrix()) * ray_eye));

    return ray_world;
}

bool App::raycastGrid(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    glm::ivec3& hitCell
) {
    const float maxDist = 100.0f;
    const float step = 0.1f;

    glm::vec3 pos = rayOrigin;

    for (float t = 0; t < maxDist; t += step) {
        pos += rayDir * step;

        int x = (int)std::floor(pos.x);
        int y = (int)std::floor(pos.y);
        int z = (int)std::floor(pos.z);

        if (!grid->inBounds(x, y, z))
            continue;

        if (grid->get(x, y, z) != Material::EMPTY) {
            hitCell = {x, y, z};
            return true;
        }
    }
    return false;
}

void App::placeMaterial(double mouseX, double mouseY)
{
    glm::vec3 rayDir = screenToWorldRay(mouseX, mouseY);
    glm::vec3 rayOrigin = camera->getPosition();

    glm::ivec3 hit;
    if (raycastGrid(rayOrigin, rayDir, hit)) {
        // Place material ABOVE the hit surface
        int px = hit.x;
        int py = hit.y + 1;
        int pz = hit.z;

        if (grid->inBounds(px, py, pz)) {
            grid->set(px, py, pz, currentBrush);
        }
    }
}