// ============================================================================
// Project:     Falling Sand Simulation
// File:        Element.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.5
// Description: Header file for the Element abstract base class.
//              Defines the common interface and fundamental properties
//              (temperature, velocity, age, simulation flags)
//              that all specific simulation elements possess.
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "Particle.h"
#include <cstdlib>
#include <algorithm>

class World;

/**
 * @brief Abstract base class for all simulated elements (particles) in the world.
 * Defines the common interface and fundamental physical properties for all elements.
 */
class Element {
public:
    // **=== Constants ===**
	static constexpr float DEFAULT_TEMPERATURE = 20.0f; // Default temperature in Celsius

    // **=== Destructor ===**

    /**
     * @brief Virtual destructor.
     */
    virtual ~Element() = default;

    // **=== Core Simulation Methods (Pure Virtual) ===**

    /**
	 * @brief Updates the element's state based on simulation logic.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;

    /**
     * @brief Gets the display color of this element.
     * @return sf::Color The color used to draw this element.
     */
    virtual sf::Color getColor() const = 0;

    /**
     * @brief Gets the specific type identifier for this element.
     * @return ParticleType The enum value representing the specific kind of element.
     */
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the unique, potentially varied color for rendering this specific particle.
     * @return sf::Color The color stored in m_variedColor.
     */
    sf::Color getRenderColor() const {
        return m_variedColor;
    }


    // **=== Common Physics & State Methods ===**

    /**
     * @brief Gets the current temperature of the element.
     * @return float The temperature value (Celsius).
     */
    virtual float getTemperature() const {
        return temperature;
    }

    /**
     * @brief Adds (or removes, if negative) heat energy to the element, potentially changing its temperature.
     * @param amount The amount of heat energy to add.
     */
    virtual void addHeat(float amount) {
        temperature += amount;
        this->wakeUp(); // wake up on temp change
    }

    /**
     * @brief Gets the maximum lifetime of the element in simulation ticks.
     * @return int Maximum lifetime in ticks, or <= 0 for infinite.
     */
    virtual int getMaxLifetime() const {
        return -1; // Default: infinite lifetime
    }

    /**
     * @brief Checks if the element is currently considered "awake".
     * @return true if the element is awake, false otherwise.
     */
    virtual bool isAwake() const {
        return awake;
    }

    /**
     * @brief Wakes the element up.
     */
    virtual void wakeUp() {
        awake = true;
    }

    /**
     * @brief Allows the element to potentially go to sleep if its state is stable.
     */
    virtual void potentiallyGoToSleep() {
        if (velocity_x == 0.0f && velocity_y == 0.0f) {
            awake = false;
        }
    }

    /**
     * @brief Checks if this element has already been updated in the current simulation tick.
     * Useful for preventing double updates.
     * @return true if the element's final state for this tick has been determined, false otherwise.
     */
    virtual bool isUpdatedThisTick() const {
        return updatedThisTick;
    }

    /**
     * @brief Marks this element as having been updated for the current simulation tick.
     * Should be called by the update logic once the element's final state/position
	 * for the tick is determined and written to the next grid.
     */
    virtual void markAsUpdated() {
        updatedThisTick = true;
    }

    /**
     * @brief Resets the update flag for the element.
     * Should be called on all elements at the very beginning of each simulation tick,
     * before any update logic runs.
     */
    virtual void resetUpdateFlag() {
        updatedThisTick = false;
    }


protected:
    // **=== Protected Members ===**
    int age = 0;

	// **== Physical Properties ==**
    float temperature = DEFAULT_TEMPERATURE;
	// --Element Velocity --
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;

	// -- Simulation Flags --
	/**
	 * @brief Flag indicating if the element is currently "awake" (active).
	 * If false, the element will not be updated in the simulation.
	 */
    bool awake = true;
    /**
     * @brief Flag indicating if the element's state for the current tick has been finalized.
     * Reset at the start of each tick, set to true when update is complete for this element.
     */
    bool updatedThisTick = false;

    /**
	 * @brief Unique color for rendering this specific particle.
     */
    sf::Color m_variedColor;

    // **=== Protected Helper Methods ===**

    /**
     * @brief Calculates a random variation based on a base color and stores it.
     * Should be called by derived class constructors.
     * @param baseColor The base color for the element type.
     */
    void initializeColorVariation(sf::Color baseColor) {
        // --- Adjust the variation range as desired ---
        int variation = 15; // Max +/- change for R, G, B
        int r_offset = (rand() % (variation * 2 + 1)) - variation; // -variation to +variation
        int g_offset = (rand() % (variation * 2 + 1)) - variation;
        int b_offset = (rand() % (variation * 2 + 1)) - variation;

        // Clamp values between 0 and 255
        int r = std::min(255, std::max(0, static_cast<int>(baseColor.r) + r_offset));
        int g = std::min(255, std::max(0, static_cast<int>(baseColor.g) + g_offset));
        int b = std::min(255, std::max(0, static_cast<int>(baseColor.b) + b_offset));

        m_variedColor = sf::Color(  
           static_cast<std::uint8_t>(r),  
           static_cast<std::uint8_t>(g),  
           static_cast<std::uint8_t>(b)
        );
    }
};