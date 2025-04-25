// ============================================================================
// Project:     Falling Sand Simulation
// File:        World.cpp
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-25 // Updated date
// Version:     1.2 // Updated version
// Description: Implementation file for the World class. Manages the grid
//              of Elements and the simulation update cycle.
// ============================================================================

#include "World.h"
#include <vector>
#include <memory>       // For std::unique_ptr, std::make_unique
#include <stdexcept>    // For std::invalid_argument, std::out_of_range
#include <utility>      // For std::move, std::swap
#include <cstdlib>      // For rand()

// Include headers for concrete Element types as they are created:
#include "SandElement.h"
#include "Liquid.h"
#include "Gas.h"
#include "DirtElement.h"
#include "GrassElement.h"
#include "WaterElement.h"
// ... etc ...


// **=== Constructors & Destructors ===**

World::World(int numRows, int numCols) : m_rows(numRows), m_cols(numCols), m_sweepRight(true) {
    // Validate dimensions
    if (m_rows <= 0 || m_cols <= 0) {
        throw std::invalid_argument("World dimensions (rows, cols) must be positive.");
    }

    // --- Revised Grid Initialization ---
    // Create the outer vector with the correct number of rows
    m_grid.resize(m_rows);
    m_nextGrid.resize(m_rows);

    // Initialize each inner vector (row) separately
    for (int i = 0; i < m_rows; ++i) {
        // Resize each row vector to the correct number of columns.
        // Elements (unique_ptr) will be default-constructed to nullptr.
        m_grid[i].resize(m_cols);
        m_nextGrid[i].resize(m_cols);
    }
    // --- End of Revised Grid Initialization ---
}

// **=== Public Getters ===**
// (getRows, getCols, getGridState, isWithinBounds, getElement, getElementFromNext, getElementType - implemented previously)
int World::getRows() const { return m_rows; }
int World::getCols() const { return m_cols; }
const std::vector<std::vector<std::unique_ptr<Element>>>& World::getGridState() const { return m_grid; }
bool World::isWithinBounds(int r, int c) const { return (r >= 0 && r < m_rows && c >= 0 && c < m_cols); }
Element* World::getElement(int r, int c) const { if (isWithinBounds(r, c)) { return m_grid[r][c].get(); } else { return nullptr; } }
Element* World::getElementFromNext(int r, int c) const { if (isWithinBounds(r, c)) { return m_nextGrid[r][c].get(); } else { return nullptr; } }
ParticleType World::getElementType(int r, int c) const { Element* element = getElement(r, c); if (element) { return element->getType(); } else { return ParticleType::EMPTY; } }


// **=== Element Creation ===**

/**
 * @brief Creates and places a new element of the specified type at (r, c)
 * in the main grid (m_grid), replacing any existing element.
 * @param r The row index.
 * @param c The column index.
 * @param type The ParticleType of the element to create.
 */
void World::setElementByType(int r, int c, ParticleType type) {
    if (!isWithinBounds(r, c)) {
        // Optionally throw an error or just return if out of bounds
        // For now, let's throw, consistent with old setParticle
        throw std::out_of_range("Coordinates [" + std::to_string(r) + "," + std::to_string(c) + "] are out of bounds in setElementByType.");
        // return;
    }

    // Use the factory method to create the element
    std::unique_ptr<Element> newElement = createElementByType(type);

    // Move the new element into the main grid, replacing the old one.
    // std::move transfers ownership of the pointer.
    m_grid[r][c] = std::move(newElement);

    // Ensure the corresponding nextGrid cell is cleared if we replace mid-update?
    // Or assume this is usually called during setup/input handling.
    // For safety, clearing nextGrid might be good IF setElementByType
    // could be called during the main update loop externally.
    // m_nextGrid[r][c] = nullptr;
}


// **=== Private Methods ===**

// Inside World.cpp

// ... (Includes, Constructor, Getters, Element Creation, Interaction Methods like move/swap) ...


// **=== Main Simulation Update ===**

/**
 * @brief Advances the simulation state by one frame/tick.
 * Manages the update cycle for all elements in the grid using double buffering.
 */
