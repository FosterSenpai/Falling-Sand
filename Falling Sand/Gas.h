// ============================================================================
// Project:     Falling Sand Simulation
// File:        Gas.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25 // Updated date
// Version:     1.1 // Incremented version
// Description: Header file for the Gas abstract class.
//              Inherits from Element and serves as a base for all gaseous
//              particle types. Defines common gas properties (density,
//              dispersion, condensation) and interfaces for behaviour
//              (expansion, flammability, condensation).
// ============================================================================

#pragma once

#include "Element.h" // Include the base class definition

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Gaseous state of matter.
 *
 * Defines interfaces and potentially common logic for gaseous behaviours like
 * rising, expansion, density interactions, and condensation. Concrete derived
 * classes must provide implementations for the pure virtual functions.
 */
class Gas : public Element {
public:
    // **=== Destructor ===**
    virtual ~Gas() = default;

    // **=== Core Simulation Method (Pure Virtual) ===**
    /**
     * @brief Updates the gas element's state (expansion, interactions, condensation).
     * Concrete derived classes MUST implement this. They will likely call helper
     * functions like attemptExpansion() and tryCondense() within their implementation.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;

    // **=== Property Getters (Pure Virtual or Virtual with Default) ===**
    virtual sf::Color getColor() const = 0;
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this gas element. Crucial for buoyancy
     * (rising/falling) relative to other gases.
     * (Must be implemented by concrete derived classes).
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;

    /**
     * @brief Gets the dispersion rate (how readily it spreads).
     * Higher values mean the gas spreads more aggressively. Controls movement
     * range in expansion logic.
     * (Must be implemented by concrete derived classes).
     * @return int The number of cells to check/attempt to move into each tick.
     */
    virtual int getDispersionRate() const = 0;

    /**
     * @brief Gets the condensation point temperature for this gas.
     * (Must be implemented by concrete derived classes).
     * @return float The temperature (e.g., in Celsius) at or below which condensation can occur.
     */
    virtual float getCondensationPoint() const = 0;

    /**
     * @brief Gets the ParticleType of the liquid this gas turns into upon condensation.
     * (Must be implemented by concrete derived classes).
     * Requires corresponding Liquid types (e.g., WATER) to be added to ParticleType enum.
     * @return ParticleType The type of the resulting liquid element.
     */
    virtual ParticleType getLiquidForm() const = 0;

    /**
     * @brief Checks if this gas is flammable.
     * @return true if flammable, false otherwise. Defaults to false.
     */
    virtual bool isFlammable() const {
        return false; // Default: gases are not flammable
    }


    // **=== Interaction Methods ===**
    // Could add methods like canBeDisplacedBy, but gas displacement is complex.
    // Often handled implicitly by the attemptExpansion logic comparing densities.


protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Attempts to perform standard gas expansion/rising logic.
     * Checks upwards, diagonally upwards, and sideways based on dispersion rate,
     * prioritizing empty cells or displacing lighter gases.
     * Should be called by the derived class's update() method.
     * (Implementation would go in Gas.cpp or be defined inline).
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if the gas successfully moved or attempted to move, false otherwise.
     */
    virtual bool attemptExpansion(World& world, int r, int c); // Declaration only

    /**
     * @brief Attempts to condense the gas into its liquid form based on temperature.
     * Checks temperature against condensation point, potentially checks neighbors,
     * and may replace this gas element with its liquid form via the World object.
     * Should be called by the derived class's update() method.
     * (Implementation would go in Gas.cpp or be defined inline).
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if condensation occurred, false otherwise.
     */
    virtual bool tryCondense(World& world, int r, int c); // Declaration only


private:
    // **=== Private Members ===**
};