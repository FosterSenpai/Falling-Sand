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

    // --- Grass Growth Logic ---
    Element* elementAbove = world.getElement(r - 1, c); // Check cell directly above in current grid
    bool isExposed = !elementAbove || elementAbove->getType() == ParticleType::GRASS; // True if the cell directly above is empty

    if (isExposed) {
        m_timeSinceExposed++;

        // Check if exposure threshold met
        if (m_timeSinceExposed > GRASS_GROW_TIME_THRESHOLD) {
            // Check random chance to *attempt* growth
            if ((rand() % 100) < GRASS_GROW_CHANCE_PERCENT) {

                // 1. Determine distance to the surface (first empty cell above)
                int surfaceDistance = 0;
                for (int check_r = r; check_r >= 0; --check_r) {
                    if (!world.getElement(check_r, c)) { // Found an empty cell (sky)
                        surfaceDistance = r - check_r;
                        break; // Stop searching upwards
                    }
                    // If we reach row 0 and it's not empty, something is wrong,
                    // but we'll use the loop counter implicitly.
                    // Safety break if somehow no empty cell is found (shouldn't happen in normal use)
                    if (check_r == 0) {
                        surfaceDistance = r + 1; // Consider it very deep if top is blocked
                        break;
                    }
                }

                // We only care about the distance *if* the cell *directly* above is empty.
                // The loop above correctly calculates distance assuming r-1 is empty.
                // So surfaceDistance effectively starts at 1 if r-1 is empty.

                // 2. Determine the random maximum growth depth for this attempt (e.g., 1-5 layers)
                //    You can adjust the '5' to change the max possible depth.
                int randomMaxGrowthDepth = 1 + (rand() % 5); // Generates 1, 2, 3, 4, or 5

                // 3. Check if this dirt block is within the allowed random depth
                if (surfaceDistance > 0 && surfaceDistance <= randomMaxGrowthDepth) {
                    // Transform to GrassElement
                    std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                    if (newGrass) {
                        world.setNextElement(r, c, std::move(newGrass));
                        becameGrass = true; // Mark transformation
                    }
                }
                // else: This dirt block is too deep for this random growth check.
            }
            // Reset timer slightly randomly to prevent perfect lines of grass growth attempts
            if (rand() % 5 == 0) { // ~20% chance to reset timer slightly
                m_timeSinceExposed = GRASS_GROW_TIME_THRESHOLD - (rand() % 10);
            }

        }
    }
    else {
        // Covered by something, reset exposure timer
        m_timeSinceExposed = 0;
    }


    // --- Static Element Sleep & Update Mark ---
    if (!becameGrass) {
        // ONLY go to sleep if NOT exposed to air above (to allow timer to run)
        if (!isExposed) {
            this->potentiallyGoToSleep();
        }
        else {
            this->wakeUp(); // Stay awake while exposed and potentially growing grass
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