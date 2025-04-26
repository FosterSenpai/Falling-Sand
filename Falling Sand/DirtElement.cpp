// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1
// Description: Implementation file for the DirtElement class.
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

    // --- Grass Growth Logic ---
    Element* elementAbove1 = world.getElement(r - 1, c); // Check current grid state
    bool above1IsEmpty = !elementAbove1;

	if (above1IsEmpty) { // If the cell above is empty, we can check for grass growth
        m_timeSinceExposed++;

        // Check if exposure threshold met
        if (m_timeSinceExposed > GRASS_GROW_TIME_THRESHOLD) {
            // Check random chance
            if ((rand() % 100) < GRASS_GROW_CHANCE_PERCENT) {

                // Random Depth Check (2-6 layers)
                int requiredEmptyDepth = 2 + (rand() % 5);
                bool allLayersClear = true; // Assume clear initially

				for (int layer = 2; layer <= requiredEmptyDepth; ++layer) { // Check layers above
                    Element* elementFurtherAbove = world.getElement(r - layer, c);
                    if (elementFurtherAbove) { // If any layer up to required depth is blocked...
                        allLayersClear = false; // ...then it's not clear enough
                        break; // No need to check further layers
                    }
                }

                // Proceed only if all checked layers above are clear
                if (allLayersClear) {
					// Transform to GrassElement
                    std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                    if (newGrass) {
                        world.setNextElement(r, c, std::move(newGrass));
                        becameGrass = true; // Mark transformation
                    }
                }
            }
        }
    } else {
        // Covered by something, reset exposure timer
        m_timeSinceExposed = 0;
    }


    // --- Static Element Sleep & Update Mark ---
    if (!becameGrass) {
        // ONLY go to sleep if NOT exposed to air above (to allow timer to run)
        if (!above1IsEmpty) {
            this->potentiallyGoToSleep();
        } else {
            this->wakeUp(); // Stay awake while exposed
        }
        this->markAsUpdated();
    }

    // TODO: Add support check logic here later if Dirt should crumble
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