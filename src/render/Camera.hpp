#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    /// \brief Class for 3D rendering camera
    Camera();

    /// \brief The view matrix describes the camera's position in the world frame
    glm::mat4 getViewMatrix() const;

    /// \brief The projection matrix maps the 3D camera space to the 2D screen
    glm::mat4 getProjectionMatrix() const;

    /// \brief Return the camera's position
    glm::vec3 getPosition() const;

    /// \brief Rotate the camera view
    /// \param deltaX Rotation about the yaw axis
    /// \param deltaY Rotation about the pitch axis
    void rotate(float deltaX, float deltaY);

    /// \brief Zoom the camera
    /// \param zoom How much to zoom in
    void zoom(float delta);

    /// \brief Pan the camera
    /// \param delta How much to move the camera in xyz
    void pan(const glm::vec3& delta);

    /// \brief Set the camera aspect ratio
    /// \param aspect The new aspect ratio
    void setAspectRatio(float aspect);

private:
    glm::vec3 position;                 // Camera's position
    glm::vec3 target;                   // Camera normal vector
    glm::vec3 up;                       // Camera up vector
    float yaw;                          // Camera rotation
    float pitch;                        
    float distance;                     // Camera zoom, fov, aspect
    float fov;
    float aspectRatio;

    // Update camera position
    void updatePosition();
};
