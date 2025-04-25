// ============================================================================
// Project:     Falling Sand Simulation
// File:        Liquid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25 // Updated date
// Version:     1.2 // Incremented version
// Description: Header file for the Liquid abstract class.
//              Inherits from Element and serves as a base for all liquid
//              particle types. Defines common liquid properties (density,
//              dispersion, boiling point) and interfaces for behaviour
//              (flow, flammability, evaporation).
// ============================================================================

#pragma once

#include "Element.h" // Include the base class definition

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Liquid state of matter.
 *
 * Defines interfaces and potentially common logic for liquid behaviours like
 * flow, density interactions, and evaporation. Concrete derived classes must
 * provide implementations for the pure virtual functions.
 */
class Liquid : public Element {
public:
    // **=== Destructor ===**
    virtual ~Liquid() = default;

    // **=== Core Simulation Method (Pure Virtual) ===**
    /**
     * @brief Updates the liquid element's state (flow, interactions, evaporation).
     * Concrete derived classes MUST implement this. They will likely call helper
     * functions like attemptFlow() and attemptEvaporation() within their implementation.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;

    // **=== Property Getters (Pure Virtual or Virtual with Default) ===**
    virtual sf::Color getColor() const = 0;
    virtual ParticleType getType() const = 0;
    virtual float getDensity() const = 0;
    virtual int getDispersionRate() const = 0;

    /**
     * @brief Gets the boiling point temperature for this liquid.
     * (Must be implemented by concrete derived classes).
     * @return float The temperature (e.g., in Celsius) at which evaporation/boiling occurs.
     */
    virtual float getBoilingPoint() const = 0;

    /**
     * @brief Gets the ParticleType of the gas this liquid turns into upon evaporation.
     * (Must be implemented by concrete derived classes).
     * Requires corresponding Gas types (e.g., STEAM) to be added to ParticleType enum.
     * @return ParticleType The type of the resulting gas element.
     */
    virtual ParticleType getGasForm() const = 0;

    virtual bool isFlammable() const {
        return false; // Default: liquids are not flammable
    }

    // **=== Interaction Methods ===**
    virtual bool canBeDisplacedBy(ParticleType incomingType, float incomingDensity) const {
        return incomingDensity > this->getDensity(); // Default implementation
    }

protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Attempts to perform standard liquid flow logic.
     * (Implementation would go in Liquid.cpp or be defined inline).
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if the liquid successfully moved or attempted to move, false otherwise.
     */
    virtual bool attemptFlow(World& world, int r, int c); // Declaration only

    /**
     * @brief Attempts to evaporate the liquid based on temperature and conditions.
     * Checks temperature against boiling point, checks space above, and potentially
     * replaces this liquid element with its gas form via the World object.
     * Should be called by the derived class's update() method.
     * (Implementation would go in Liquid.cpp or be defined inline).
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if evaporation occurred, false otherwise.
     */
    virtual bool attemptEvaporation(World& world, int r, int c); // Declaration only

private:
    // **=== Private Members ===**
};