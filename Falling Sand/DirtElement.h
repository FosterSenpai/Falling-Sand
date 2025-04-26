// ============================================================================
// Project:     Falling Sand Simulation
// File:        DirtElement.h
// Author:      Foster Rae
// Date Created:2025-04-26
// Last Update: 2025-04-26
// Version:     1.1
// Description: Header file for the DirtElement class. Represents dirt.
//              Inherits from StaticSolid. Can turn into Grass if exposed
//              within a certain random depth from the surface.
// ============================================================================

#pragma once

#include "StaticSolid.h"
#include "Particle.h"

// Forward declaration
class World;

/**
 * @brief Represents a particle of Dirt. Typically static.
 *
 * Can turn into GrassElement if exposed to air (empty cells above)
 * within a randomly determined depth (2-6 layers) for a sufficient duration.
 * Inherits from StaticSolid.
 */
class DirtElement : public StaticSolid {
public:
    // **=== Constructors / Destructor ===**

    /**
     * @brief Constructor, initializes exposure timer.
    */
    DirtElement();
    /** @brief Default virtual destructor. */
    virtual ~DirtElement() = default;

    // **=== Overridden Public Methods ===**

    /**
     * @brief Updates the dirt's state, checking for grass growth conditions.
     * @param world A reference to the World object.
     * @param r The element's current row index.
     * @param c The element's current column index.
     */
    void update(World& world, int r, int c) override;

    /**
     * @brief Gets the display color for Dirt.
     * @return sf::Color The color of dirt.
     */
    sf::Color getColor() const override;

    /**
     * @brief Gets the type identifier for Dirt.
     * @return ParticleType The ParticleType::DIRT enum value.
     */
    ParticleType getType() const override;

    /**
     * @brief Gets the density of Dirt.
     * @return float The density value for dirt.
     */
    float getDensity() const override;

    // **=== Concrete Properties ===**

	/**
	 * @brief Gets the hardness of Dirt.
	 * @return float The hardness value for dirt.
	 */
    float getHardness() const;

	/**
	 * @brief Gets the thermal conductivity of Dirt.
	 * @return float The thermal conductivity value for dirt.
	 */
    float getThermalConductivity() const;

	/**
	 * @brief Gets the melting point of Dirt.
	 * @return float The melting point value for dirt.
	 */
    float getMeltingPoint() const;

	/**
	 * @brief Gets the liquid form of Dirt. TODO: (Molten Dirt or something)
	 * @return ParticleType The type of the resulting liquid element.
	 */
    ParticleType getLiquidForm() const;

    /**
	* @brief Gets the gas form of Dirt. (Doesn't have one)
	* @return ParticleType The type of the resulting gas element.
    */ 
    ParticleType getGasForm() const;

private:
    // **=== Private Members ===**

    /**
     * @brief Tracks how long (in ticks) this dirt particle has had empty
     * space directly above it continuously. Resets if covered.
     */
    int m_timeSinceExposed;

    /**
     * @brief Minimum time (in ticks) required exposure to air before grass can potentially grow.
     */
    static constexpr int GRASS_GROW_TIME_THRESHOLD = 150;

    /**
     * @brief Chance (out of 100) per tick to grow grass once threshold and depth checks pass.
     */
    static constexpr int GRASS_GROW_CHANCE_PERCENT = 2;

};