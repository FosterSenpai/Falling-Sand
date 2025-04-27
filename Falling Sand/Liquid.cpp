// ============================================================================
// Project:     Falling Sand Simulation
// File:        Liquid.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.4
// Description: Implementation file for the Liquid abstract class.
//              Contains common logic shared by all liquid elements,
//              including flow and evaporation behaviours.
// ============================================================================

#include "Liquid.h"
#include "World.h"
#include "Gas.h"
#include "Particle.h"
#include <cstdlib>
#include <utility>
#include <memory>
#include "Solid.h"

// **=== Protected Helper Methods ===**

bool Liquid::attemptFlow(World& world, int r, int c) {

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

    // --- Priority 3: Check Horizontal (Yielding to Denser Falling) ---
    int dispersion = this->getDispersionRate();
    int best_h_move_c = c;      // Target column, c means no move found yet
    int min_dist = dispersion + 1; // Distance to closest valid spot

    int horiz_dir = (rand() % 2 == 0) ? 1 : -1; // Randomize side check order

    for (int i = 0; i < 2; ++i) { // Check both L/R directions
        for (int step = 1; step <= dispersion; ++step) {
            int check_c = c + (horiz_dir * step);
            int check_r = r; // Keep row the same for horizontal check

            // Check bounds first
            if (!world.isWithinBounds(check_r, check_c)) {
                break; // Stop checking this direction if out of bounds
            }

            // Check if we should yield to a denser particle falling above target
            bool yieldToElementAbove = false;
            Element* aboveTarget = world.getElement(check_r - 1, check_c); // Check cell *above* the potential target

            if (aboveTarget) { // If there is something above the target...
                // Get the density of the element
                float aboveTargetDensity = 0.0f;
                bool densityObtained = false;
                if (Solid* solidAbove = dynamic_cast<Solid*>(aboveTarget)) { aboveTargetDensity = solidAbove->getDensity(); densityObtained = true; }
                else if (Liquid* liquidAbove = dynamic_cast<Liquid*>(aboveTarget)) { aboveTargetDensity = liquidAbove->getDensity(); densityObtained = true; }
                else if (Gas* gasAbove = dynamic_cast<Gas*>(aboveTarget)) { aboveTargetDensity = gasAbove->getDensity(); densityObtained = true; }

                // Check if the element above has density AND is denser than this liquid
                if (densityObtained && aboveTargetDensity > this->getDensity()) {
                    yieldToElementAbove = true; // Yield to the denser falling element
                }
            }

            // Check if target spot in nextGrid is already claimed OR if we should yield
            if (world.getElementFromNext(check_r, check_c) || yieldToElementAbove) {
                // If claimed OR denser element is above, water cannot flow here horizontally this step.
                break; // Stop checking further in this direction
            }

			// Hits here if the target cell is empty and not blocked by a denser element above
            // Now check if the original spot was empty (only move into originally empty spots horizontally)
            Element* originalTargetElement = world.getElement(check_r, check_c);
			// Move to closest empty spot if found
            if (!originalTargetElement) {
                // Found a potential empty spot! Is it the closest so far?
                if (step < min_dist) {
                    min_dist = step;
                    best_h_move_c = check_c;
                }
                // Continue checking on this side, maybe find closer on other side
            }
            else {
                // Hit a non-empty original cell, stop checking further this direction
                break;
            }
        }

        // Optimization: If we found the closest possible move (step 1), no need to check other side.
        if (min_dist == 1) break;

        horiz_dir *= -1; // Flip direction to check other side
    }

    // After checking both sides, if we found a valid target column:
    if (best_h_move_c != c) {
        // Attempt the move to the best (closest) spot found
        if (world.tryMoveOrSwap(r, c, r, best_h_move_c)) {
            return true; // Moved horizontally
        }
    }

    // --- No Movement ---
    return false;
}

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
    // TODO: Could refine this - maybe only if the gas above is the *same* gas type?
    bool spaceAbove = !elementAbove || dynamic_cast<Gas*>(elementAbove);

    if (!spaceAbove) {
        return false; // Blocked above
    }

    // 3. Probability Check (default 20% chance per tick if conditions met)
    const int EVAPORATION_CHANCE = 20; // Percent
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
        return true; // Evaporation successful
    }

    return false; // Failed to create gas element
}