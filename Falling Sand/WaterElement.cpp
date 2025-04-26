// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Implementation file for the WaterElement class.
// ============================================================================

#include "WaterElement.h"
#include "World.h"          // Needed for update parameters
#include "Particle.h"       // For ParticleType enum
#include <SFML/Graphics.hpp> // For sf::Color

// **=== Overridden Public Methods ===**

void WaterElement::update(World& world, int r, int c) {
    this->wakeUp(); // Force awake
    age++;
    bool evaporated = false;
    bool moved = false;

    if (attemptEvaporation(world, r, c)) {
        evaporated = true;
    }

    if (!evaporated) {
        moved = attemptFlow(world, r, c); // Store result
        if (moved) {
            // Wakeup handled by tryMoveOrSwap's neighbour calls
        }
    }

    // Mark as updated if the element still exists (wasn't evaporated)
    if (!evaporated) {
        this->markAsUpdated();
    }
}
sf::Color WaterElement::getColor() const {
    // Water color (e.g., from Utils.cpp)
    // TODO: Add variation based on depth/neighbours later?
    return sf::Color(60, 120, 180);
}

ParticleType WaterElement::getType() const {
    return ParticleType::WATER;
}

// --- Liquid Property Implementations ---

float WaterElement::getDensity() const {
    // Standard density for water
    return 1.0f;
}

int WaterElement::getDispersionRate() const {
    // Water spreads relatively easily
    return 6; // Example value (try higher numbers for more spread)
}

float WaterElement::getBoilingPoint() const {
    // Standard boiling point
    return 100.0f; // Degrees C
}

ParticleType WaterElement::getGasForm() const {
    // Water turns into Steam
    // TODO: Add ParticleType::STEAM to the enum in Particle.h
    // For now, return EMPTY as a placeholder if STEAM doesn't exist
    // return ParticleType::EMPTY;
    return ParticleType::STEAM; // Assuming STEAM will be added
}