void World::update() {
    // --- Step 0: Process Placement Requests ---
    // Apply requested placements BEFORE the simulation tick starts.
    // This places them directly into the current m_grid.
    for (const auto& request : m_placementRequests) {
        setElementByType(request.r, request.c, request.type); // Use existing method
    }
    m_placementRequests.clear(); // Clear buffer for next frame

    // --- Step 1: Prepare for the new tick ---

    // Reset update flags for all elements in the current grid (m_grid)
    // Also clear the nextGrid entirely to ensure it starts empty.
    // (We could combine clearing nextGrid with the reset loop later if needed)
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c]) { // Check if element exists
                m_grid[r][c]->resetUpdateFlag();
            }
            // Ensure next grid starts clean for this tick
            // (Could also be done after the swap at the end of the *previous* tick)
            m_nextGrid[r][c] = nullptr;
        }
    }


    // --- Step 2: Update active elements ---
    // Iterate BOTTOM-UP
    for (int r = m_rows - 1; r >= 0; --r) {
        // ALTERNATE column direction based on the flag
        if (m_sweepRight) {
            // Sweep Left-to-Right
            for (int c = 0; c < m_cols; ++c) {
                Element* element = m_grid[r][c].get();
                if (element && element->isAwake() && !element->isUpdatedThisTick()) {
                    element->update(*this, r, c);
                }
            }
        }
        else {
            // Sweep Right-to-Left
            for (int c = m_cols - 1; c >= 0; --c) { // Note the loop changes
                Element* element = m_grid[r][c].get();
                if (element && element->isAwake() && !element->isUpdatedThisTick()) {
                    element->update(*this, r, c);
                }
            }
        }
    }

    // Toggle sweep direction for the NEXT frame
    m_sweepRight = !m_sweepRight;

    // --- Step 3: Handle stationary elements ---
    // (This loop can remain left-to-right, its order is less critical)
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c] && !m_nextGrid[r][c]) {
                m_nextGrid[r][c] = std::move(m_grid[r][c]);
            }
        }
    }


    // --- Step 4: Swap grids ---
    m_grid.swap(m_nextGrid);
}

void World::requestPlacement(int r, int c, ParticleType type) {
    // Could add checks here if needed (e.g., limit buffer size)
    m_placementRequests.push_back({ r, c, type });
}

// **=== Element Interaction Methods (Implementation) ===**
// Inside World::tryMoveOrSwap in World.cpp

