// ============================================================================
// Project:     Falling Sand Simulation
// File:        Gas.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.1
// Description: Header file for the Gas abstract class.
//              Inherits from Element and serves as a base for all gaseous
//              particle types. Defines common gas properties (density,
//              dispersion, condensation) and interfaces for behaviour
//              (expansion, flammability, condensation).
// ============================================================================

#pragma once

#include "Element.h"

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Gaseous state of matter.
 *
 * Defines interfaces and potentially common logic for gaseous behaviours like
 * rising, expansion, density interactions, and condensation.
 */
class Gas : public Element {
public:
    // **=== Destructor ===**

    /**
     * @brief Virtual destructor. Ensures proper cleanup for derived classes.
     */
    virtual ~Gas() = default;


    // **=== Core Simulation Method ===**

    /**
     * @brief Updates the gas element's state (expansion, interactions, condensation).
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;


    // **=== Getters ===**

    /**
     * @brief Gets the display color of this gas element.
     * @return sf::Color The color for rendering.
     */
    virtual sf::Color getColor() const = 0;

    /**
     * @brief Gets the specific type identifier for this gas element.
     * @return ParticleType The enum value (e.g., ParticleType::STEAM).
     */
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this gas element. Crucial for buoyancy
     * (rising/falling) relative to other gases.
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;

    /**
     * @brief Gets the dispersion rate (how readily it spreads).
     * 
     * Higher values mean the gas spreads more aggressively. Controls movement
     * range in expansion logic.
     * @return int The number of cells to check/attempt to move into each tick.
     */
    virtual int getDispersionRate() const = 0;

    /**
     * @brief Gets the condensation point temperature for this gas.
     * @return float The temperature (e.g., in Celsius) at or below which condensation can occur.
     */
    virtual float getCondensationPoint() const = 0;

    /**
     * @brief Gets the ParticleType of the liquid this gas turns into upon condensation.
     * @param type The ParticleType of the element to create.
     * @return ParticleType The type of the resulting liquid element.
     */
    virtual ParticleType getLiquidForm() const = 0;

    /**
     * @brief Checks if this gas is flammable.
     * @return true if flammable, false otherwise. Defaults to false.
     */
    virtual bool isFlammable() const {
        return false;
    }

protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Attempts to perform standard gas expansion/rising logic.
     * Checks upwards, diagonally upwards, and sideways based on dispersion rate,
     * prioritizing empty cells or displacing lighter gases.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if the gas successfully moved or swapped, false otherwise.
     */
    virtual bool attemptExpansion(World& world, int r, int c); // Declaration only

    /**
     * @brief Attempts to condense the gas into its liquid form based on temperature.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if condensation occurred and element was replaced, false otherwise.
     */
    virtual bool tryCondense(World& world, int r, int c); // Declaration only

};