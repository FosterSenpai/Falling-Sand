// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Header file for the WaterElement class. Represents water.
//              Inherits from Liquid.
// ============================================================================

#pragma once

#include "Liquid.h" // Include the direct parent class definition

// Forward declaration
class World;

/**
 * @brief Represents a particle of Water.
 *
 * Implements standard liquid flow and evaporation into Steam.
 * Inherits from Liquid.
 */
class WaterElement : public Liquid {
public:
    // **=== Constructors / Destructor ===**
    WaterElement() = default;
    virtual ~WaterElement() = default;

    // **=== Overridden Public Methods ===**
    // Implementations required by Element and Liquid interfaces

    void update(World& world, int r, int c) override;
    sf::Color getColor() const override;
    ParticleType getType() const override;
    float getDensity() const override;
    int getDispersionRate() const override;
    float getBoilingPoint() const override;
    ParticleType getGasForm() const override;

    // Inherits isFlammable() (false), canBeDisplacedBy() from Liquid

private:
    // **=== Private Members ===**
    // Add any state specific ONLY to WaterElement here, if needed later.
};