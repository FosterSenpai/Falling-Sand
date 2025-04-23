// ============================================================================
// Project:     Falling Sand Simulation
// File:        Game.h
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-23
// Version:     1.0
// Description: Header file for the Game class. 
//              Handles the main game loop, window management, input handling,
//              UI display and rendering.
// ============================================================================
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "World.h" // Game contains a World
#include "Particle.h" // Include ParticleType definition

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
	sf::RenderWindow window; // The main window for rendering
	World world;             // The world object containing the grid of particles

	// **=== Game State & Settings ===**
    bool isRunning;
    int brushSize;
    ParticleType brushType;
	// TODO: Make the randomness of the brush placement a 'density' setting

    // -- Timing & FPS --
    sf::Clock clock;
	float lastTimeForFPS; // Time since last FPS update

	// -- Grid Variables --
    sf::VertexArray gridVertices; // The vertex array of the grid.
    float cellWidth;
    unsigned int windowWidth;
    unsigned int windowHeight;
    int gridCols; // Width / cellWidth
    int gridRows; // height / cellWidth

    // -- UI --
    sf::Font font;
    sf::Text uiText;

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
    void placeParticles(int mouseGridX, int mouseGridY); // Logic for placing particles

    /**
	 * @brief Updates the overall game state for the current frame. Calls the world update method and UI updates.
     */
    void update();

    /**
	 * @brief Renders the current game state to the window.
     */
    void render();

    /**
	 * @brief Updates the content of m_uiText with the ui information.
     */
    void updateUIText();

    /**
	 * @brief Prepares the vertex array for drawing the grid.
     */
    void prepareVertices();

    /**
	 * @brief Loads the resources needed for the game (fonts, textures, etc.). (Just font right now)
     */
    void loadResources();
    
    /**
	 * @brief Sets up the initial state of the game variables, window, and rendering components.
     */
    void setupInitialState();
};

