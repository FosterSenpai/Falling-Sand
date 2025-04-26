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

// **=== Constructor ===**
GrassElement::GrassElement() : m_timeSinceCovered(0) {}

// **=== Overridden Public Methods ===**

void GrassElement::update(World& world, int r, int c) {
    age++;
    bool becameDirt = false;

    // --- Grass Death Logic ---
    Element* elementAbove = world.getElement(r - 1, c);

	// If dirt above die instantly
    if (elementAbove && elementAbove->getType() == ParticleType::DIRT) {
		// Grass dies and turns into dirt
		std::unique_ptr<Element> newDirt = world.createElementByType(ParticleType::DIRT);
		if (newDirt) {
			world.setNextElement(r, c, std::move(newDirt));
			becameDirt = true;
		}
    }

    if (elementAbove) {
        // Increment timer only when covered by a problematic element
        m_timeSinceCovered++;

        // Check if covered for long enough
        if (m_timeSinceCovered > GRASS_DEATH_TIME_THRESHOLD) {
            // Now check random chance to die
            if ((rand() % 100) < GRASS_DEATH_CHANCE_PERCENT) {
                // Grass dies and turns into dirt
                std::unique_ptr<Element> newDirt = world.createElementByType(ParticleType::DIRT);
                if (newDirt) {
                    world.setNextElement(r, c, std::move(newDirt));
                    becameDirt = true;
                }
            }
        }
        // If covered but timer not met, do nothing this tick regarding death.

    }
    else {
        // Reset timer if uncovered OR covered by grass
        m_timeSinceCovered = 0;
    }


    // --- Update Mark ---
    if (!becameDirt) {
        this->wakeUp();
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