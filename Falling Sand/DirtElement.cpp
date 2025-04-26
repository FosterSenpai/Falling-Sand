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

DirtElement::DirtElement() : m_timeSinceExposed(0) {}

// **=== Overridden Public Methods ===**

void DirtElement::update(World& world, int r, int c) {
    age++;
    bool becameGrass = false;

    Element* elementAbove = world.getElement(r - 1, c);
    bool isEffectivelyExposed = !elementAbove || (elementAbove && elementAbove->getType() == ParticleType::GRASS);

    if (isEffectivelyExposed) {
        m_timeSinceExposed++;

        if (m_timeSinceExposed > GRASS_GROW_TIME_THRESHOLD) {
            if ((rand() % 100) < GRASS_GROW_CHANCE_PERCENT) {

				int surfaceHeight = world.getSurfaceHeight(c); // Get surface height for this column

                // Calculate distance/depth of this block from the surface
                int surfaceDistance = (surfaceHeight < world.getRows()) ? (r - surfaceHeight + 1) : 0;

                // Determine the random maximum growth depth (2-5 layers)
                int randomMaxGrowthDepth = 2 + (rand() % 5);

                // Check if this dirt block is within the allowed random depth
                if (surfaceDistance > 0 && surfaceDistance <= randomMaxGrowthDepth) {

                    // only grow if directly below air or grass
                    bool growConditionMet = false;
                    if (!elementAbove) { // Exposed to air
                        growConditionMet = true;
                    }
                    else if (elementAbove->getType() == ParticleType::GRASS) { // Below grass
                        growConditionMet = true;
                    }

					// If the growth condition is met, transform to GrassElement
                    if (growConditionMet) {
                        std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                        if (newGrass) {
                            world.setNextElement(r, c, std::move(newGrass));
                            becameGrass = true;
                        }
                    }
                }
            }
            // Reset timer slightly randomly
            if (rand() % 5 == 0) {
                m_timeSinceExposed = GRASS_GROW_TIME_THRESHOLD - (rand() % 10);
            }
        }
    }
    else {
        m_timeSinceExposed = 0;
    }

    // --- Static Element Sleep & Update Mark ---
    if (!becameGrass) {
        if (!isEffectivelyExposed) {
            this->potentiallyGoToSleep();
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