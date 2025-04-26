// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.cpp
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.2
// Description: Implementation file for the WaterElement class.
// ============================================================================

#include "WaterElement.h"
#include "World.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <cstdlib>


// **=== Constructor ===**

WaterElement::WaterElement(){
    m_variedColor = sf::Color(60, 120, 180); // Base shallow color
}


// **=== Overridden Public Methods ===**

void WaterElement::update(World& world, int r, int c) {

	age++;              // Increment age for this element
	bool acted = false; // Track if the element has acted this tick

    // --- Evaporation ---
    // Check temperature and attempt to evaporate first
    if (attemptEvaporation(world, r, c)) {
        // If evaporation happened, the element was replaced in nextGrid.
        // No further action needed for this water particle this tick.
        acted = true;
        // Don't mark as updated, it was replaced.
    }

    // --- Flow ---
    // Attempt flow if not evaporated
    if (!acted) {
        if (attemptFlow(world, r, c)) {
			acted = true; // Element successfully flowed
        }
    }

    // --- NEW: Dynamic Color Calculation based on Depth ---
    int waterDepth = 0;
    const int MAX_WATER_DEPTH_CHECK = 10; // How many cells up to check maximum

    for (int i = 1; i <= MAX_WATER_DEPTH_CHECK; ++i) {
        Element* elementAbove = world.getElement(r - i, c);
        if (elementAbove && elementAbove->getType() == ParticleType::WATER) {
            waterDepth++;
        }
        else {
            // Stop counting if we hit non-water or empty space
            break;
        }
    }

    // --- Map depth to color ---
    // Define shallow and deep colors
    sf::Color shallowColor(60, 120, 180);
    sf::Color deepColor(30, 60, 100); // Make this darker/different hue

    // Calculate interpolation factor (0.0 at surface, 1.0 at max depth or deeper)
    float factor = std::min(1.0f, static_cast<float>(waterDepth) / static_cast<float>(MAX_WATER_DEPTH_CHECK));

    // Linearly interpolate between shallow and deep colors
    uint8_t red = static_cast<uint8_t>(shallowColor.r + (deepColor.r - shallowColor.r) * factor);
    uint8_t green = static_cast<uint8_t>(shallowColor.g + (deepColor.g - shallowColor.g) * factor);
    uint8_t blue = static_cast<uint8_t>(shallowColor.b + (deepColor.b - shallowColor.b) * factor);

    // Store the calculated color in the member variable for rendering
    m_variedColor = sf::Color(red, green, blue);

    // --- Update Mark ---
    // Mark as updated if the element still exists (wasn't evaporated)
    // This tells the World::update loop that this element processed its turn.
    if (!acted) {
        this->markAsUpdated();
    }
    else {
        // If it acted (flowed), it also needs marking.
        // Only skip marking if evaporated (element is gone).
        this->markAsUpdated();
    }
}

sf::Color WaterElement::getColor() const {
    // TODO: Add variation based on depth/neighbours later?
    return sf::Color(60, 120, 180);
}

ParticleType WaterElement::getType() const {
    return ParticleType::WATER;
}

// **=== Property Implementations ===**

float WaterElement::getDensity() const {
    return 1.0f;
}

int WaterElement::getDispersionRate() const {
    return 6;
}

float WaterElement::getBoilingPoint() const {
    return 100.0f; // Degrees C
}

ParticleType WaterElement::getGasForm() const {
    return ParticleType::STEAM;
}