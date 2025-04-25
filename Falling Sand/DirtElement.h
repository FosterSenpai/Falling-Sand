// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.0
// Description: Header file for the DirtElement class. Represents dirt.
//              Inherits from StaticSolid. Can turn into Grass if exposed.
// ============================================================================

#pragma once

#include "StaticSolid.h" // Include the direct parent class definition

// Forward declaration
class World;

/**
 * @brief Represents a particle of Dirt. Typically static.
 *
 * Can turn into GrassElement if the top 3 layers above it are empty
 * for a sufficient duration. Inherits from StaticSolid.
 */
class DirtElement : public StaticSolid {
public:
    // **=== Constructors / Destructor ===**
    DirtElement(); // Constructor to initialize timeSinceExposed
    virtual ~DirtElement() = default;

    // **=== Overridden Public Methods ===**
    void update(World& world, int r, int c) override;
    sf::Color getColor() const override;
    ParticleType getType() const override;
    float getDensity() const override;
    float getHardness() const override;
    float getThermalConductivity() const override;
    float getMeltingPoint() const override;
    ParticleType getLiquidForm() const override; // Magma/Lava?
    ParticleType getGasForm() const override;    // ??

    // Inherits providesSupport() (true), isFlammable() (false) from Solid
    // Inherits requiresSupport() (false) from StaticSolid (can override later if needed)

private:
    // **=== Private Members ===**

    /**
     * @brief Tracks how long (in ticks) this dirt particle has had empty
     * space directly above it continuously. Resets if covered.
     */
    int m_timeSinceExposed;

    /**
     * @brief Minimum time (in ticks) required exposure to air before grass can grow.
     */
    static constexpr int GRASS_GROW_TIME_THRESHOLD = 50;

    /**
     * @brief Chance (out of 100) per tick to grow grass once threshold is met.
     */
    static constexpr int GRASS_GROW_CHANCE_PERCENT = 60;
};
