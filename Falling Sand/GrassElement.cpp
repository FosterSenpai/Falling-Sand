// ============================================================================
// Project:     Falling Sand Simulation
// File:        GrassElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1 // Updated version
// Description: Implementation file for the GrassElement class.
// ============================================================================

#include "GrassElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <memory>
#include <iostream>
#include "DirtElement.h"

// **=== Overridden Public Methods ===**

void GrassElement::update(World& world, int r, int c) {
    age++; // Increment age
    bool becameDirt = false;

    // --- Grass Death Logic ---
    // 1. Check cell directly above
    Element* elementAbove = world.getElement(r - 1, c); // Check current grid state

    // If there IS something above (not null/empty)
    if (elementAbove) {
        // 2. Check random chance to die
        if ((rand() % 100) < GRASS_DEATH_CHANCE_PERCENT) {
			// Grass dies and turns into dirt
            std::unique_ptr<Element> newDirt = world.createElementByType(ParticleType::DIRT);
            if (newDirt) {
                world.setNextElement(r, c, std::move(newDirt));
                becameDirt = true; // Mark transformation
            }
        }
    }

    // --- Update Mark ---
    // Grass is currently set to always be "awake" (no potentiallyGoToSleep)
    // Mark as updated if it wasn't replaced by Dirt
    if (!becameDirt) {
        this->markAsUpdated();
    }
}

sf::Color GrassElement::getColor() const {
    return sf::Color(40, 140, 40);
}

ParticleType GrassElement::getType() const {
    return ParticleType::GRASS;
}

float GrassElement::getDensity() const {
    return 1.1f;
}

// **=== Concrete Property Implementations ===**

float GrassElement::getHardness() const {
    return 0.1f; // Very soft
}

float GrassElement::getThermalConductivity() const {
    return 0.15f; // Similar to dirt? Low.
}

float GrassElement::getMeltingPoint() const {
    // Grass burns/decomposes. Use a relatively low temp compared to rock/sand.
    return 400.0f; // Example decomposition/ignition temperature?
}

ParticleType GrassElement::getLiquidForm() const {
    // Doesn't melt into a liquid. Maybe turns to Ash?
    // TODO: Add ASH type later?
    return ParticleType::EMPTY; // Placeholder
}

ParticleType GrassElement::getGasForm() const {
    // Burns into maybe smoke/carbon? Needs reaction system.
    // TODO: Add SMOKE type later?
    return ParticleType::EMPTY; // Placeholder
}