bool World::tryMoveOrSwap(int r_from, int c_from, int r_to, int c_to) {
    // 1. Bounds checks for both source and destination
    if (!isWithinBounds(r_from, c_from) || !isWithinBounds(r_to, c_to)) {
        // std::cerr << "Debug: tryMoveOrSwap failed (Out of Bounds) for " << r_from << "," << c_from << " -> " << r_to << "," << c_to << std::endl;
        return false;
    }

    // 2. Check if source element exists in the current grid (m_grid)
    if (!m_grid[r_from][c_from]) {
        // std::cerr << "Debug: tryMoveOrSwap failed (Source Element is Null) at " << r_from << "," << c_from << std::endl;
        return false; // Cannot move nothing
    }
    Element* moverElement = m_grid[r_from][c_from].get(); // Get raw pointer for later use

    // 3. Check target cell in NEXT grid **atomically**
    // If m_nextGrid[r_to][c_to] is ALREADY occupied by something this tick, fail.
    if (m_nextGrid[r_to][c_to]) {
        // std::cerr << "Debug: tryMoveOrSwap failed (Target [" << r_to << "," << c_to << "] in nextGrid already claimed by Type "
        //           << static_cast<int>(m_nextGrid[r_to][c_to]->getType()) << ") when called by (" << r_from << "," << c_from << ")" << std::endl;
        return false; // Target spot already claimed this tick
    }

    // 4. Target spot in m_nextGrid is free! Check what was originally
    //    in the target cell in m_grid to see if it's empty, fluid, or solid.
    Element* originalTargetElement = getElement(r_to, c_to); // Check m_grid

    // --- Case A: Original Target was Empty ---
    if (!originalTargetElement) {
        // Perform the move: Mover goes from m_grid[from] to m_nextGrid[to]
        m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]);

        // --- Wake up relevant particles ---
        wakeNeighbors(r_from, c_from); // Wake neighbors around original spot
        wakeNeighbors(r_to, c_to);     // Wake neighbors around new spot
        if (m_nextGrid[r_to][c_to]) {    // Wake up the element that just moved
            m_nextGrid[r_to][c_to]->wakeUp();
        }
        // std::cout << "Debug: tryMoveOrSwap SUCCESS (Moved into empty) from (" << r_from << "," << c_from << ") to (" << r_to << "," << c_to << ")" << std::endl;
        return true;
    }
    // --- Case B: Original Target was Occupied ---
    else {
        // Get mover's density SAFELY
        float moverDensity = 0.0f;
        bool densityObtained = false;
        if (Solid* solidMover = dynamic_cast<Solid*>(moverElement)) { moverDensity = solidMover->getDensity(); densityObtained = true; }
        else if (Liquid* liquidMover = dynamic_cast<Liquid*>(moverElement)) { moverDensity = liquidMover->getDensity(); densityObtained = true; }
        else if (Gas* gasMover = dynamic_cast<Gas*>(moverElement)) { moverDensity = gasMover->getDensity(); densityObtained = true; }

        if (!densityObtained) {
            // This element type doesn't participate in density-based displacement
             // std::cerr << "Debug: tryMoveOrSwap failed (Mover has no density?) at (" << r_from << "," << c_from << ")" << std::endl;
            return false;
        }

        // Check if original target is a displaceable fluid
        float targetDensity = 0.0f;
        bool isFluid = false;
        if (Liquid* liquidTarget = dynamic_cast<Liquid*>(originalTargetElement)) { isFluid = true; targetDensity = liquidTarget->getDensity(); }
        else if (Gas* gasTarget = dynamic_cast<Gas*>(originalTargetElement)) { isFluid = true; targetDensity = gasTarget->getDensity(); }

        if (isFluid && moverDensity > targetDensity) {
            // Perform the swap: Mover from m_grid[from] -> m_nextGrid[to]
            //                   Target from m_grid[to]   -> m_nextGrid[from]
            m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]);
            m_nextGrid[r_from][c_from] = std::move(m_grid[r_to][c_to]); // Get original target from m_grid

            // --- Wake up relevant particles ---
            wakeNeighbors(r_from, c_from); // Wake neighbors around original spot of mover
            wakeNeighbors(r_to, c_to);     // Wake neighbors around original spot of target
            if (m_nextGrid[r_to][c_to]) {    // Wake up the element that moved to target
                m_nextGrid[r_to][c_to]->wakeUp();
            }
            if (m_nextGrid[r_from][c_from]) { // Wake up the element that was displaced to origin
                m_nextGrid[r_from][c_from]->wakeUp();
            }
            // std::cout << "Debug: tryMoveOrSwap SUCCESS (Swapped with fluid) involving (" << r_from << "," << c_from << ") and (" << r_to << "," << c_to << ")" << std::endl;
            return true;
        }
        else {
            // Target was solid or denser/equal fluid - cannot displace
            // std::cerr << "Debug: tryMoveOrSwap FAILED (Target is Solid or denser fluid) at (" << r_to << "," << c_to << ") called by (" << r_from << "," << c_from << ")" << std::endl;
            return false;
        }
    }
}

void World::setNextElement(int r, int c, std::unique_ptr<Element> element) {
    if (isWithinBounds(r, c)) {
        m_nextGrid[r][c] = std::move(element);
    }
    // Else: Out of bounds - maybe log an error? For now, do nothing.
}

void World::clearNextGridCell(int r, int c) {
    if (isWithinBounds(r, c)) {
        m_nextGrid[r][c] = nullptr;
    }
}

