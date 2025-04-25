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
    // Increment base age
    age++;

    bool acted = false; // Did the water do anything this tick?

    // --- Evaporation ---
    // Check temperature and attempt to evaporate first
    if (attemptEvaporation(world, r, c)) {
        // If evaporation happened, the element was replaced in nextGrid.
        // No further action needed for this water particle this tick.
        // The new gas particle will mark itself updated next tick.
        // We don't call markAsUpdated() for the original water here.
        acted = true;
        // No need to call wakeUp() here, evaporation handles replacement.
    }

    // --- Flow ---
    // Only attempt flow if not evaporated
    if (!acted) {
        if (attemptFlow(world, r, c)) {
            // If flow happened, mark as acted and ensure awake
            acted = true;
            this->wakeUp();
        }
    }

    // TODO: Add other interactions (e.g., reacting with other elements?)

    // --- Sleep & Update Mark ---
    // Only manage sleep/mark if nothing happened that replaced the element
    if (!acted) {
        //this->potentiallyGoToSleep();
    }
    // Mark as updated regardless of action, as state was processed.
    // Unless it evaporated, in which case it doesn't exist to mark.
    if (!acted) { // Only mark if it didn't evaporate
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