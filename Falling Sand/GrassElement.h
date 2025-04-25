// ============================================================================
// Project:     Falling Sand Simulation
// File:        GrassElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Header file for the GrassElement class. Represents grass.
//              Inherits from StaticSolid. Can turn back into Dirt if covered.
// ============================================================================

#pragma once

#include "StaticSolid.h" // Include the direct parent class definition

// Forward declaration
class World;

/**
 * @brief Represents a particle of Grass. Typically static.
 *
 * Can turn back into DirtElement if the cell above it becomes occupied.
 * Inherits from StaticSolid.
 */
class GrassElement : public StaticSolid {
public:
    // **=== Constructors / Destructor ===**
    GrassElement() = default; // Use default constructor
    virtual ~GrassElement() = default; // Use default destructor

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

    // Inherits providesSupport() (true), isFlammable() (false - default, maybe override?),
    // requiresSupport() (false) from Solid/StaticSolid.

private:
    // **=== Private Members ===**
    // Add any state specific ONLY to GrassElement here, if needed later.
    // e.g., time_covered counter? Use age? Let's keep it simple first.

    /**
     * @brief Chance (out of 100) per tick for grass to die and turn to dirt if covered.
     */
    static constexpr int GRASS_DEATH_CHANCE_PERCENT = 2; // Example: 2% chance per tick
};