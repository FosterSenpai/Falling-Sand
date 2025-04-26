// ============================================================================
// Project:     Falling Sand Simulation
// File:        SandElement.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.1
// Description: Header file for the SandElement class. Represents sand particles.
//              Inherits from DynamicSolid.
// ============================================================================

#pragma once

#include "DynamicSolid.h"
#include "Particle.h"

// Forward declaration
class World;

/**
 * @brief Represents a single particle of Sand.
 *
 * Inherits from DynamicSolid, implementing falling behaviour with diagonal slide.
 * Defines concrete physical properties like density, hardness, melting point etc.
 */
class SandElement : public DynamicSolid {
public:
    // **=== Constructors / Destructor ===**

    /** @brief Default constructor. */
    SandElement();
    /** @brief Default virtual destructor. */
    virtual ~SandElement() = default;

    // **=== Overridden Public Methods ===**

    /**
     * @brief Updates the sand particle's state (falling/sliding).
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    void update(World& world, int r, int c) override;

    /**
     * @brief Gets the display color for Sand.
     * @return sf::Color The color of sand.
     */
    sf::Color getColor() const override;

    /**
     * @brief Gets the type identifier for Sand.
     * @return ParticleType The ParticleType::SAND enum value.
     */
    ParticleType getType() const override;

    /**
     * @brief Gets the density of Sand.
     * @return float The density value for sand.
     */
    float getDensity() const override;


    // **=== Concrete Properties ===**

    /**
     * @brief Gets the hardness of Sand.
     * @return float The hardness value for sand (relatively low).
     */
    float getHardness() const;

    /**
     * @brief Gets the thermal conductivity of Sand.
     * @return float The thermal conductivity value (low - insulator).
     */
    float getThermalConductivity() const;

    /**
     * @brief Gets the melting point for Sand (Silica).
     * @return float The melting temperature (very high).
     */
    float getMeltingPoint() const;

    /**
     * @brief Gets the ParticleType Sand turns into when melted (e.g., Molten Glass).
     * @return ParticleType The type of the resulting liquid element.
     */
    ParticleType getLiquidForm() const;

    /**
     * @brief Gets the ParticleType Sand turns into upon sublimation (N/A).
     * @return ParticleType Usually EMPTY or a NONE type.
     */
    ParticleType getGasForm() const;
};