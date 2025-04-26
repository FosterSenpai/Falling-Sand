// ============================================================================
// Project:     Falling Sand Simulation
// File:        Solid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.6
// Description: Header file for the Solid abstract class.
//              Inherits from Element and serves as a base for solid sub-types
//              (DynamicSolid, StaticSolid). Defines interfaces common
//              to all solid particles, primarily density.
// ============================================================================

#pragma once

#include "Element.h"
#include "Particle.h"

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Solid state of matter.
 *
 * Inherits the Element interface. Serves as a base class for more specific
 * solid types like DynamicSolid and StaticSolid. Defines methods common
 * to all solids, mainly density.
 */
class Solid : public Element {
public:
    // **=== Destructor ===**

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup for derived classes.
     */
    virtual ~Solid() = default;


    // **=== Pure Virtual Public Methods ===**

    /**
     * @brief Updates the solid element's state.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    virtual void update(World& world, int r, int c) = 0;

    /**
     * @brief Gets the display color of this solid element.
     * @return sf::Color The color for rendering.
     */
    virtual sf::Color getColor() const = 0;

    /**
     * @brief Gets the specific type identifier for this solid element.
     * @return ParticleType The enum value (e.g., ParticleType::SAND).
     */
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this solid element.
     * 
     * Important for interactions, like displacement of liquids or buoyancy.
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;


    // **=== Common Solid Properties ===**

    /**
     * @brief Determines if this solid element provides support to elements directly above it.
     * @return true by default, solids generally support weight.
     */
    virtual bool providesSupport() const {
        return true; // Default: Most solids provide support
    }

    /**
     * @brief Checks if this solid is flammable.
     * @return true if flammable, false otherwise. Defaults to false.
     */
    virtual bool isFlammable() const {
        return false;
    }
};