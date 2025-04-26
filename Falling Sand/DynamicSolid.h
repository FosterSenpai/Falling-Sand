// ============================================================================
// Project:     Falling Sand Simulation
// File:        DynamicSolid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25
// Version:     1.2
// Description: Header file for the DynamicSolid abstract class.
//              Inherits from Solid and serves as a base for solid elements
//              that are typically affected by gravity and can move
//              (e.g., Sand, Silt). Defines interfaces for movement behaviour.
// ============================================================================

#pragma once

#include "Solid.h"

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing dynamic (movable) Solids.
 *
 * Inherits the Solid interface. Base class for elements like Sand and Silt
 * that typically fall due to gravity. 
 *
 * Provides helpers and interfaces for fall/slide behaviour.
 */
class DynamicSolid : public Solid {
public:
    // **=== Destructor ===**
    virtual ~DynamicSolid() = default;


    // **=== Pure Virtual Public Methods (Inherited from Solid) ===**
    virtual void update(World& world, int r, int c) = 0;
    virtual sf::Color getColor() const = 0;
    virtual ParticleType getType() const = 0;
    virtual float getDensity() const = 0;
    virtual float getHardness() const = 0;
    virtual float getThermalConductivity() const = 0;
    virtual float getMeltingPoint() const = 0;
    virtual ParticleType getLiquidForm() const = 0;
    virtual ParticleType getGasForm() const = 0;


    // **=== Movement Behaviour Customization ===**

    /**
     * @brief Determines if this dynamic solid attempts to slide diagonally
     * when blocked directly below.
     * @return true by default (like Sand). Can be overridden by derived classes (like Silt).
     */
    virtual bool canSlideDiagonally() const {
        return true; // Default: Dynamic solids try to slide
    }


protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Attempts to perform standard dynamic solid falling logic.
     * Checks below, then potentially diagonally below (if canSlideDiagonally() is true),
     * attempting to move into empty space or displace lighter elements.
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     * @return true if the solid successfully moved or attempted to move, false otherwise.
     */
    virtual bool attemptFall(World& world, int r, int c);
};