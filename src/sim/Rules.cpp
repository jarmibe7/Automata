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
    // Get adjacent cells
    auto full_below = grid.get(x, y - 1, z) == Material::EMPTY;
    auto full_above = grid.get(x, y + 1, z) == Material::EMPTY;
    auto full_back = grid.get(x - 1, y, z) == Material::EMPTY;
    auto full_forward = grid.get(x + 1, y, z) == Material::EMPTY;

    int count = 0;
    if (full_below) count++;
    if (full_above) count++;
    if (full_back) count++;
    if (full_forward) count++;

    if (m == Material::GOL)
    {
        // Underpopulation and overpopulation
        if (count < 2){
            grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
        }
        else if (count > 3) {
            grid.getNextBuffer()[grid.index(x, y, z)] = Material::EMPTY;
        }
        // Else, continue to be alive
    }
    else {
        if (count == 3) {
            grid.getNextBuffer()[grid.index(x, y, z)] = Material::GOL;;
        }
        // Else, continue to be empty
    }
    

}