// ============================================================================
// Project:     Falling Sand Simulation
// File:        SandElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25
// Version:     1.0
// Description: Implementation file for the SandElement class.
// ============================================================================

#include "SandElement.h"
#include "World.h"          // Needed for update parameters
#include "Particle.h"       // For ParticleType enum
#include <SFML/Graphics.hpp> // For sf::Color

// **=== Overridden Public Methods ===**

void SandElement::update(World& world, int r, int c) {
    // Increment age at the start of the update
    age++;

    // Attempt the standard dynamic solid fall/slide logic
    bool moved = attemptFall(world, r, c); // Calls the helper from DynamicSolid

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
    // Note: The actual move/swap happens via World methods called within attemptFall,
    // modifying the m_nextGrid.
    this->markAsUpdated();
}

sf::Color SandElement::getColor() const {
    // Return sand color (e.g., from Utils.cpp)
    // TODO: Add slight random variation later if desired
    return sf::Color(194, 178, 128);
}

ParticleType SandElement::getType() const {
    return ParticleType::SAND;
}

// --- Physical Property Implementations ---

float SandElement::getDensity() const {
    // Sand is denser than water (typically ~1.0), less dense than many rocks/metals
    return 1.6f; // Example value (adjust based on desired interactions)
}

float SandElement::getHardness() const {
    // Sand is not very hard
    return 0.2f; // Example value (relative scale 0-1? or other scale)
}

float SandElement::getThermalConductivity() const {
    // Sand is a poor conductor of heat (insulator)
    return 0.05f; // Example low value
}

float SandElement::getMeltingPoint() const {
    // Silica sand melts at a very high temperature
    return 1700.0f; // Example value (degrees C)
}

ParticleType SandElement::getLiquidForm() const {
    // What does sand turn into when melted? Molten glass/silica.
    // We don't have this type yet. Return EMPTY for now.
    // TODO: Add ParticleType::MOLTEN_GLASS or similar later
    return ParticleType::EMPTY;
}

ParticleType SandElement::getGasForm() const {
    // Sand doesn't typically sublimate.
    // Maybe return a special NONE type if we add one, or EMPTY.
    return ParticleType::EMPTY;
}