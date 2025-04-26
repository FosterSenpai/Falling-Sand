// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-27
// Version:     1.4
// Description: Implementation file for the WaterElement class. (Single Base Color)
// ============================================================================

#include "WaterElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <cstdlib>
#include <algorithm>

// **=== Constructor ===**

WaterElement::WaterElement() {
    // Directly set the single base color for all water particles.
    m_variedColor = sf::Color(60, 120, 180);
}


// **=== Overridden Public Methods ===**

void WaterElement::update(World& world, int r, int c) {

    age++;
    bool acted = false;

    // --- Evaporation ---
    if (attemptEvaporation(world, r, c)) {
        acted = true;
        // No update needed if evaporated
    }

    // --- Flow ---
    if (!acted) {
        if (attemptFlow(world, r, c)) {
            acted = true;
            this->wakeUp();
        }
    }

    // --- Update Mark ---
    if (!acted && !attemptEvaporation(world, r, c)) {
        //this->potentiallyGoToSleep();
    }
    if (!attemptEvaporation(world, r, c)) {
        this->markAsUpdated();
    }
}

sf::Color WaterElement::getColor() const {
    // This still returns the conceptual base color, which is consistent
    // with what we set in the constructor.
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
    return 10; // Still using the original dispersion rate
}

float WaterElement::getBoilingPoint() const {
    return 100.0f; // Degrees C
}

ParticleType WaterElement::getGasForm() const {
    return ParticleType::STEAM;
}