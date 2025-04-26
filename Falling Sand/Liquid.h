// ============================================================================
// Project:     Falling Sand Simulation
// File:        Liquid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.3
// Description: Header file for the Liquid abstract class.
//              Inherits from Element and serves as a base for all liquid
//              particle types. Defines common liquid properties (density,
//              dispersion, boiling point) and interfaces for behaviour
//              (flow, flammability, evaporation).
// ============================================================================

#pragma once

#include "Element.h"
#include "Particle.h"

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Liquid state of matter.
 *
 * Defines interfaces and potentially common logic for liquid behaviours like
 * flow, density interactions, and evaporation.
 */
class Liquid : public Element {
public:
    // **=== Destructor ===**

    /**
     * @brief Virtual destructor. Ensures proper cleanup for derived classes.
     */
    virtual ~Liquid() = default;


    // **=== Core Simulation Method ===**

    /**
     * @brief Updates the liquid element's state (flow, interactions, evaporation).
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;


    // **=== Getters ===**

    /**
     * @brief Gets the display color of this liquid element.
     * @return sf::Color The color for rendering.
     */
    virtual sf::Color getColor() const = 0;

    /**
     * @brief Gets the specific type identifier for this liquid element.
     * @return ParticleType The enum value (e.g., ParticleType::WATER).
     */
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this liquid element. Crucial for buoyancy
     * and displacement interactions.
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;

    /**
     * @brief Gets the dispersion rate (how far it tries to spread horizontally).
     * Higher values mean the liquid spreads more readily. Controls horizontal
     * movement range in flow logic.
     * @return int The number of cells to check/attempt to move horizontally.
     */
    virtual int getDispersionRate() const = 0;

    /**
     * @brief Gets the boiling point temperature for this liquid.
     * @return float The temperature (e.g., in Celsius) at which evaporation/boiling occurs.
     */
    virtual float getBoilingPoint() const = 0;

    /**
     * @brief Gets the ParticleType of the gas this liquid turns into upon evaporation.
     * 
     * Requires corresponding Gas types (e.g., STEAM) to be added to ParticleType enum.
     * @return ParticleType The type of the resulting gas element.
     */
    virtual ParticleType getGasForm() const = 0;

    /**
     * @brief Checks if this liquid is flammable.
     * @return true if flammable, false otherwise. Defaults to false.
     */
    virtual bool isFlammable() const {
        return false;
    }


    // **=== Interaction Methods ===**

    /**
     * @brief Determines if this liquid can be displaced by another incoming element.
     * @param incomingType The type of the element trying to displace this liquid.
     * @param incomingDensity The density of the incoming element.
     * @return true if this liquid should be displaced, false otherwise.
     */
    virtual bool canBeDisplacedBy(ParticleType incomingType, float incomingDensity) const {
        // Displaced if the incoming element is denser
        return incomingDensity > this->getDensity();
    }


protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Attempts to perform standard liquid flow logic.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if the liquid successfully moved or swapped, false otherwise.
     */
    virtual bool attemptFlow(World& world, int r, int c); // Declaration only

    /**
     * @brief Attempts to evaporate the liquid based on temperature and conditions.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if evaporation occurred and element was replaced, false otherwise.
     */
    virtual bool attemptEvaporation(World& world, int r, int c); // Declaration only

};