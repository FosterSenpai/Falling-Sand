// ============================================================================
// Project:     Falling Sand Simulation
// File:        StaticSolid.h
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-25 // Updated date
// Version:     1.1 // Incremented version
// Description: Header file for the StaticSolid abstract class.
//              Inherits from Solid and serves as a base for solid elements
//              that are typically immovable unless specific conditions are met
//              (e.g., Dirt, Grass). Defines interfaces for support requirements
//              and environmental checks.
// ============================================================================

#pragma once

#include "Solid.h" // Include the base class definition

// Forward declaration of World
class World;

/**
 * @brief Abstract intermediate class representing static (generally immovable) Solids.
 *
 * Inherits the Solid interface. Base class for elements like Dirt and Grass
 * that typically remain fixed unless specific interaction rules apply.
 * Concrete derived classes must implement the pure virtual functions.
 */
class StaticSolid : public Solid {
public:
    // **=== Destructor ===**
    virtual ~StaticSolid() = default;


    // **=== Pure Virtual Public Methods (Inherited from Solid) ===**
    virtual void update(World& world, int r, int c) = 0;
    virtual sf::Color getColor() const = 0;
    virtual ParticleType getType() const = 0;
    virtual float getDensity() const = 0;
    virtual float getHardness() const = 0;
    virtual float getThermalConductivity() const = 0;
    virtual float getMeltingPoint() const = 0;
    virtual ParticleType getLiquidForm() const = 0;
    virtual ParticleType getGasForm() const = 0;


    // **=== Common Static Solid Properties (Virtual with Default) ===**
    // Inherits providesSupport() and isFlammable() from Solid

    /**
     * @brief Determines if this static solid requires support from below
     * to remain stable (e.g., some types might crumble if undermined).
     * @return false by default. Derived classes like Dirt might override this.
     */
    virtual bool requiresSupport() const {
        return false; // Default: Static solids don't require support
    }


protected:
    // **=== Protected Helper Methods ===**

    /**
     * @brief Performs common checks on the element's immediate environment.
     * Can be called by derived class's update() method to get information
     * about neighbors (e.g., the element above) to decide on reactions
     * like Dirt->Grass transformation.
     * (Implementation would go in StaticSolid.cpp or be defined inline).
     * @param world Reference to the world grid.
     * @param r Current row.
     * @param c Current column.
     */
    virtual void checkEnvironment(World& world, int r, int c); // Declaration only


private:
    // **=== Private Members ===**

};