void World::moveElementToNext(int r_from, int c_from, int r_to, int c_to) {
    if (!isWithinBounds(r_from, c_from) || !isWithinBounds(r_to, c_to)) {
        // Log error or handle invalid move?
        return;
    }
    // Check if source element exists
    if (!m_grid[r_from][c_from]) {
        // Trying to move an empty cell? Log error or ignore.
        return;
    }

    // Move the unique_ptr from the current grid's source cell
    // to the next grid's target cell. This transfers ownership.
    // The unique_ptr at m_grid[r_from][c_from] becomes nullptr after the move.
    m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]);

    // We don't explicitly clear m_nextGrid[r_from][c_from] here,
    // because something else might move *into* that spot later in the tick.
    // If nothing moves into it, it should remain nullptr (its default state).
}

void World::swapElementsInNext(int r1, int c1, int r2, int c2) {
    if (!isWithinBounds(r1, c1) || !isWithinBounds(r2, c2)) {
        // Log error or handle invalid swap?
        return;
    }

    // Check if source elements exist (optional, depends on calling context)
    // if (!m_grid[r1][c1] || !m_grid[r2][c2]) { return; }

   // Move element from (r1, c1) in m_grid -> (r2, c2) in m_nextGrid
    m_nextGrid[r2][c2] = std::move(m_grid[r1][c1]);

    // Move element from (r2, c2) in m_grid -> (r1, c1) in m_nextGrid
    m_nextGrid[r1][c1] = std::move(m_grid[r2][c2]);

}

std::unique_ptr<Element> World::createElementByType(ParticleType type) {
    switch (type) {
    case ParticleType::EMPTY:
        return nullptr; // Represent empty cells with nullptr

        // --- Add cases for each concrete element type as they are created ---

        case ParticleType::SAND:
            return std::make_unique<SandElement>();
        case ParticleType::DIRT:
            return std::make_unique<DirtElement>();
        case ParticleType::GRASS:
            return std::make_unique<GrassElement>();
        case ParticleType::WATER:
			return std::make_unique<WaterElement>();
		/*
        case ParticleType::WATER:
            // Requires WaterElement class to be defined and WaterElement.h included
            // return std::make_unique<WaterElement>();
            return nullptr; // Placeholder

        case ParticleType::DIRT:
             // Requires DirtElement class ...
             // return std::make_unique<DirtElement>();
             return nullptr; // Placeholder

        case ParticleType::GRASS:
             // Requires GrassElement class ...
             // return std::make_unique<GrassElement>();
             return nullptr; // Placeholder

        case ParticleType::SANDWET:
             // Requires SandWetElement class ...
             // return std::make_unique<SandWetElement>();
             return nullptr; // Placeholder

        case ParticleType::SILT:
             // Requires SiltElement class ...
             // return std::make_unique<SiltElement>();
             return nullptr; // Placeholder

        case ParticleType::OIL:
             // Requires OilElement class ...
             // return std::make_unique<OilElement>();
             return nullptr; // Placeholder
        */

        // Add cases for future types (STEAM, STONE, WOOD, etc.) here

    default:
        // Handle unknown types - return nullptr or throw an error?
        // Returning nullptr is safer if new types might be added without updating this.
        return nullptr;
    }
}

/**
 * @brief Wakes up elements in a larger neighborhood around the given cell.
 * Used to ensure nearby elements re-evaluate their state after movement occurs.
 * @param r Central row index.
 * @param c Central column index.
 */
void World::wakeNeighbors(int r, int c) {
    // Iterate through a 5x5 neighborhood centered on (r, c)
    for (int dr = -2; dr <= 2; ++dr) { // <--- Changed radius to 2
        for (int dc = -2; dc <= 2; ++dc) { // <--- Changed radius to 2
            // Skip the center cell itself
            if (dr == 0 && dc == 0) continue;

            int nr = r + dr;
            int nc = c + dc;

            // Check bounds and get element from the *current* grid (m_grid)
            Element* neighbor = getElement(nr, nc); // Use existing getter
            if (neighbor) { // If neighbor exists
                neighbor->wakeUp(); // Call its wakeUp method
            }
        }
    }
}
