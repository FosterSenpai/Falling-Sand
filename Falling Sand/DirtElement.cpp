// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Implementation file for the DirtElement class.
// ============================================================================

#include "DirtElement.h"
#include "World.h"          // Needed for update parameters & neighbour checks
#include "Particle.h"       // For ParticleType enum
#include <SFML/Graphics.hpp> // For sf::Color
#include <cstdlib>           // For rand()
#include <memory>            // For std::move in setNextElement

// We will need GrassElement later for the transformation
// #include "GrassElement.h"

// **=== Constructors ===**

DirtElement::DirtElement() : m_timeSinceExposed(0) {
    // Initialize time exposed counter
}

// **=== Overridden Public Methods ===**
// Temporarily reduce threshold and increase chance for testing
constexpr int DirtElement::GRASS_GROW_TIME_THRESHOLD; // Keep original declaration linked
constexpr int DirtElement::GRASS_GROW_CHANCE_PERCENT; // Keep original declaration linked
const int TEST_GROW_THRESHOLD = 20; // Lower threshold for testing
const int TEST_GROW_CHANCE = 100;   // 100% chance for testing

void DirtElement::update(World& world, int r, int c) {
    // Increment base age (may or may not be used by dirt itself)
    age++;

    bool becameGrass = false; // Flag to check if transformation happens

    // --- Grass Growth Logic ---
    // 1. Check cell directly above
    Element* elementAbove1 = world.getElement(r - 1, c); // Check current grid state
    bool above1IsEmpty = !elementAbove1; // True if nullptr

    if (above1IsEmpty) {
        // If space above, increment exposure timer
        m_timeSinceExposed++;

        // 2. Check if exposure threshold met
        if (m_timeSinceExposed > GRASS_GROW_TIME_THRESHOLD) {
            // 3. Check random chance
            if ((rand() % 100) < GRASS_GROW_CHANCE_PERCENT) {
                // 4. Check cells 2 and 3 above are also empty
                Element* elementAbove2 = world.getElement(r - 2, c);
                Element* elementAbove3 = world.getElement(r - 3, c);
                if (!elementAbove2 && !elementAbove3) {
                    // Conditions met! Transform into Grass for the NEXT tick.
                    // Create a new Grass element (assuming GrassElement exists)
                    std::unique_ptr<Element> newGrass = world.createElementByType(ParticleType::GRASS);
                    if (newGrass) {
                        // Place it in the next grid, replacing this dirt particle
                        world.setNextElement(r, c, std::move(newGrass));
                        becameGrass = true; // Mark that transformation occurred
                    }
                }
            }
        }
    }
    else {
        // Covered by something, reset exposure timer
        m_timeSinceExposed = 0;
    }

    // --- End Grass Growth Logic ---


    // --- Static Element Sleep Logic ---
    // Dirt usually doesn't move, so allow it to sleep
    // Only mark as updated if it didn't turn into grass (which already updated nextGrid)
    // --- Static Element Sleep Logic ---
    if (!becameGrass) {
        // ONLY go to sleep if NOT exposed to air above
        if (!above1IsEmpty) { // Check the flag we determined earlier
            this->potentiallyGoToSleep();
        }
        else {
            // If exposed to air, ensure we stay awake to keep counting
            this->wakeUp(); // Explicitly stay awake
        }
        this->markAsUpdated();
    }
    // Note: If it became grass, the grass element in nextGrid is new and
    // will handle its own updated flag on the next tick. The original dirt
    // unique_ptr in m_grid[r][c] will be discarded/replaced correctly by World::update.

    // TODO: Add support check logic here later if Dirt should crumble
}

sf::Color DirtElement::getColor() const {
    // Dirt color (e.g., from Utils.cpp)
    return sf::Color(133, 94, 66);
}

ParticleType DirtElement::getType() const {
    return ParticleType::DIRT;
}

// --- Physical Property Implementations (Example Values) ---

float DirtElement::getDensity() const {
    return 1.7f; // Slightly denser than sand?
}

float DirtElement::getHardness() const {
    return 0.3f; // Slightly harder than sand?
}

float DirtElement::getThermalConductivity() const {
    return 0.1f; // Poor conductor
}

float DirtElement::getMeltingPoint() const {
    // Dirt doesn't really "melt" cleanly, often forms magma/lava. Very high temp.
    return 1500.0f; // High placeholder
}

ParticleType DirtElement::getLiquidForm() const {
    // TODO: Add LAVA or MAGMA type later?
    return ParticleType::EMPTY; // Placeholder
}

ParticleType DirtElement::getGasForm() const {
    // Doesn't sublimate easily
    return ParticleType::EMPTY;
}