// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-27
// Version:     1.3
// Description: Implementation file for the DirtElement class.
//              Turns into grass if exposed within a random depth from the surface.
// ============================================================================

#include "DirtElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <memory>
#include <iostream>

#include "GrassElement.h"

// **=== Constructors ===**

DirtElement::DirtElement() : m_timeSinceExposed(0) {
	initializeColorVariation(getColor());
}

// **=== Overridden Public Methods ===**

void DirtElement::update(World& world, int r, int c) {
    age++;
    bool becameGrass = false;

    Element* elementAbove = world.getElement(r - 1, c);
    // Allow growth if air OR grass is directly above
    bool isEffectivelyExposed = !elementAbove || (elementAbove && elementAbove->getType() == ParticleType::GRASS);

    if (isEffectivelyExposed) {
        m_timeSinceExposed++;

        if (m_timeSinceExposed > GRASS_GROW_TIME_THRESHOLD) {
            if ((rand() % 100) < GRASS_GROW_CHANCE_PERCENT) {
				// Create the grass element
                std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                if (newGrass) {
                    world.setNextElement(r, c, std::move(newGrass));
                    becameGrass = true;
                }

            }
            // Reset timer slightly randomly
            if (!becameGrass && (rand() % 5 == 0)) {
                m_timeSinceExposed = GRASS_GROW_TIME_THRESHOLD - (rand() % 10);
            }
        }
    }
    else {
        m_timeSinceExposed = 0; // Reset timer if covered
    }

    // --- Static Element Sleep & Update Mark ---
    if (!becameGrass) {
        if (!isEffectivelyExposed) {
            // Consider uncommenting sleep for performance on buried dirt
            // this->potentiallyGoToSleep();
        }
        else {
            this->wakeUp();
        }
        this->markAsUpdated();
    }
}
sf::Color DirtElement::getColor() const {
    return sf::Color(133, 94, 66);
}

ParticleType DirtElement::getType() const {
    return ParticleType::DIRT;
}

float DirtElement::getDensity() const {
    return 1.7f; // Slightly denser than sand
}


// **=== Concrete Property Implementations ===**

float DirtElement::getHardness() const {
    return 0.3f; // Slightly harder than sand
}

float DirtElement::getThermalConductivity() const {
    return 0.1f; // Poor conductor
}

float DirtElement::getMeltingPoint() const {
    return 1500.0f;
}

ParticleType DirtElement::getLiquidForm() const {
    // TODO: Add LAVA or MAGMA type later?
    return ParticleType::EMPTY; // Placeholder
}

ParticleType DirtElement::getGasForm() const {
    return ParticleType::EMPTY;
}