// ============================================================================
// Project:     Falling Sand Simulation
// File:        Solid.h (Corrected)
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26 // Updated date
// Version:     1.5 // Incremented version
// Description: Header file for the Solid abstract class.
//              Inherits from Element and serves as a base for solid sub-types
//              (DynamicSolid, StaticSolid). Defines interfaces common
//              to all solid particles, primarily density. Specific properties
//              like hardness or melting point are left to concrete classes.
// ============================================================================

#pragma once

#include "Element.h" // Include the base class definition

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing the Solid state of matter.
 *
 * Inherits the Element interface. Serves as a base class for more specific
 * solid types like DynamicSolid and StaticSolid. Defines methods common
 * to all solids, primarily density.
 */
class Solid : public Element {
public:
    // **=== Destructor ===**
    virtual ~Solid() = default;


    // **=== Pure Virtual Public Methods (Inherited or Defined Here) ===**
    // These must be implemented by concrete derived classes (e.g., SandElement)
    virtual void update(World& world, int r, int c) = 0;
    virtual sf::Color getColor() const = 0;
    virtual ParticleType getType() const = 0;

    /**
     * @brief Gets the density of this solid element.
     * (Must be implemented by concrete derived classes).
     * @return float The density value (relative units).
     */
    virtual float getDensity() const = 0;


    // **=== Common Solid Properties (Virtual with Default) ===**

    /**
     * @brief Determines if this solid element provides support to elements directly above it.
     * @return true by default, indicating solids generally support weight.
     */
    virtual bool providesSupport() const {
        return true; // Default: Most solids provide support
    }

    /**
     * @brief Checks if this solid is flammable.
     * @return true if flammable, false otherwise. Defaults to false.
     */
    virtual bool isFlammable() const {
        return false; // Default: Most solids are not flammable
    }

    // Note: Hardness, ThermalConductivity, MeltingPoint, LiquidForm, GasForm
    // have been removed from this base class. They should be implemented directly
    // by concrete solid classes like SandElement, DirtElement, etc. as needed.

protected:
    // **=== Protected Members ===**

private:
    // **=== Private Members ===**

};