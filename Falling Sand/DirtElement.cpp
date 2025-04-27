// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.2
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

                // --- REMOVE DEPTH CHECK ---
                // int surfaceHeight = world.getSurfaceHeight(c); // No longer needed
                // int surfaceDistance = (surfaceHeight < world.getRows()) ? (r - surfaceHeight + 1) : 0; // No longer needed
                // int randomMaxGrowthDepth = 2 + (rand() % 4); // No longer needed

                // The 'if' condition below is removed, growth happens if time/chance pass
                // if (surfaceDistance > 0 && surfaceDistance <= randomMaxGrowthDepth) { // REMOVED CONDITION

                    // (Also remove the inner redundant growConditionMet check if you haven't already)
                std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                if (newGrass) {
                    world.setNextElement(r, c, std::move(newGrass));
                    becameGrass = true;
                }

                // } // REMOVED corresponding '}'

            }
            // Reset timer slightly randomly (keep this if you like the behaviour)
            if (!becameGrass && (rand() % 5 == 0)) { // Added !becameGrass check
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
            // Keep waking up if exposed, as it might be counting time
            this->wakeUp();
        }
        this->markAsUpdated();
    }
    // If it became grass, the new GrassElement will handle its own update mark
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