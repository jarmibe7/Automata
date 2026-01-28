cmake_minimum_required(VERSION 3.16)
project(automata)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add GLAD
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

# Main executable
add_executable(automata
    src/main.cpp
    src/app/App.cpp
    src/render/Renderer.cpp
    src/render/Camera.cpp
    src/utils/Shader.cpp
    src/sim/Rules.cpp
    src/sim/Grid.cpp
)

target_include_directories(automata PRIVATE
    src
    extern/glm
    extern/glad/include
)

target_compile_definitions(automata PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(automata PRIVATE
    OpenGL::OpenGL
    glfw
    glad
)

# Copy shaders to build directory
add_custom_command(TARGET automata POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:automata>/shaders
)
