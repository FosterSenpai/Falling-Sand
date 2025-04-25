// ============================================================================
// Project:     Falling Sand Simulation
// File:        GrassElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Implementation file for the GrassElement class.
// ============================================================================

#include "GrassElement.h"
#include "World.h"          // Needed for update parameters & neighbour checks
#include "Particle.h"       // For ParticleType enum
#include <SFML/Graphics.hpp> // For sf::Color
#include <cstdlib>           // For rand()
#include <memory>            // For std::move in setNextElement

// We will need DirtElement later for the transformation
// #include "DirtElement.h" // Not strictly needed if just using factory

// **=== Overridden Public Methods ===**

void GrassElement::update(World& world, int r, int c) {
    // Increment base age
    age++;

    bool becameDirt = false; // Flag

    // --- Grass Death Logic ---
    // 1. Check cell directly above
    Element* elementAbove = world.getElement(r - 1, c); // Check current grid state

    // If there IS something above (not null/empty)
    if (elementAbove) {
        // 2. Check random chance to die
        if ((rand() % 100) < GRASS_DEATH_CHANCE_PERCENT) {
            // Conditions met! Transform into Dirt for the NEXT tick.
            std::unique_ptr<Element> newDirt = world.createElementByType(ParticleType::DIRT);
            if (newDirt) {
                world.setNextElement(r, c, std::move(newDirt));
                becameDirt = true; // Mark transformation
            }
        }
        // Optional: Add a timer (like time_covered) if death shouldn't be instant chance.
    }
    // Else: Nothing above, grass survives.

    // --- End Grass Death Logic ---


    // --- Static Element Sleep Logic ---
    if (!becameDirt) {
        this->markAsUpdated();
    }
    // If it became Dirt, the new dirt element handles its own flags next tick.
}

sf::Color GrassElement::getColor() const {
    // Grass color (e.g., from Utils.cpp)
    return sf::Color(40, 140, 40);
}

ParticleType GrassElement::getType() const {
    return ParticleType::GRASS;
}

// --- Physical Property Implementations (Example Values) ---

float GrassElement::getDensity() const {
    return 1.1f; // Lighter than dirt/sand?
}

float GrassElement::getHardness() const {
    return 0.1f; // Very soft
}

float GrassElement::getThermalConductivity() const {
    return 0.15f; // Similar to dirt?
}

float GrassElement::getMeltingPoint() const {
    // Grass burns/decomposes, doesn't melt cleanly. Use a high dummy value?
    return 400.0f; // Lower than dirt perhaps, representing decomposition temp?
}

ParticleType GrassElement::getLiquidForm() const {
    // Doesn't melt into a standard liquid
    return ParticleType::EMPTY;
}

ParticleType GrassElement::getGasForm() const {
    // Burns into maybe smoke/carbon? Needs reaction system.
    // TODO: Add SMOKE or ASH type later?
    return ParticleType::EMPTY;
}

// Optional Override: Make grass slightly flammable?
// bool GrassElement::isFlammable() const override {
//    return true;
// }