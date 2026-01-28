#pragma once

#include "Grid.hpp"

class Rules
{
public:
    /// \brief Function that updates all materials in the grid according to their respective rules
    static void update(Grid& grid);

private:
    // Update functions for each material
    static void updateSand(Grid& grid, int x, int y, int z);
    static void updateWater(Grid& grid, int x, int y, int z);
    static void updateEmpty(Grid& grid, int x, int y, int z);
    static void updateGOL(Material m, Grid& grid, int x, int y, int z);
};
