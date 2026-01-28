#pragma once

#include "Materials.hpp"
#include <vector>
#include <glm/glm.hpp>

class Grid
{
public:
    static constexpr int SIZE = 64;

    /// \brief Cubic voxel render grid
    Grid();

    /// \brief Get the material at a given coordinate
    /// \param x X-coord
    /// \param Y Y-coord
    /// \param Z Z-coord
    Material get(int x, int y, int z) const;

    /// \brief Set the material at a given coordinate
    /// \param x X-coord
    /// \param Y Y-coord
    /// \param Z Z-coord
    void set(int x, int y, int z, Material m);

    /// \brief Update the new current buffer for cellular automata rule calculation from the previous
    void swapBuffers();

    /// \brief Get the current state buffer
    const std::vector<Material>& getCurrentBuffer() const { return current; }

    /// \brief Get the next state buffer
    std::vector<Material>& getNextBuffer() { return next; }

    /// \brief Clear all buffers
    void clear();

    /// \brief Check if point is in bounds
    bool inBounds(int x, int y, int z) const;

    /// \brief Get a point's index
    int index(int x, int y, int z) const;

private:
    // Current and next state buffers
    std::vector<Material> current;
    std::vector<Material> next;
};
