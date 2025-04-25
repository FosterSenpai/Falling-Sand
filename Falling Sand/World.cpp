// World.cpp
#include "World.h"
#include <vector>
#include <stdexcept>
#include <cstdlib>   // For rand()
#include <algorithm> // For std::max

// **=== Constructors & Destructors ===**

World::World(int numRows, int numCols) : m_rows(numRows), m_cols(numCols) {
    // Throw exception if dimensions invalid (0 or less)
    if (m_rows <= 0 || m_cols <= 0) {
        throw std::invalid_argument("World dimensions (rows, cols) must be positive.");
    }

    // Initialize the grids. (FILLed with default constructed Particle (EMPTY))
    m_grid.resize(m_rows, std::vector<Particle>(m_cols, Particle{}));
    m_nextGrid.resize(m_rows, std::vector<Particle>(m_cols, Particle{}));
}

// **=== PUBLIC METHODS ===**

// -- Main Public Methods --

void World::update() 
{
	// Copy current state of m_grid to m_nextGrid
	m_nextGrid = m_grid;

	// Iterate over the grid and update each particle
	for (int row = 0; row < m_rows; ++row) {
		for (int col = 0; col < m_cols; ++col) {

			// Check if the particle type in the next grid is the same as in the current grid ( Hasn't been overwritten )
            if (m_nextGrid[row][col].type == m_grid[row][col].type)
            {
				// If it hasn't been overwritten, update the particle
                updateParticle(row, col);
            }
		}
	}
	// Update main grid to next grid
	m_grid = m_nextGrid;
}

// -- Public Getters --

int World::getRows() const {return m_rows;}
int World::getCols() const {return m_cols;}
const std::vector<std::vector<Particle>>& World::getGridState() const {return m_grid;}

Particle World::getParticle(int r, int c) const {
    if (isInBounds(r, c)) {
        return m_grid[r][c];
    }
    else {
		return Particle{}; // Return default EMPTY particle if out of bounds TODO: Update to return a boundary type
    }
}

// -- Public Setters --

void World::setParticle(int r, int c, ParticleType type) 
{
	if (isInBounds(r, c)) {
		m_grid[r][c].type = type;
	}
	else {
		throw std::out_of_range("Coordinates are out of bounds.");
	}
}

// **=== Private Methods ===**

// -- Check Helper Methods --

bool World::isInBounds(int r, int c) const
{
    bool rowInBounds = (r >= 0 && r < m_rows);
    bool colInBounds = (r >= 0 && c < m_cols);
    return rowInBounds && colInBounds;
}

bool World::isNextGridCellEmpty(int r_target, int c_target) const {
    if (!isInBounds(r_target, c_target)) {
        // Can't move into an out-of-bounds cell, so it's not "empty" for movement purposes.
        return false;
    }
	// Return true if the target cell in m_nextGrid is empty
    return (m_nextGrid[r_target][c_target].type == ParticleType::EMPTY);
}

NeighborhoodInfo World::getNeighborhoodInfo(int r, int c) const {
    NeighborhoodInfo info; // Create an instance of the struct

	// Check availability in m_nextGrid for neighbors
    info.canMoveUp = isNextGridCellEmpty(r - 1, c);
    info.canMoveDown = isNextGridCellEmpty(r + 1, c);
    info.canMoveLeft = isNextGridCellEmpty(r, c - 1);
    info.canMoveRight = isNextGridCellEmpty(r, c + 1);
    info.canMoveDiagUL = isNextGridCellEmpty(r - 1, c - 1);
    info.canMoveDiagUR = isNextGridCellEmpty(r - 1, c + 1);
    info.canMoveDiagDL = isNextGridCellEmpty(r + 1, c - 1);
    info.canMoveDiagDR = isNextGridCellEmpty(r + 1, c + 1);

	// Get initial types from m_grid for neighbors
    info.typeAbove = getNeighborType(r, c, -1, 0);
    info.typeBelow = getNeighborType(r, c, 1, 0);
    info.typeLeft = getNeighborType(r, c, 0, -1);
    info.typeRight = getNeighborType(r, c, 0, 1);
    info.typeDiagUL = getNeighborType(r, c, -1, -1);
    info.typeDiagUR = getNeighborType(r, c, -1, 1);
    info.typeDiagDL = getNeighborType(r, c, 1, -1);
    info.typeDiagDR = getNeighborType(r, c, 1, 1);

    return info; // Return the populated struct
}

ParticleType World::getNeighborType(int r, int c, int dr, int dc) const
{
	// Check if the neighbor coordinates are within bounds
	int neighborRow = r + dr;
	int neighborCol = c + dc;
    if (isInBounds(neighborRow, neighborCol)) {
		// If in bounds, return the type of the neighbor particle
		return m_grid[neighborRow][neighborCol].type;
    }
    else {
		return ParticleType::DIRT; // Treat out-of-bounds as solid TODO: Make boundary type
    }
}

// **=== Particle Logic ===**

void World::updateParticle(int r, int c)
{
	// Read the type of the particle at the given coordinates on m_grid
	ParticleType currentType = m_grid[r][c].type;

	// Switch to call the appropriate update function based on the particle type
    switch (currentType) {
    case ParticleType::EMPTY:
        // No action needed for empty cells
        break;
    case ParticleType::SAND:
        updateSand(r, c);
        break;
    case ParticleType::SANDWET:
        updateSandWet(r, c);
        break;
    case ParticleType::DIRT:
        updateDirt(r, c);
        break;
    case ParticleType::GRASS:
        updateGrass(r, c);
        break;
    case ParticleType::WATER:
        updateWater(r, c);
        break;
    case ParticleType::SILT:
        updateSilt(r, c);
        break;
    case ParticleType::OIL:
        updateOil(r, c);
        break;
    default:
        break;
    }
}

void World::updateSand(int r, int c)
{
}

void World::updateSandWet(int r, int c) { updateSand(r, c); } // TODO: FILL

void World::updateDirt(int r, int c){} // TODO: FILL

void World::updateGrass(int r, int c){} // TODO: FILL

void World::updateWater(int r, int c){} // TODO: FILL

void World::updateSilt(int r, int c){} // TODO: FILL

void World::updateOil(int r, int c){} // TODO: FILL
