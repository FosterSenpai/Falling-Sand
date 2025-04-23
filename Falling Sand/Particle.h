#pragma once
// Holds particle type enum & particle struct
// --- Structs & Enums ---
enum class ParticleType {
    EMPTY,
    SAND,
    SANDWET,
    DIRT,
    GRASS,
    WATER,
    SILT,
    OIL,
};

struct Particle {
    // -- Particle Data --
    ParticleType type = ParticleType::EMPTY; // Default to empty, color tied to type
};