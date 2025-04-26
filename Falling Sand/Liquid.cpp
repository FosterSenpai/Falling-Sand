// ============================================================================
// Project:     Falling Sand Simulation
// File:        Liquid.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1
// Description: Implementation file for the Liquid abstract class.
//              Contains common logic shared by all liquid elements,
//              including flow and evaporation behaviours.
// ============================================================================

#include "Liquid.h"
#include "World.h"       // Need World definition for interactions
#include "Gas.h"         // Needed for evaporation check & density comparison
#include "Particle.h"    // For ParticleType enum
#include <cstdlib>       // For rand()
#include <utility>       // For std::move (used by World methods)
#include <memory>        // For std::unique_ptr (used by World methods)

// **=== Protected Helper Methods ===**

/**
 * @brief Attempts to perform standard liquid flow logic (down, diagonal, horizontal).
 * Should be called by the derived class's update() method.
 * Interacts with the World's nextGrid state via World methods.
 * @param world Reference to the world grid and its methods.
 * @param r Current row.
 * @param c Current column.
 * @return true if the liquid successfully moved or swapped, false otherwise.
 */
bool Liquid::attemptFlow(World& world, int r, int c) {
    float selfDensity = this->getDensity();

    // --- Priority 1: Try Move/Swap Directly Below ---
    if (world.tryMoveOrSwap(r, c, r + 1, c)) {
        return true; // Moved/swapped down
    }

    // --- Priority 2: Try Move/Swap Diagonals Down ---
    int diag_dir = (rand() % 2 == 0) ? 1 : -1; // Randomize diagonal check order
    // Try preferred diagonal
    if (world.tryMoveOrSwap(r, c, r + 1, c + diag_dir)) {
        return true;
    }
    // Try other diagonal
    if (world.tryMoveOrSwap(r, c, r + 1, c - diag_dir)) {
        return true;
    }

    // --- Priority 3: Check Horizontal ---
    // Find ALL potentially valid horizontal moves first, then try the closest one.
    int dispersion = this->getDispersionRate();
    int best_h_move_c = c; // Target column, c means no move found yet
    int min_dist = dispersion + 1; // Distance to closest valid spot

    int horiz_dir = (rand() % 2 == 0) ? 1 : -1; // Randomize side check order

    for (int i = 0; i < 2; ++i) { // Check both L/R directions
        for (int step = 1; step <= dispersion; ++step) {
            int check_c = c + (horiz_dir * step);

            // Check bounds first
            if (!world.isWithinBounds(r, check_c)) {
                break; // Stop checking this direction if out of bounds
            }

            // Check if target spot in nextGrid is already claimed
            if (world.getElementFromNext(r, check_c)) {
                break; // Stop checking this direction (already claimed)
            }

            // Check if original spot was empty
            Element* originalTargetElement = world.getElement(r, check_c);
            if (!originalTargetElement) {
                // Found a potential empty spot! Is it the closest so far?
                if (step < min_dist) {
                    min_dist = step;
                    best_h_move_c = check_c;
                }
                // Even if found, continue checking closer spots on the other side
                // So, don't break the inner step loop here.
            }
            else {
                // Hit a non-empty original cell, stop checking further this direction
                break;
            }
        }
        // Optimization: If we found a move on the first side, we don't *need*
        // to check the other side for an equally close or closer spot,
        // as tryMoveOrSwap gives priority based on call order. But checking
        // both ensures we find the *absolute* closest if desired.
        // Let's keep checking both for now.
        horiz_dir *= -1; // Flip direction
    }

    // After checking both sides, if we found a valid target column:
    if (best_h_move_c != c) {
        // Attempt the move to the best (closest) spot found
        // tryMoveOrSwap will do the final atomic check on nextGrid availability
        if (world.tryMoveOrSwap(r, c, r, best_h_move_c)) {
            return true; // Moved horizontally
        }
    }

    // --- No Movement ---
    return false;
}
/**
 * @brief Attempts to evaporate the liquid based on temperature and conditions.
 * Checks temperature against boiling point, checks space above, and potentially
 * replaces this liquid element with its gas form via the World object's nextGrid.
 * Should be called by the derived class's update() method.
 * @param world Reference to the world grid and its methods.
 * @param r Current row.
 * @param c Current column.
 * @return true if evaporation occurred and element was replaced, false otherwise.
 */
bool Liquid::attemptEvaporation(World& world, int r, int c) {
    // 1. Check temperature
    if (this->getTemperature() < this->getBoilingPoint()) {
        return false; // Not hot enough
    }

    // 2. Check space above
    int above_r = r - 1;
    int above_c = c;
    Element* elementAbove = world.getElement(above_r, above_c); // Check current grid for space

    // Allow evaporation into empty space or existing gas
    // (Could refine this - maybe only if the gas above is the *same* gas type?)
    bool spaceAbove = !elementAbove || dynamic_cast<Gas*>(elementAbove);

    if (!spaceAbove) {
        return false; // Blocked above
    }

    // 3. Probability Check (Example: 5% chance per tick if conditions met)
    const int EVAPORATION_CHANCE = 5; // Percent
    if ((rand() % 100) >= EVAPORATION_CHANCE) {
        return false; // Didn't evaporate this tick
    }

    // 4. Conditions met - Evaporate!
    ParticleType gasType = this->getGasForm();
    if (gasType == ParticleType::EMPTY) { // Check if a valid gas form is defined
        return false; // Cannot evaporate if no gas form specified
    }

    // Create the new gas element using the world's factory
    std::unique_ptr<Element> newGasElement = world.createElementByType(gasType);

    if (newGasElement) {
        // Place the new gas element into the NEXT grid at the current position
        world.setNextElement(r, c, std::move(newGasElement));
        // Note: The original liquid unique_ptr in m_grid[r][c] will be implicitly
        // handled later - either moved over if stationary or discarded if source moved.
        // By placing the gas in m_nextGrid[r][c], we ensure it replaces the liquid.
        return true; // Evaporation successful
    }

    return false; // Failed to create gas element
}