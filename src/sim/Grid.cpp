// Create this file with Grid implementation

#include "Grid.hpp"
#include <algorithm>

Grid::Grid() : current(SIZE * SIZE * SIZE, Material::EMPTY), 
               next(SIZE * SIZE * SIZE, Material::EMPTY)
{
    // Add initial walls (floor and walls)
    for (int x = 0; x < SIZE; ++x) {
        for (int z = 0; z < SIZE; ++z) {
            set(x, 0, z, Material::WALL);
            set(x, SIZE - 1, z, Material::WALL);
        }
    }
    for (int y = 0; y < SIZE; ++y) {
        for (int z = 0; z < SIZE; ++z) {
            set(0, y, z, Material::WALL);
            set(SIZE - 1, y, z, Material::WALL);
        }
    }
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            set(x, y, 0, Material::WALL);
            set(x, y, SIZE - 1, Material::WALL);
        }
    }
}

Material Grid::get(int x, int y, int z) const
{
    if (!inBounds(x, y, z)) return Material::WALL;
    return current[index(x, y, z)];
}

void Grid::set(int x, int y, int z, Material m)
{
    if (!inBounds(x, y, z)) return;
    current[index(x, y, z)] = m;
}

void Grid::swapBuffers()
{
    std::swap(current, next);
}

void Grid::clear()
{
    std::fill(current.begin(), current.end(), Material::EMPTY);
    std::fill(next.begin(), next.end(), Material::EMPTY);
}

bool Grid::inBounds(int x, int y, int z) const
{
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE && z >= 0 && z < SIZE;
}

int Grid::index(int x, int y, int z) const
{
    return z * SIZE * SIZE + y * SIZE + x;
}
