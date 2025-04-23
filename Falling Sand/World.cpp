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
void World::update() {} // TODO: FILL

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
void World::updateParticle(int r, int c){} // TODO: FILL

void World::updateSand(int r, int c){} // TODO: FILL

void World::updateSandWet(int r, int c){} // TODO: FILL

void World::updateDirt(int r, int c){} // TODO: FILL

void World::updateGrass(int r, int c){} // TODO: FILL

void World::updateWater(int r, int c){} // TODO: FILL

void World::updateSilt(int r, int c){} // TODO: FILL

void World::updateOil(int r, int c){} // TODO: FILL
