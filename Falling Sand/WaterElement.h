// ============================================================================
// Project:     Falling Sand Simulation
// File:        WaterElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.2
// Description: Header file for the WaterElement class. Represents water.
//              Inherits from Liquid.
// ============================================================================

#pragma once

#include "Liquid.h"
#include "Particle.h"

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

    /** @brief Default constructor. */
    WaterElement();
    /** @brief Default virtual destructor. */
    virtual ~WaterElement() = default;

    // **=== Overridden Public Methods ===**

    /**
     * @brief Updates the water's state (flow, evaporation).
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    void update(World& world, int r, int c) override;

    /**
     * @brief Gets the display color for Water.
     * @return sf::Color The color of water.
     */
    sf::Color getColor() const override;

    /**
     * @brief Gets the type identifier for Water.
     * @return ParticleType The ParticleType::WATER enum value.
     */
    ParticleType getType() const override;

    /**
     * @brief Gets the density of Water.
     * @return float The density value for water (typically 1.0).
     */
    float getDensity() const override;

    /**
     * @brief Gets the dispersion rate for Water.
     * @return int How far water attempts to spread horizontally.
     */
    int getDispersionRate() const override;

    /**
     * @brief Gets the boiling point for Water.
     * @return float The boiling temperature (e.g., 100.0 C).
     */
    float getBoilingPoint() const override;

    /**
     * @brief Gets the gas form of Water (Steam).
     * @return ParticleType The ParticleType::STEAM enum value.
     */
    ParticleType getGasForm() const override;
};