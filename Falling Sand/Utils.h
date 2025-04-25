#pragma once
#include <SFML/Graphics.hpp> // For sf::Color
#include <string>
#include "Particle.h"        // For ParticleType

// holds general utility functions, especially those related to particles but not tied to the 
// World or Game state directly.
// Using a namespace for utility functions is often preferred over a static class
namespace Utils {

    sf::Color getColorForType(ParticleType type);// Gets the display color for a given particle type

    std::string getNameForType(ParticleType type);// Converts a particle type enum to its string name (for UI)

	int getDensityForType(ParticleType type);// Gets the density of a particle type (for brush density)
}