// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.2
// Description: Implementation file for the WaterElement class.
// ============================================================================

#include "WaterElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <cstdlib>

// **=== Overridden Public Methods ===**

void WaterElement::update(World& world, int r, int c) {

	age++;              // Increment age for this element
	bool acted = false; // Track if the element has acted this tick

    // --- Evaporation ---
    // Check temperature and attempt to evaporate first
    if (attemptEvaporation(world, r, c)) {
        // If evaporation happened, the element was replaced in nextGrid.
        // No further action needed for this water particle this tick.
        acted = true;
        // Don't mark as updated, it was replaced.
    }

    // --- Flow ---
    // Attempt flow if not evaporated
    if (!acted) {
        if (attemptFlow(world, r, c)) {
			acted = true; // Element successfully flowed
        }
    }

    // --- Update Mark ---
    // Mark as updated if the element still exists (wasn't evaporated)
    // This tells the World::update loop that this element processed its turn.
    if (!acted) {
        this->markAsUpdated();
    }
    else {
        // If it acted (flowed), it also needs marking.
        // Only skip marking if evaporated (element is gone).
        this->markAsUpdated();
    }
}

sf::Color WaterElement::getColor() const {
    // TODO: Add variation based on depth/neighbours later?
    return sf::Color(60, 120, 180);
}

ParticleType WaterElement::getType() const {
    return ParticleType::WATER;
}

// **=== Property Implementations ===**

float WaterElement::getDensity() const {
    return 1.0f;
}

int WaterElement::getDispersionRate() const {
    return 6;
}

float WaterElement::getBoilingPoint() const {
    return 100.0f; // Degrees C
}

ParticleType WaterElement::getGasForm() const {
    return ParticleType::STEAM;
}