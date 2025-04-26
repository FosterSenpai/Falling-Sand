// ============================================================================
// Project:     Falling Sand Simulation
// File:        SandElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.1
// Description: Implementation file for the SandElement class.
// ============================================================================

#include "SandElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <cstdlib>

// **=== Constructor ===**
SandElement::SandElement() {
    initializeColorVariation(getColor());
}

// **=== Overridden Public Methods ===**

void SandElement::update(World& world, int r, int c) {
    // Increment age at the start of the update
    age++;

    // Attempt the standard dynamic solid fall/slide logic
    bool moved = attemptFall(world, r, c);

    // Manage awake state based on movement
    if (moved) {
        this->wakeUp(); // Ensure it stays awake if it moved
    }
    else {
        this->potentiallyGoToSleep(); // Allow it to sleep if it didn't move
    }

    // TODO: Add temperature-based logic (melting checks using getMeltingPoint)
    // TODO: Add interactions with other elements based on temperature/type

    // Mark this element as updated for this tick AFTER its state is final
    this->markAsUpdated();
}

sf::Color SandElement::getColor() const {
    return sf::Color(194, 178, 128);
}

ParticleType SandElement::getType() const {
    return ParticleType::SAND;
}

float SandElement::getDensity() const {
    // Sand is denser than water (typically ~1.0), less dense than many rocks/metals
    return 1.6f;
}


// **=== Concrete Property Implementations ===**

float SandElement::getHardness() const {
    // Sand is not very hard
    return 0.2f;
}

float SandElement::getThermalConductivity() const {
    // Sand is a poor conductor of heat (insulator)
    return 0.05f;
}

float SandElement::getMeltingPoint() const {
    // Silica sand melts at a very high temperature
    return 1700.0f;
}

ParticleType SandElement::getLiquidForm() const {
    // What does sand turn into when melted? Molten glass/silica.
    // Don't have this type yet. Return EMPTY for now.
    // TODO: Add ParticleType::MOLTEN_GLASS
    return ParticleType::EMPTY;
}

ParticleType SandElement::getGasForm() const {
    // Sand doesn't typically sublimate.
    return ParticleType::EMPTY;
}