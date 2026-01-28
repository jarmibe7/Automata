#pragma once

#include <cstdint>
#include <glm/glm.hpp>

/// \brief Enumerate all material types
enum class Material : uint8_t
{
    EMPTY = 0,
    SAND,
    WATER,
    GOL,
    WALL,
    COUNT
};

/// \brief Struct to hold a material's properties
struct MaterialInfo
{
    glm::vec3 color;
    bool gravity;
    bool fluid;
};

/// \brief Getter to access a material's properties
constexpr MaterialInfo getMaterialInfo(Material m)
{
    switch (m) {
        case Material::SAND:
            return {{0.9f, 0.8f, 0.3f}, true, false};
        case Material::WATER:
            return {{0.2f, 0.6f, 1.0f}, true, true};
        case Material::GOL:
            return {{0.0f, 1.0f, 0.0f}, false, false};
        case Material::WALL:
            return {{0.5f, 0.5f, 0.5f}, false, false};
        case Material::EMPTY:
        default:
            return {{0.0f, 0.0f, 0.0f}, false, false};
    }
}
