// ============================================================================
// Project:     Falling Sand Simulation
// File:        Utils.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1
// Description: Implementation file for general utility functions related
//              to particle types (colors, names, densities).
// ============================================================================

#include "Utils.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <string>

// **=== Public Utility Functions ===**

sf::Color Utils::getColorForType(ParticleType type)
{
    // TODO: Make a random variation in brightness of color
    switch (type) {
    case ParticleType::SAND:      return sf::Color(194, 178, 128);
    case ParticleType::SANDWET:   return sf::Color(144, 128, 78);
    case ParticleType::DIRT:      return sf::Color(133, 94, 66);
    case ParticleType::GRASS:     return sf::Color(40, 140, 40);
    case ParticleType::WATER:     return sf::Color(60, 120, 180);
    case ParticleType::SILT:      return sf::Color(115, 105, 90);
    case ParticleType::OIL:       return sf::Color(90, 30, 30);
		// **=== Add new type colors above ===**

    case ParticleType::EMPTY:     return sf::Color::White; // Often background/transparent
    default:                      return sf::Color::Black; // Unknown particles are black
    }
}

std::string Utils::getNameForType(ParticleType type)
{
    std::string particleTypeName;
    switch (type) {
    case ParticleType::SAND:    particleTypeName = "Sand"; break;
    case ParticleType::SANDWET: particleTypeName = "Wet Sand"; break;
    case ParticleType::DIRT:    particleTypeName = "Dirt"; break;
    case ParticleType::GRASS:   particleTypeName = "Grass"; break;
    case ParticleType::WATER:   particleTypeName = "Water"; break;
    case ParticleType::SILT:    particleTypeName = "Silt"; break;
    case ParticleType::OIL:     particleTypeName = "Oil"; break;
    case ParticleType::EMPTY:   particleTypeName = "Empty"; break;
		//**=== Add new type names above ===**
    default:                    particleTypeName = "Unknown"; break;
    }
    return particleTypeName;
}

int Utils::getDensityForType(ParticleType type)
{
    switch (type) {
        // Solids / Granular - place quite densely
    case ParticleType::SAND:    return 85;
    case ParticleType::SANDWET: return 85;
    case ParticleType::DIRT:    return 95; // Dirt might be very dense
    case ParticleType::GRASS:   return 90;
    case ParticleType::SILT:    return 80;

        // Liquids - place less densely for a "splash" effect
    case ParticleType::WATER:   return 40;
    case ParticleType::OIL:     return 35;

        // Eraser - always place (100%)
    case ParticleType::EMPTY:   return 100;

        // Default for unknown types or future additions
    default:                    return 50;
    }
}