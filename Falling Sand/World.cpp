// World.cpp
#include "World.h"   // Include the header for our class definition
#include <vector>    // Used for the grid members
#include <stdexcept> // Used for throwing exceptions on invalid input (optional)
#include <cstdlib>   // For rand() - will be needed in particle updates
#include <algorithm> // For std::max - might be needed later

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
    if (r >= 0 && r < m_rows && c >= 0 && c < m_cols) { 
    }
}

// -- Public Setters --

void World::setParticle(int r, int c, ParticleType type) {} // TODO: FILL

// **=== Private Methods ===**

// -- Check Helper Methods --

bool World::isInBounds(int r, int c) const
{
    bool rowInBounds = (r >= 0 && r < m_rows);
    bool colInBounds = (r >= 0 && c < m_cols);
    return rowInBounds && colInBounds;
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

void World::updateSand(int r, int c){} // TODO: FILL

void World::updateSandWet(int r, int c){} // TODO: FILL

void World::updateDirt(int r, int c){} // TODO: FILL

void World::updateGrass(int r, int c){} // TODO: FILL

void World::updateWater(int r, int c){} // TODO: FILL

void World::updateSilt(int r, int c){} // TODO: FILL

void World::updateOil(int r, int c){} // TODO: FILL
