// ============================================================================
// Project:     Falling Sand Simulation
// File:        World.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-26
// Version:     1.6
// Description: Header file for the World class.
//              Manages the grid of Elements and handles simulation updates,
//              providing interaction methods for Elements.
// ============================================================================

#pragma once

#include <vector>
#include <memory>
#include "Particle.h"
#include "Element.h"

// Forward declaration
class Element;

/**
 * @brief Structure to hold information for pending element placements.
 *
 * These requests decouple the user input from the simulation update cycle,
 * so when placement is requested, it doesn't immediately affect the grid.
 * This avoids issues with elements trying to interact with each other
 * straight away.
 */
struct PlacementRequest {
    int r;
    int c;
    ParticleType type;
};

/**
 * @brief Manages the simulation grid and element interactions.
 *
 * Handles the storage of elements using unique pointers in a double buffer,
 * handles the update cycle, and manages element placement requests.
 *
 * Provides methods for elements to query their neighbours and request moves/swaps.
 */
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

    // Defauld destructor is okay for now as unique_ptrs will handle cleanup themselves.

    // **=== Public Methods ===**

    // -- Main Simulation --
    /**
     * @brief Advances the simulation state by one frame/tick.
     * Manages the update cycle for all elements in the grid.
     */
    void update();

    // -- Element Placement --
    /**
     * @brief Requests placement of an element type at given coordinates.
     * The placement will be processed at the start of the next update cycle.
     * @param r The row index for placement.
     * @param c The column index for placement.
     * @param type The ParticleType of the element to request.
     */
    void requestPlacement(int r, int c, ParticleType type);

    /**
     * @brief Creates and places an element in the main grid (m_grid).
     *
     * Primarily used during initial setup or direct buffer processing.
     * @param r The row index.
     * @param c The column index.
     * @param type The ParticleType of the element to create.
     */
    void setElementByType(int r, int c, ParticleType type);


    // -- Getters --
     /**
     * @brief Get the pre-calculated surface height for a given column.
     * The surface height is the row index of the first non-empty cell from the top (0).
     * Returns numRows if the column is empty.
     * @param c The column index.
     * @return int The row index of the highest element, or numRows if empty.
     */
    int getSurfaceHeight(int c) const;

    /**
     * @brief Gets a pointer to the element in the current grid (m_grid).
     *
     * @param r The row index.
     * @param c The column index.
     * @return Element* Pointer to the element, or nullptr if out of bounds or empty.
     */
    Element* getElement(int r, int c) const;

    /**
     * @brief Gets a pointer to the element in the next grid state (m_nextGrid).
     *
     * Useful for checks within element update logic against the next state.
     * @param r The row index.
     * @param c The column index.
     * @return Element* Pointer to the element, or nullptr if out of bounds or empty.
     */
    Element* getElementFromNext(int r, int c) const;

    /**
     * @brief Gets the ParticleType of the element at (r, c) in the current grid.
     * @param r The row index.
     * @param c The column index.
     * @return ParticleType The type of the element. If out of bounds or empty, returns ParticleType::EMPTY.
     */
    ParticleType getElementType(int r, int c) const;

    /**
     * @brief Gets the current state of the simulation grid (m_grid).
     * @return Const reference to the grid of unique element pointers.
     */
    const std::vector<std::vector<std::unique_ptr<Element>>>& getGridState() const;

    /**
     * @brief Get the number of rows in the grid.
     * @return int The number of rows.
     */
    int getRows() const;

    /**
     * @brief Get the number of columns in the grid.
     * @return int The number of columns.
     */
    int getCols() const;


    // **=== Methods for Element Interaction ===**

    /**
     * @brief Checks if the given coordinates are within the grid boundaries.
     * @param r The row index.
     * @param c The column index.
     * @return True if within bounds, false otherwise.
     */
    bool isWithinBounds(int r, int c) const;

    /**
     * @brief Attempts to move/swap element from (r_from, c_from) in m_grid
     * to (r_to, c_to) in m_nextGrid. Checks nextGrid availability and original
     * occupant type/density to decide on move/swap/fail.
     * @param r_from Source row index.
     * @param c_from Source column index.
     * @param r_to Target row index.
     * @param c_to Target column index.
     * @return true if the move/swap was successfully performed, false otherwise.
     */
    bool tryMoveOrSwap(int r_from, int c_from, int r_to, int c_to);

    /**
     * @brief Moves the element pointer from m_grid[from] to m_nextGrid[to].
     * Assumes necessary checks have been made by caller.
     * @param r_from Source row index.
     * @param c_from Source column index.
     * @param r_to Target row index.
     * @param c_to Target column index.
     */
    void moveElementToNext(int r_from, int c_from, int r_to, int c_to); // *** Declaration Included ***

    /**
     * @brief Swaps element pointers between m_grid locations into m_nextGrid.
     * Assumes necessary checks have been made by caller.
     * @param r1 Row index of the first element location.
     * @param c1 Column index of the first element location.
     * @param r2 Row index of the second element location.
     * @param c2 Column index of the second element location.
     */
    void swapElementsInNext(int r1, int c1, int r2, int c2); // *** Declaration Included ***

    /**
     * @brief Directly sets the element pointer for a cell in the next grid (m_nextGrid).
     *
     * Used internally by other methods.
     * @param r The row index.
     * @param c The column index.
     * @param element A unique_ptr to the element to place (ownership transferred).
     */
    void setNextElement(int r, int c, std::unique_ptr<Element> element);

    /**
     * @brief Clears a cell in the next grid (m_nextGrid), setting it to nullptr.
     * @param r The row index.
     * @param c The column index.
     */
    void clearNextGridCell(int r, int c);

    /**
     * @brief Creates a unique_ptr to a specific Element subclass based on type. (Factory)
     * @param type The ParticleType to create.
     * @return std::unique_ptr<Element> Pointer to the new element, or nullptr.
     */
    std::unique_ptr<Element> createElementByType(ParticleType type);


private:
    // **=== Private Members ===**

    // -- Grids --
    /** @brief The main grid representing the current simulation state. */
    std::vector<std::vector<std::unique_ptr<Element>>> m_grid;
    /** @brief The grid used to calculate the next simulation state. */
    std::vector<std::vector<std::unique_ptr<Element>>> m_nextGrid;
    /** @brief Buffer for element placement requests from user input or other sources. */
    std::vector<PlacementRequest> m_placementRequests;

    // -- Dimensions --
    /** @brief Number of rows in the simulation grid. */
    int m_rows;
    /** @brief Number of columns in the simulation grid. */
    int m_cols;
    /** @brief Stores the calculated row index of the highest non-empty element in each column. */
    std::vector<int> m_surfaceHeights;

    // -- Update Logic State --
    /** @brief Tracks the column sweep direction for the update loop (alternates each frame). */
    bool m_sweepRight = true;


    // **=== Private Methods ===**

    /**
     * @brief Calculates the surface height for all columns and stores it in m_surfaceHeights.
     * Should be called once at the beginning of the World::update cycle.
     */
    void calculateSurfaceHeights();

    /**
     * @brief Wakes up elements in a neighborhood around the given cell.
     * Called after a move/swap to ensure neighbours react on the next tick.
     * @param r Central row index.
     * @param c Central column index.
     */
    void wakeNeighbors(int r, int c);
};