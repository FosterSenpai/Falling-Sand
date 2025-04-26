// ============================================================================
// Project:     Falling Sand Simulation
// File:        Game.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-23
// Version:     1.7
// Description: Header file for the Game class. 
//              Handles the main game loop, window management, input handling,
//              UI display and rendering.
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "World.h"
#include "Particle.h"

class Game
{
public:
    // **=== Constructors & Destructors ===**

    /**
	 * @brief Constructs the game object, initializes the window, world, UI and other components.
     */
    Game();

	// **=== Public Methods ===**

    /**
	 * @brief Starts and runs the main game loop. continues until the window is closed.
     */
    void run();

private:
	// **=== Private Members ===**
    // -- Config / Base Variables --
    float m_cellWidth;
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;

    // -- Calculated Variables --
    int m_gridCols;
    int m_gridRows;

    // -- Core Components (Depend on calculated values) --
    sf::RenderWindow m_window;
    World m_world;

    // -- Game State & Settings --
    bool m_isRunning;
    int m_brushSize;
    ParticleType m_brushType;
	int m_brushDensity;

    // -- Timing & FPS --
    sf::Clock m_clock;
    float m_lastTimeForFPS;

    // -- Rendering --
    sf::VertexArray m_gridVertices;

    // -- UI --
    sf::Font m_font;
    sf::Text m_uiText;

	// **=== Private Methods ===**
    
    /**
	 * @brief Processes SFML events (keyboard, mouse, etc.)
     */
    void processEvents();

    /**
	 * @brief Handles continuous input (mouse hold) for placing particles.
     */
    void handleRealtimeInput();
    
    /**
	 * @brief Places particles in the world based on current brush settings.
	 * @param mouseGridX The grid column index where the mouse is located.
	 * @param mouseGridY The grid row index where the mouse is located.
     */
    void placeParticles(int mouseGridX, int mouseGridY);

    /**
	 * @brief Updates the overall game state for the current frame. Calls the world update method and UI updates.
     */
    void update();

    /**
	 * @brief Renders the current game state to the window.
     */
    void render();

    /**
	 * @brief Updates the UI with the current info.
     */
    void updateUIText();

    /**
	 * @brief Iterates through the grid and prepares the vertex array for rendering.
     */
    void prepareVertices();

    /**
	 * @brief Loads the resources needed for the game (fonts, textures, etc.), and sets up the UI.
     */
    void loadResources();
    
    /**
	 * @brief Sets up the initial state of the game variables, window, and rendering components.
     */
    void setupInitialState();
};

