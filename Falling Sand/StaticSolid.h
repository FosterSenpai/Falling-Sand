// ============================================================================
// Project:     Falling Sand Simulation
// File:        StaticSolid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.3
// Description: Header file for the StaticSolid abstract class.
//              Inherits from Solid and serves as a base for solid elements
//              that are typically immovable unless specific conditions are met
//              (e.g., Dirt, Grass). Defines interfaces for support requirements
//              and environmental checks.
// ============================================================================

#pragma once

#include "Solid.h"
#include "Particle.h"

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing static (generally immovable) Solids.
 *
 * Inherits the Solid interface. Base class for elements like Dirt and Grass
 * that typically remain fixed unless specific interaction rules apply.
 */
class StaticSolid : public Solid {
public:
    // **=== Destructor ===**

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup for derived classes (e.g., DirtElement).
     */
    virtual ~StaticSolid() = default;


    // **=== Pure Virtual Public Methods ===**

    /**
     * @brief Updates the static solid's state (e.g., environmental interactions).
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;

    /**
     * @brief Gets the display color of this static solid element.
     * @return sf::Color The color for rendering.
     */
    virtual sf::Color getColor() const = 0;

    /**
     * @brief Gets the specific type identifier for this static solid element.
     * @return ParticleType The enum value (e.g., ParticleType::DIRT).
     */
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this static solid element.
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;

    // Note: getHardness, getThermalConductivity, getMeltingPoint etc. are NOT
    // required by this intermediate class, only by concrete classes if needed.


    // **=== Common Static Solid Properties ===**

    /**
     * @brief Determines if this static solid requires support from below
     * to remain stable (e.g., some types might crumble if undermined).
	 * @return false by default. Derived classes that need support will override.
     */
    virtual bool requiresSupport() const {
        return false;
    }


protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Performs common checks on the element's immediate environment.
     * Can be called by derived class's update() method to get information
     * about neighbors (e.g., the element above) to decide on reactions.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     */
    virtual void checkEnvironment(World& world, int r, int c);

};