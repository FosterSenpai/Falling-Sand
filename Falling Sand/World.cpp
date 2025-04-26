// ============================================================================
// Project:     Falling Sand Simulation
// File:        World.cpp
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-26
// Version:     1.5
// Description: Implementation file for the World class. Manages the grid
//              of Elements and the simulation update cycle.
// ============================================================================

#include "World.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstdlib>

// **=== Element Includes ===**
#include "SandElement.h"
#include "Liquid.h"
#include "Gas.h"
#include "DirtElement.h"
#include "GrassElement.h"
#include "WaterElement.h"


// **=== Constructors & Destructors ===**

World::World(int numRows, int numCols) : m_rows(numRows), m_cols(numCols), m_sweepRight(true), m_surfaceHeights(numCols, numRows) {
    // Validate dimensions
    if (m_rows <= 0 || m_cols <= 0) {
        throw std::invalid_argument("World dimensions (rows, cols) must be positive.");
    }

    // --- Grid Initialization ---
    m_grid.resize(m_rows);
    m_nextGrid.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        m_grid[i].resize(m_cols);
        m_nextGrid[i].resize(m_cols);
    }
}

// **=== Public Getters ===**

int World::getSurfaceHeight(int c) const {
    if (c >= 0 && c < m_cols) {
        return m_surfaceHeights[c];
    }
    // Out of bounds, return a value indicating empty/bottom
    return m_rows;
}
int World::getRows() const { return m_rows; }
int World::getCols() const { return m_cols; }
const std::vector<std::vector<std::unique_ptr<Element>>>& World::getGridState() const { return m_grid; }
bool World::isWithinBounds(int r, int c) const { return (r >= 0 && r < m_rows && c >= 0 && c < m_cols); }
Element* World::getElement(int r, int c) const { if (isWithinBounds(r, c)) { return m_grid[r][c].get(); } else { return nullptr; } }
Element* World::getElementFromNext(int r, int c) const { if (isWithinBounds(r, c)) { return m_nextGrid[r][c].get(); } else { return nullptr; } }
ParticleType World::getElementType(int r, int c) const { Element* element = getElement(r, c); if (element) { return element->getType(); } else { return ParticleType::EMPTY; } }


// **=== Element Creation ===**

void World::setElementByType(int r, int c, ParticleType type) {
	if (!isWithinBounds(r, c)) { // Check bounds
        throw std::out_of_range("Coordinates [" + std::to_string(r) + "," + std::to_string(c) + "] are out of bounds in setElementByType.");
    }
    
	// Create a new element of the specified type
	std::unique_ptr<Element> newElement = createElementByType(type); // Create the element
	m_grid[r][c] = std::move(newElement);                            // Move it's unique_ptr to the grid
}


// **=== Main Simulation Update ===**

void World::update() {
    // --- Step 0: Process Placement Requests ---
    for (const auto& request : m_placementRequests) {
        setElementByType(request.r, request.c, request.type);
    }
    m_placementRequests.clear();

    // --- Step 1: Prepare for the new tick ---
	// Calculate surface heights for the current grid
    calculateSurfaceHeights();
	// Clear the next grid and reset update flags
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_grid[r][c]) {
                m_grid[r][c]->resetUpdateFlag();
            }
            m_nextGrid[r][c] = nullptr;
        }
    }

    // --- Step 2: Update active elements ---
    for (int r = m_rows - 1; r >= 0; --r) { // Iterate from bottom row upwards
        // Alternate column direction based on m_sweepRight
        if (m_sweepRight) {
            // Sweep Left-to-Right
            for (int c = 0; c < m_cols; ++c) {
                Element* element = m_grid[r][c].get();
                if (element && !element->isUpdatedThisTick()) {
                    element->update(*this, r, c);
                }
            }
        }
        else {
            // Sweep Right-to-Left
            for (int c = m_cols - 1; c >= 0; --c) {
                Element* element = m_grid[r][c].get();
                if (element && !element->isUpdatedThisTick()) {
                    element->update(*this, r, c);
                }
            }
        }
    }
    // Toggle sweep direction for the NEXT frame
    m_sweepRight = !m_sweepRight;

    // --- Step 3: Handle stationary elements ---
	// Copy any stationary elements from m_grid to m_nextGrid
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
    m_placementRequests.push_back({ r, c, type });
}

