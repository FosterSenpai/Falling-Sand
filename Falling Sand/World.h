// ============================================================================
// Project:     Falling Sand Simulation
// File:        World.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-23
// Version:     1.0
// Description: Header file for the World class. 
//              Defines the World class, handles particle logic and interaction
//              with the grid (Setting particles, accessing grid). Handles
//              particle and grid updates.
// ============================================================================

#pragma once
#include <vector>
#include "Particle.h"

/**
 * @brief Struct to hold information about all the neighbors of a particle.
 */
struct NeighborhoodInfo {
    // Availability in the next grid (m_nextGrid)
    bool canMoveUp = false;
    bool canMoveDown = false;
    bool canMoveLeft = false;
    bool canMoveRight = false;
    bool canMoveDiagUL = false; // Up-Left
    bool canMoveDiagUR = false; // Up-Right
    bool canMoveDiagDL = false; // Down-Left
    bool canMoveDiagDR = false; // Down-Right

    // Initial types from the current grid (m_grid)
	// Default to a solid type for safety if accessors fail TODO: Make boundary type and use that
    ParticleType typeAbove = ParticleType::DIRT;
    ParticleType typeBelow = ParticleType::DIRT;
    ParticleType typeLeft = ParticleType::DIRT;
    ParticleType typeRight = ParticleType::DIRT;
    ParticleType typeDiagUL = ParticleType::DIRT;
    ParticleType typeDiagUR = ParticleType::DIRT;
    ParticleType typeDiagDL = ParticleType::DIRT;
    ParticleType typeDiagDR = ParticleType::DIRT;
};

class World
{
public:
	// **=== Constructors & Destructors ===**

    /**
	 * @brief Constructs a World object with a grid of the specified dimensions.
	 * @param numRows The number of rows in the grid.
	 * @param numCols The number of columns in the grid.
     */
    World(int numRows, int numCols);

	// **=== Public Methods ===**
    
    // -- Main Methods --
    
    /**
	 * @brief Advances the simulation state by one frame/tick. Processes physics and interactions 
     * for all particles in the grid and updates them.
     * 
     * Notes:
     * 
	 * This is the main simulation function. Handles the double buffering of the grid, updating the next grid
	 * and then swapping it with the main grid.
     * 
     * TODO:
     * 
	 * Implement alternating sweep direction for reading the grid, will help with directional bias. (HARD)
     */
    void update();
    
    // -- Setters --

    /**
	 * @brief Set the particle at the specified row and column to a new type.
	 * @param r The row index of the particle to set.
	 * @param c The column index of the particle to set.
	 * @param type The new particle type to set.
     */
    void setParticle(int r, int c, ParticleType type);
   
    // -- Getters --

    /**
	 * @brief Get the particle at the specified row and column.
	 * @param r The row index of the particle.
	 * @param c The column index of the particle.
     */
    Particle getParticle(int r, int c) const;
    /**
	 * @brief Get the current state of the grid.
	 * @return A reference to the grid containing particles.
     */
    const std::vector<std::vector<Particle>>& getGridState() const;

    // Get the number of rows in the grid
	int getRows() const; 
    // Get the number of columns in the grid
	int getCols() const; 


private:
	// **=== Member data ===**
    // - Grids -
    std::vector<std::vector<Particle>> m_grid;      // Main grid
    std::vector<std::vector<Particle>> m_nextGrid;  // Grid to calculate into and swap with

    // - Dimensions - 
    int m_rows;
    int m_cols;

	// **=== Private Methods ===**
    // -- Helper Methods --

    /**
     * @brief Checks if the given row and column indices are within the bounds of the grid.
     * @param r The row index to check.
     * @param c The column index to check.
     * @return True if the indices are within bounds, false otherwise.
     */
    bool isInBounds(int r, int c) const;

    /**
     * @brief Checks if the target cell is empty in the grid being built for the next frame (m_nextGrid).
     * @param r_target The target row index to check.
     * @param c_target The target column index to check.
     * @return True if the cell is within bounds and empty in m_nextGrid, false otherwise.
     */
    bool isNextGridCellEmpty(int r_target, int c_target) const;

    /**
	 * @brief Gets the neighborhood information for a particle at the specified row and column.
	 * @param r The row index of the particle.
	 * @param c The column index of the particle.
	 * @return A NeighborhoodInfo struct containing information about the surrounding particles.
     */
    NeighborhoodInfo getNeighborhoodInfo(int r, int c) const; // <-- Add declaration

    /**
     * @brief Gets the type of the particle at a relative offset (dr, dc) from (r, c).
     * @param r The current particle's row.
     * @param c The current particle's column.
     * @param dr The row offset (e.g., 1 for below, -1 for above, 0 for same row).
     * @param dc The column offset (e.g., 1 for right, -1 for left, 0 for same column).
     * @return The ParticleType of the neighbor, or a default boundary type if out of bounds.
     */
    ParticleType getNeighborType(int r, int c, int dr, int dc) const;

    // -- Main Private Methods --

    /**
	 * @brief Dispatches the update logic to the correct particle-specific update function based on the particle type at the given coordinates.
	 * @param r The row index of the particle to potentially update.
	 * @param c The column index of the particle to potentially update.
     * 
	 * Notes: 
     * 
     * This function reads the particle type from the current state grid (m_grid)
	 * and calls the corresponding private update helper (e.g., updateSand, updateWater).
     * 
     * It acts as a central router within the main World::update() loop.
     * 
     * The actual state changes are performed within the specific helper functions,
     * writing to m_nextGrid.
     */
    void updateParticle(int r, int c);

    // **=== Particle Logic ===**

    /**
     * @brief Handles physics for Sand particles: falling due to gravity, settling diagonally, and displacing liquids.
     * @param r The row index of the sand particle to update.
     * @param c The column index of the sand particle to update.
     */
    void updateSand(int r, int c);
    /**
     * @brief Handles physics for Wet Sand particles, including movement similar to sand and a chance to dry out.
     * @param r The row index of the wet sand particle to update.
     * @param c The column index of the wet sand particle to update.
     * 
     * TODO : Refine movement logic (e.g., make it 'stickier' than dry sand?), just holds a darker color for now.
     */
    void updateSandWet(int r, int c);
    /**
     * @brief Handles logic for Dirt particles, mainly static, grows grass on exposed surfaces.
     * @param r The row index of the dirt particle to update.
     * @param c The column index of the dirt particle to update.
     * 
	 * TODO : Add logic for dirt to fall from the bottom of a structure for a few seconds, then stay static.  
	 * Maybe add vines growing from the bottom of exposed dirt?
     */
    void updateDirt(int r, int c);
    /**
	 * @brief Handles logic for Grass particles, mainly static, turns into dirt if covered.
	 * @param r The row index of the grass particle to update.
	 * @param c The column index of the grass particle to update.
     */
    void updateGrass(int r, int c);
    /**
	 * @brief Handles logic for Water particles, falls, and spreads, other particles can displace it.
	 * @param r The row index of the water particle to update.
	 * @param c The column index of the water particle to update.
     */
    void updateWater(int r, int c);
    /**
	 * @brief Handles logic for Silt particles, which fall and can displace water. Moves down only.
	 * @param r The row index of the silt particle to update.
	 * @param c The column index of the silt particle to update.
     * 
	 * TODO : A bit boring, make it more interesting?
     */
    void updateSilt(int r, int c);
    /**
	 * @brief Handles logic for Oil particles, which can move diagonally and displace water. Like water but more viscous.
	 * @param r The row index of the oil particle to update.
	 * @param c The column index of the oil particle to update.
     */
    void updateOil(int r, int c);
};

