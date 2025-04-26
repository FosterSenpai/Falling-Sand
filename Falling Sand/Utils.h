// ============================================================================
// Project:     Falling Sand Simulation
// File:        Utils.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-26
// Version:     1.1
// Description: Header file for general utility functions, particularly
//              those related to particles (Elements) but not tied to
//              World or Game state directly.
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Particle.h"

/**
 * @brief Namespace containing general utility functions for the simulation.
 */
namespace Utils {

    // **=== Public Utility Functions ===**

    /**
     * @brief Gets the display color for a given particle type.
     * @param type The ParticleType enum value.
     * @return sf::Color The color associated with the particle type.
     */
    sf::Color getColorForType(ParticleType type);

    /**
     * @brief Converts a particle type enum to its string name (for UI or debugging).
     * @param type The ParticleType enum value.
     * @return std::string The string representation of the particle type name.
     */
    std::string getNameForType(ParticleType type);

    /**
     * @brief Gets the brush placement density for a given particle type.
     * Used to control how densely particles are placed when using the brush tool.
     * @param type The ParticleType enum value.
     * @return int The density percentage (0-100).
     */
    int getDensityForType(ParticleType type); // TODO: Probably need a new name now that density is part of each element

}