// **=== Element Interaction Methods ===**

void World::calculateSurfaceHeights() {
    for (int c = 0; c < m_cols; ++c) {
        m_surfaceHeights[c] = m_rows; // Default to bottom (empty column)
        for (int r = 0; r < m_rows; ++r) {
            if (m_grid[r][c]) { // Found the first non-empty cell from the top
                m_surfaceHeights[c] = r;
                break; // Stop scanning down this column
            }
        }
    }
}

bool World::tryMoveOrSwap(int r_from, int c_from, int r_to, int c_to) {
    // Bounds checks
    if (!isWithinBounds(r_from, c_from) || !isWithinBounds(r_to, c_to)) {
        return false;
    }

	// Check if the source cell is empty
    if (!m_grid[r_from][c_from]) {
        return false; // Cannot move nothing
    }

	// Get the element that is being moved
    Element* moverElement = m_grid[r_from][c_from].get();
    if (!moverElement) return false; // Safety check

    // Check original target in m_grid first
    Element* originalTargetElement = getElement(r_to, c_to);

    // Check target cell in NEXT grid (for conflict detection)
	Element* claimedNextElement = m_nextGrid[r_to][c_to].get(); // Get whats in the next grid target cell
	bool targetClaimed = claimedNextElement != nullptr;         // If empty, targetClaimed is false

    // --- Case A: Original Target was Empty ---
    if (!originalTargetElement) {
        // Can only move here if the spot hasn't already been claimed in nextGrid
        if (targetClaimed) {
			return false; // Block, target already claimed in nextGrid
        }

		// Perform the move
        m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]);
        wakeNeighbors(r_from, c_from);
        wakeNeighbors(r_to, c_to);
		if (m_nextGrid[r_to][c_to]) { m_nextGrid[r_to][c_to]->wakeUp(); } // Wake up the moved element
        return true;
    }

    // --- Case B: Original Target was Occupied ---
    else {
        // If the target spot in nextGrid is already claimed by something
        // *other* than the particle originally there, then it's a conflict.
        // This prevents overwriting a completed move/swap by another particle.
        if (targetClaimed && claimedNextElement != originalTargetElement) {
            return false; // Blocked by a third-party claim/move into the target spot
        }

        // If we passed the check above, the target spot in nextGrid is either
        // empty (targetClaimed is false) OR claimed by the original target element itself.

		// -- Check if original target is a displaceable Liquid --
        float moverDensity = 0.0f; // Get mover density safely
        bool densityObtained = false;
		// Get the density of the mover element for each type
        if (Solid* solidMover = dynamic_cast<Solid*>(moverElement)) { moverDensity = solidMover->getDensity(); densityObtained = true; }
        else if (Liquid* liquidMover = dynamic_cast<Liquid*>(moverElement)) { moverDensity = liquidMover->getDensity(); densityObtained = true; }
        else if (Gas* gasMover = dynamic_cast<Gas*>(moverElement)) { moverDensity = gasMover->getDensity(); densityObtained = true; }
        if (!densityObtained) return false; // Should not happen

		// Fluid density check for if original target is a fluid (Liquid or Gas)
        float targetDensity = 0.0f; // Get target density if fluid
        bool isFluid = false;
        if (Liquid* liquidTarget = dynamic_cast<Liquid*>(originalTargetElement)) { isFluid = true; targetDensity = liquidTarget->getDensity(); }
        else if (Gas* gasTarget = dynamic_cast<Gas*>(originalTargetElement)) { isFluid = true; targetDensity = gasTarget->getDensity(); }


        // --- Density Check ---
        if (isFluid && moverDensity > targetDensity) {
            // Perform the SWAP
			std::unique_ptr<Element> originalTargetPtr = std::move(m_grid[r_to][c_to]); // Store original target before moving
			m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]);                 // Move the mover to target cell
            // Only move target back if the source spot wasn't claimed by something else
            if (!m_nextGrid[r_from][c_from]) {
                m_nextGrid[r_from][c_from] = std::move(originalTargetPtr);   // Target takes mover's original spot in next
            }
            else {
                // Displaced fluid is lost if source spot taken. originalTargetPtr is deleted.
            }

            // Wake up relevant particles
            wakeNeighbors(r_from, c_from);
            wakeNeighbors(r_to, c_to);
            if (m_nextGrid[r_to][c_to]) { m_nextGrid[r_to][c_to]->wakeUp(); }
            if (m_nextGrid[r_from][c_from]) { m_nextGrid[r_from][c_from]->wakeUp(); }
            return true; // Swap succeeded
        }
        else {
            // Blocked by solid or denser/equal fluid
            return false;
        }
    }
}

