#include "Rules.hpp"
#include <random>

namespace {
    std::mt19937 rng(42);
}

void Rules::update(Grid& grid)
{
    // Copy current to next
    grid.getNextBuffer() = grid.getCurrentBuffer();

    // Iterate in deterministic order: z -> y -> x
    for (int z = 0; z < Grid::SIZE; ++z) {
        for (int y = Grid::SIZE - 1; y >= 0; --y) {
            for (int x = 0; x < Grid::SIZE; ++x) {
                Material m = grid.get(x, y, z);

                if (m == Material::SAND) {
                    updateSand(grid, x, y, z);
                } else if (m == Material::WATER) {
                    updateWater(grid, x, y, z);
                }
                else {
                    updateGOL(m, grid, x, y, z);
                }
            }
        }
    }

    grid.swapBuffers();
}

void Rules::updateSand(Grid& grid, int x, int y, int z)
{
    if (y == 0) return;

    Material below = grid.get(x, y - 1, z);
    if (below == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y - 1, z)] = Material::SAND;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
        return;
    }

    // Try diagonal slides
    std::uniform_int_distribution<int> dist(0, 3);
    int dir = dist(rng);

    if (dir == 0 && grid.get(x + 1, y - 1, z) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x + 1, y - 1, z)] = Material::SAND;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 1 && grid.get(x - 1, y - 1, z) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x - 1, y - 1, z)] = Material::SAND;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 2 && grid.get(x, y - 1, z + 1) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y - 1, z + 1)] = Material::SAND;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 3 && grid.get(x, y - 1, z - 1) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y - 1, z - 1)] = Material::SAND;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    }
}

void Rules::updateWater(Grid& grid, int x, int y, int z)
{
    if (y == 0) return;

    Material below = grid.get(x, y - 1, z);
    
    // Always try to fall first
    if (below == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y - 1, z)] = Material::WATER;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
        return;
    }

    // Only flow laterally if blocked below
    std::uniform_int_distribution<int> dist(0, 3);
    int dir = dist(rng);

    if (dir == 0 && grid.get(x + 1, y, z) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x + 1, y, z)] = Material::WATER;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 1 && grid.get(x - 1, y, z) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x - 1, y, z)] = Material::WATER;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 2 && grid.get(x, y, z + 1) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y, z + 1)] = Material::WATER;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    } else if (dir == 3 && grid.get(x, y, z - 1) == Material::EMPTY) {
        grid.getNextBuffer()[grid.index(x, y, z - 1)] = Material::WATER;
        grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
    }
}

void Rules::updateGOL(Material m, Grid& grid, int x, int y, int z)
{
    // Count neighbors
    int count = 0;
    for (int dz = -1; dz <= 1; ++dz)
    for (int dy = -1; dy <= 1; ++dy)
    for (int dx = -1; dx <= 1; ++dx)
    {
        if (dx == 0 && dy == 0 && dz == 0) continue;

        int nx = x + dx;
        int ny = y + dy;
        int nz = z + dz;

        if (grid.inBounds(nx, ny, nz) &&
            grid.get(nx, ny, nz) == Material::GOL)
        {
            count++;
        }
    }

    if (m == Material::GOL) {
        if (count < 5 || count > 7) {
            grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
        }
        // else survives (already copied)
    }
    else if (m == Material::EMPTY) {
        if (count == 6) {
            grid.getNextBuffer()[grid.index(x, y, z)] = Material::GOL;
        }
    }

    

}