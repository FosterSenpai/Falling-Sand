// ============================================================================
// Project:     Falling Sand Simulation
// File:        SandElement.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25
// Version:     1.0
// Description: Header file for the SandElement class. Represents sand particles.
//              Inherits from DynamicSolid.
// ============================================================================

#pragma once

#include "DynamicSolid.h" // Include the direct parent class definition

// Forward declaration (optional, but good practice if needed)
class World;

/**
 * @brief Represents a single particle of Sand.
 *
 * Inherits from DynamicSolid, implementing falling behaviour with diagonal slide.
 */
class SandElement : public DynamicSolid {
public:
    // **=== Constructors / Destructor ===**
    SandElement() = default; // Use default constructor
    virtual ~SandElement() = default; // Use default destructor

    // **=== Overridden Public Methods ===**
    // These override the pure virtual methods from the base classes.

    void update(World& world, int r, int c) override;
    sf::Color getColor() const override;
    ParticleType getType() const override;
    float getDensity() const override;
    float getHardness() const override;
    float getThermalConductivity() const override;
    float getMeltingPoint() const override;
    ParticleType getLiquidForm() const override;
    ParticleType getGasForm() const override;

    // We don't need to override canSlideDiagonally() because the default
    // implementation in DynamicSolid (return true) is correct for Sand.
    // We also don't need to override providesSupport() or isFlammable()
    // as the defaults from Solid (true and false respectively) are fine.

private:
    // **=== Private Members ===**
    // Add any state specific ONLY to SandElement here, if needed later.
};