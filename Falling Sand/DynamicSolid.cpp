// ============================================================================
// Project:     Falling Sand Simulation
// File:        DynamicSolid.cpp
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25
// Version:     1.5
// Description: Implementation file for the DynamicSolid abstract class.
//              Contains common logic shared by dynamic solid elements,
//              primarily the gravity-driven falling behaviour.
// ============================================================================

#include "DynamicSolid.h"
#include "World.h"       // Need World definition for interactions
#include "Liquid.h"      // Need Liquid definition for type checking/casting
#include "Gas.h"         // Need Gas definition for type checking/casting (and density later)
#include "Particle.h"    // For ParticleType::EMPTY
#include <cstdlib>       // For rand()
#include <memory>        // For std::unique_ptr comparisons if needed
#include <utility>       // For std::move if transferring ownership

// **=== Protected Helper Methods ===**

/**
 * @brief Attempts to perform standard dynamic solid falling logic.
 * Checks below, then potentially diagonally below (if canSlideDiagonally() is true),
 * attempting to move into empty space or displace lighter liquids/gases.
 * Interacts with the World's nextGrid state.
 * @param world Reference to the world grid and its methods.
 * @param r Current row.
 * @param c Current column.
 * @return true if the solid successfully moved or swapped, false otherwise.
 */
bool DynamicSolid::attemptFall(World& world, int r, int c) {

    // --- Priority 1: Try Move/Swap Directly Below ---
    if (world.tryMoveOrSwap(r, c, r + 1, c)) {
        return true; // Fell/Swapped straight down
    }

    // --- Priority 2: Try Move/Swap Diagonally (if allowed) ---
    // Only try this if falling straight down failed.
    if (!this->canSlideDiagonally()) {
        return false; // Sliding not allowed for this element type
    }

    int dir = (rand() % 2 == 0) ? 1 : -1; // Randomize check order

    // Try preferred diagonal
    if (world.tryMoveOrSwap(r, c, r + 1, c + dir)) {
        return true;
    }

    // Try other diagonal
    if (world.tryMoveOrSwap(r, c, r + 1, c - dir)) {
        return true;
    }

    // --- No Movement ---
    return false;
}