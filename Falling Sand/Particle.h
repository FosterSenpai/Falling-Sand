// ============================================================================
// Project:     Falling Sand Simulation
// File:        Particle.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-26
// Version:     1.1
// Description: Defines the ParticleType enumeration used to identify different
//              element types throughout the simulation.
// ============================================================================

#pragma once

// **=== Enums ===**

/**
 * @brief Enumeration defining all possible types of elements (particles)
 * in the simulation grid.
 */
enum class ParticleType {
    EMPTY,
    SAND,
    SANDWET,
    DIRT,
    GRASS,
    WATER,
    SILT,
    OIL,
    STEAM
	// **=== Add New Particle Types above ===**
    // 
    // TODO:
    // maybe a NONE type for functions like getGasForm if sublimation isn't standard
};