// --- Other World Member Functions ---

void World::setNextElement(int r, int c, std::unique_ptr<Element> element) {
	if (isWithinBounds(r, c)) { // Check bounds
		m_nextGrid[r][c] = std::move(element); // Move the element into the next grid
    }
}

void World::clearNextGridCell(int r, int c) {
	if (isWithinBounds(r, c)) { // Check bounds
		m_nextGrid[r][c] = nullptr; // Clear the cell in the next grid
    }
}

void World::moveElementToNext(int r_from, int c_from, int r_to, int c_to) {
	if (!isWithinBounds(r_from, c_from) || !isWithinBounds(r_to, c_to)) { // Check bounds of both cells
		return; // Cannot move out of bounds
    }
	if (!m_grid[r_from][c_from]) { // Check if the source cell is empty
		return; // Cannot move nothing
    }
	m_nextGrid[r_to][c_to] = std::move(m_grid[r_from][c_from]); // Move the element to the next grid
}

void World::swapElementsInNext(int r1, int c1, int r2, int c2) {
    //TODO: Check if actually using this func
	if (!isWithinBounds(r1, c1) || !isWithinBounds(r2, c2)) { // Check bounds of both cells
        return;
    }
	m_nextGrid[r2][c2] = std::move(m_grid[r1][c1]); // Move the element from r1,c1 to r2,c2
	m_nextGrid[r1][c1] = std::move(m_grid[r2][c2]); // Move the element from r2,c2 to r1,c1
}

// **=== Factory for Creating Elements ===**

std::unique_ptr<Element> World::createElementByType(ParticleType type) {
	// Create a new element based on the ParticleType
    switch (type) {
    case ParticleType::EMPTY:   return nullptr;
    case ParticleType::SAND:    return std::make_unique<SandElement>();
    case ParticleType::DIRT:    return std::make_unique<DirtElement>();
    case ParticleType::GRASS:   return std::make_unique<GrassElement>();
    case ParticleType::WATER:   return std::make_unique<WaterElement>();
    default:                    return nullptr;
    }
}

void World::wakeNeighbors(int r, int c) {
	for (int dr = -2; dr <= 2; ++dr) {     // Check rows range -2 to 2
		for (int dc = -2; dc <= 2; ++dc) { // Check columns range -2 to 2
			if (dr == 0 && dc == 0) continue; // Skip self
			// Calculate neighbor coordinates
            int nr = r + dr;
            int nc = c + dc;
			Element* neighbor = getElement(nr, nc); // Get the neighbor element
			if (neighbor) {         // If the neighbor exists
				neighbor->wakeUp(); // Wake it up
            }
        }
    }
}