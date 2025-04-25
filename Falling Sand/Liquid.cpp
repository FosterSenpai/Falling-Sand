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

    // No need to get selfDensity here anymore, tryMoveOrSwap handles comparisons internally

    // --- Priority 1: Try Move/Swap Directly Below ---
    int next_r = r + 1;
    int next_c = c;
    if (world.tryMoveOrSwap(r, c, next_r, next_c)) {
        return true; // Moved/swapped down
    }

    // --- Priority 2: Try Move/Swap Diagonals Down (Randomized Order) ---
    int dir = (rand() % 2 == 0) ? 1 : -1; // Randomly check left (-1) or right (+1) first

    for (int i = 0; i < 2; ++i) { // Check both diagonals (dir, then -dir)
        next_r = r + 1;
        next_c = c + dir;
        // Only attempt move/swap into empty diagonal cells for liquids (simpler)
        // Check original grid's diagonal target - if it was empty, try moving there
        // Note: tryMoveOrSwap currently checks original target internally
        if (world.tryMoveOrSwap(r, c, next_r, next_c)) {
            // We might want to refine tryMoveOrSwap later to only allow liquids
            // to move diagonally into EMPTY cells, not displace fluids diagonally.
            // For now, this uses the standard tryMoveOrSwap.
            return true; // Moved/swapped diagonally
        }
        dir *= -1; // Flip direction
    }

    // --- Priority 3: Try Move Horizontally (Using Dispersion Rate) ---
    int dispersion = this->getDispersionRate();
    dir = (rand() % 2 == 0) ? 1 : -1; // Randomize horizontal check order

    for (int i = 0; i < 2; ++i) { // Check both directions (dir, then -dir)
        for (int step = 1; step <= dispersion; ++step) {
            next_r = r; // Stay on same row
            next_c = c + (dir * step);

            // Check if the original cell we are checking was empty before trying move
            // This prevents "skipping" over obstacles more effectively
            Element* originalTargetElement = world.getElement(next_r, next_c);
            if (!originalTargetElement) { // Only try to move if original target was empty
                if (world.tryMoveOrSwap(r, c, next_r, next_c)) {
                    return true; // Moved horizontally
                }
                else {
                    // If tryMoveOrSwap failed even though original was empty,
                    // it means the spot in nextGrid was already claimed this tick.
                    // Stop checking further in this direction.
                    break;
                }
            }
            else {
                // Original target wasn't empty, stop checking this direction
                break;
            }
        }
        dir *= -1; // Flip direction
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