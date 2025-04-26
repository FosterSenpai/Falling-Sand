// ============================================================================
// Project:     Falling Sand Simulation
// File:        GrassElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1
// Description: Header file for the GrassElement class. Represents grass.
//              Inherits from StaticSolid. Can turn back into Dirt if covered.
// ============================================================================

#pragma once

#include "StaticSolid.h"
#include "Particle.h"

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

    /** @brief Default constructor. */
    GrassElement();
    /** @brief Default virtual destructor. */
    virtual ~GrassElement() = default;

    // **=== Overridden Public Methods ===**

    /**
     * @brief Updates the grass's state, checking if it should turn into dirt.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    void update(World& world, int r, int c) override;

    /**
     * @brief Gets the display color for Grass.
     * @return sf::Color The color of grass.
     */
    sf::Color getColor() const override;

    /**
     * @brief Gets the type identifier for Grass.
     * @return ParticleType The ParticleType::GRASS enum value.
     */
    ParticleType getType() const override;

    /**
     * @brief Gets the density of Grass.
     * @return float The density value for grass.
     */
    float getDensity() const override;

    // **=== Concrete Properties ===**

    /**
	 * @brief Gets the hardness of Grass.
	 * @return float The hardness value for grass (very low).
     */
    float getHardness() const;

	/**
	 * @brief Gets the thermal conductivity of Grass.
	 * @return float The thermal conductivity value (low).
	 */
    float getThermalConductivity() const;

    /**
	 * @brief Gets the "melting" point for Grass (more like combustion/decomposition).
	 * @return float The temperature at which grass might be destroyed or change state.
     */
    float getMeltingPoint() const;

    /**
	 * @brief Gets the ParticleType Grass turns into when "melted" (burned/decomposed).
	 * @return ParticleType The type of the resulting element (e.g., EMPTY or ASH).
     */
    ParticleType getLiquidForm() const;

    /**
	 * @brief Gets the ParticleType Grass turns into when it evaporates.
	 * @return ParticleType The type of the resulting element (e.g., EMPTY or ASH).
     */
    ParticleType getGasForm() const;

private:
    // **=== Private Members ===**

    /**
     * @brief Chance (out of 100) per tick for grass to die and turn to dirt if covered.
     */
    static constexpr int GRASS_DEATH_CHANCE_PERCENT = 2;

    /**
     * @brief Minimum time (in ticks) grass must be covered by non-grass/non-air before potentially dying.
     */
    static constexpr int GRASS_DEATH_TIME_THRESHOLD = 150;

    /**
     * @brief Tracks how long (in ticks) this grass particle has been covered
     * by a non-grass/non-air element continuously. Resets if uncovered or covered by grass.
     */
    int m_timeSinceCovered;
};