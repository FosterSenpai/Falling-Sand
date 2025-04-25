// ============================================================================
// Project:     Falling Sand Simulation
// File:        World.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-25 // Updated Date
// Version:     1.1 // Updated Version
// Description: Header file for the World class.
//              Manages the grid of Elements and handles simulation updates,
//              providing interaction methods for Elements.
// ============================================================================

#pragma once

#include <vector>
#include <memory>       // For std::unique_ptr
#include "Particle.h"    // For ParticleType enum
#include "Element.h"     // Include base Element class

// Forward declare Element (already included, but good practice)
class Element;

// Struct to hold particle placement requests
struct PlacementRequest {
    int r;
    int c;
    ParticleType type;
};

class World
{
public:
    // **=== Constructors & Destructors ===**
    World(int numRows, int numCols);
    // Default destructor is likely fine as unique_ptr handles cleanup

    // **=== Public Methods ===**

    // -- Main Simulation --
    /**
     * @brief Advances the simulation state by one frame/tick.
     * Manages the update cycle for all elements in the grid.
     */
    void update();

    /**
	 * @brief Handles the placement of elements in the grid.
	 * @param r Row index for placement.
	 * @param c Column index for placement.
	 * @param type The ParticleType of the element to place.
     */
    void requestPlacement(int r, int c, ParticleType type);

    // -- Element Creation --
    /**
     * @brief Creates and places a new element of the specified type at (r, c)
     * in the main grid (m_grid), replacing any existing element.
     * @param r The row index.
     * @param c The column index.
     * @param type The ParticleType of the element to create.
     */
    void setElementByType(int r, int c, ParticleType type);

    // -- Getters --
    /**
     * @brief Gets a raw pointer to the element in the current grid (m_grid).
     * Returns nullptr if coordinates are out of bounds or cell is empty.
     * Pointer is for observation, ownership remains with the World.
     * @param r The row index.
     * @param c The column index.
     * @return Element* Pointer to the element, or nullptr.
     */
    Element* getElement(int r, int c) const;

     /**
     * @brief Gets a raw pointer to the element in the next grid (m_nextGrid).
     * Used for checking state during updates before it's finalized.
     * Returns nullptr if coordinates are out of bounds or cell is empty.
     * Pointer is for observation, ownership remains with the World.
     * @param r The row index.
     * @param c The column index.
     * @return Element* Pointer to the element, or nullptr.
     */
    Element* getElementFromNext(int r, int c) const;

    /**
     * @brief Gets the ParticleType of the element at (r, c) in the current grid.
     * Convenience function. Returns EMPTY if out of bounds or cell is empty.
     * @param r The row index.
     * @param c The column index.
     * @return ParticleType The type of the element.
     */
    ParticleType getElementType(int r, int c) const;

    /**
     * @brief Gets the current state of the simulation grid (m_grid).
     * @return Const reference to the grid of unique element pointers.
     */
    const std::vector<std::vector<std::unique_ptr<Element>>>& getGridState() const;

    // Get grid dimensions
    int getRows() const;
    int getCols() const;


    // **=== Methods for Element Interaction (used by Element::update) ===**

    /**
     * @brief Checks if the given coordinates are within the grid boundaries.
     * @param r The row index.
     * @param c The column index.
     * @return True if within bounds, false otherwise.
     */
    bool isWithinBounds(int r, int c) const;

    /**
     * @brief Attempts to move/swap element from (r_from, c_from) to (r_to, c_to).
     * Checks if the target cell in m_nextGrid is available or displaceable,
     * and if so, performs the move/swap atomically for this tick.
     * @return true if the move/swap was successfully performed, false otherwise.
     */
    bool tryMoveOrSwap(int r_from, int c_from, int r_to, int c_to);

    /**
     * @brief Moves the element from (r_from, c_from) in m_grid to (r_to, c_to)
     * in m_nextGrid. The original cell in m_nextGrid is implicitly left empty
     * unless something else moves there. Ownership is transferred via std::move.
     * Precondition: Target cell in m_nextGrid should be empty or prepared.
     * @param r_from Source row index.
     * @param c_from Source column index.
     * @param r_to Target row index.
     * @param c_to Target column index.
     */
    void moveElementToNext(int r_from, int c_from, int r_to, int c_to);

    /**
     * @brief Swaps the elements currently at (r1, c1) and (r2, c2) in m_grid,
     * placing them into the corresponding positions in m_nextGrid.
     * Handles ownership transfer via std::move.
     * @param r1 Row index of the first element.
     * @param c1 Column index of the first element.
     * @param r2 Row index of the second element.
     * @param c2 Column index of the second element.
     */
    void swapElementsInNext(int r1, int c1, int r2, int c2);

    /**
     * @brief Directly sets the element pointer for a cell in the next grid (m_nextGrid).
     * Takes ownership of the provided element pointer. Used internally by move/swap.
     * @param r The row index.
     * @param c The column index.
     * @param element A unique_ptr to the element to place.
     */
     void setNextElement(int r, int c, std::unique_ptr<Element> element);

    /**
     * @brief Clears a cell in the next grid (m_nextGrid), setting it to nullptr.
     * @param r The row index.
     * @param c The column index.
     */
    void clearNextGridCell(int r, int c);

    // **=== Element Factory (Example) ===**
    /**
     * @brief Creates a unique_ptr to a specific Element subclass based on type.
     * (Could be implemented in World.cpp)
     * @param type The ParticleType to create.
     * @return std::unique_ptr<Element> Pointer to the new element, or nullptr if type is invalid/EMPTY.
     */
    std::unique_ptr<Element> createElementByType(ParticleType type);


private:
    // **=== Member data ===**
    // Grids now store unique pointers to Element objects
    std::vector<std::vector<std::unique_ptr<Element>>> m_grid;      // Current state grid
    std::vector<std::vector<std::unique_ptr<Element>>> m_nextGrid;  // Grid for calculating next state
	std::vector<PlacementRequest> m_placementRequests;              // Requests for element placements

    // Dimensions
    int m_rows;
    int m_cols;

    // -- Flags --
    bool m_sweepRight = true; // Tracks the column update direction

    void wakeNeighbors(int r, int c);

    // **=== Private Methods ===**

    // Remove or refactor old private helpers like isNextGridCellEmpty, getNeighborType,
    // updateSand, updateWater, etc. The logic moves to Element classes.
    // The updateParticle dispatcher might remain but call element->update().

    /**
     * @brief Dispatches the update logic to the element at the given coordinates.
     * Reads the element from m_grid, checks flags (awake, updated), calls element->update().
     * @param r The row index of the particle to potentially update.
     * @param c The column index of the particle to potentially update.
     */
    void updateElement(int r, int c); // Renamed from updateParticle
};