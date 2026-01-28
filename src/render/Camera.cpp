#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

// Consntructor
Camera::Camera()
    : target(32.0f, 32.0f, 32.0f), up(0.0f, 1.0f, 0.0f), yaw(45.0f),
      pitch(35.0f), distance(100.0f), fov(45.0f), aspectRatio(16.0f / 9.0f)
{
    updatePosition();
}

// Manually set camera pose
void Camera::updatePosition()
{
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    position.x = target.x + distance * cos(radYaw) * cos(radPitch);
    position.y = target.y + distance * sin(radPitch);
    position.z = target.z + distance * sin(radYaw) * cos(radPitch);
}

// Get camera transform in world
glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

// Get projection to screen plane
glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

// Get camera's position
glm::vec3 Camera::getPosition() const
{
    return position;
}

// Rotate camera
void Camera::rotate(float deltaX, float deltaY)
{
    yaw += deltaX * 0.5f;
    pitch += deltaY * 0.5f;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updatePosition();
}

// Zoom camera
void Camera::zoom(float delta)
{
    distance -= delta * 2.0f;
    if (distance < 10.0f) distance = 10.0f;         // Cap zoom (min and max)
    if (distance > 200.0f) distance = 200.0f;
    updatePosition();
}

// Pan camera
void Camera::pan(const glm::vec3& delta)
{
    target += delta;
}

// Manually set camera aspect ratio
void Camera::setAspectRatio(float aspect)
{
    aspectRatio = aspect;
}
