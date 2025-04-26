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
    int r_below = r + 1;
    int c_below = c;

    Element* self = world.getElement(r, c); // Get self
    if (!self) return false;

    // 1. Bounds check below
    if (!world.isWithinBounds(r_below, c_below)) { return false; }

    Element* element_below = world.getElement(r_below, c_below); // What's below?

    // --- Priority 1: Try Move/Swap Directly Below ---
    if (world.tryMoveOrSwap(r, c, r_below, c_below)) {
        return true; // Moved/swapped down
    }

    // --- IF Downward Move Failed ---

    // --- NEW: Special Check for Sand blocked vertically by Water ---
    // (Even though logs show swap happens, the *effect* is blockage due to cascade)
    // If the direct move failed, AND we are sand, AND water is below: TRY HORIZONTAL first.
    bool tried_horizontal = false;
    if (self->getType() == ParticleType::SAND && element_below && element_below->getType() == ParticleType::WATER) {
        tried_horizontal = true; // Mark that we tried this special path
        int h_dir = (rand() % 2 == 0) ? 1 : -1; // Randomize L/R

        // Try moving horizontally LEFT/RIGHT into EMPTY or WATER
        if (world.tryMoveOrSwap(r, c, r, c + h_dir)) { // Try first horizontal dir
            return true;
        }
        if (world.tryMoveOrSwap(r, c, r, c - h_dir)) { // Try other horizontal dir
            return true;
        }
        // If horizontal move failed, continue to diagonal below
    }

    // --- Priority 2/3: Try Move/Swap Diagonals Down ---
    // (Only if downward failed, and potentially after horizontal check for Sand-on-Water)
    if (this->canSlideDiagonally()) {
        int diag_dir = (rand() % 2 == 0) ? 1 : -1;

        if (world.tryMoveOrSwap(r, c, r_below, c + diag_dir)) { // Try first diagonal dir
            return true;
        }
        if (world.tryMoveOrSwap(r, c, r_below, c - diag_dir)) { // Try other diagonal dir
            return true;
        }
    }

    // --- No Movement ---
    return false;
}