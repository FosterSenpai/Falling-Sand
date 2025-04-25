// ============================================================================
// Project:     Falling Sand Simulation
// File:        StaticSolid.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Implementation file for the StaticSolid abstract class.
//              May contain common logic/helpers for static solids later.
// ============================================================================

#include "StaticSolid.h"
#include "World.h" // Include World if needed for future logic

// **=== Protected Helper Methods ===**

/**
 * @brief Performs common checks on the element's immediate environment.
 * Can be called by derived class's update() method to get information
 * about neighbors (e.g., the element above) to decide on reactions
 * like Dirt->Grass transformation.
 * (Currently empty, logic is handled in derived classes for now).
 * @param world Reference to the world grid.
 * @param r Current row.
 * @param c Current column.
 */
void StaticSolid::checkEnvironment(World& world, int r, int c) {
    // Base implementation is empty for now.
    // Derived classes like DirtElement/GrassElement currently handle
    // their own environment checks directly in their update() methods.
    // Common logic could be added here later if needed.
    (void)world; // Mark parameters as unused to prevent compiler warnings
    (void)r;
    (void)c;
}

// NOTE: No implementations needed for the pure virtual functions here,
// as StaticSolid remains abstract. Those are implemented by concrete
// classes like DirtElement and GrassElement.