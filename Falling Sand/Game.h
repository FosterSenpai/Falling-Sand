// Game.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "World.h" // Game contains a World

// Manages the overall application lifecycle, window, input, UI, timing, and orchestrates 
// interactions between other components(like World and rendering).

class Game
{
public:
    // -- Constructors & Destructors --
    Game();     // Constructor

    // -- Public Methods --
    void run(); // Main game loop entry point

private:
    // -- Core Components --
    sf::RenderWindow window;
    World world; // The simulation world instance

    // -- Game State & Settings --
    bool isRunning;
    int brushSize;
    ParticleType brushType;

    // -- Timing & FPS --
    sf::Clock clock;
    float lastTimeForFPS;

    // -- Rendering --
    sf::VertexArray gridVertices; // The vertex array of the grid.
    float cellWidth;
    unsigned int windowWidth;
    unsigned int windowHeight;
    int gridCols; // Width / cellWidth
    int gridRows; // height / cellWidth

    // -- UI --
    sf::Font font;
    sf::Text uiText;

    // -- Private Methods --
    void processEvents();       // Handle SFML events (keyboard, close, etc.)
    void handleRealtimeInput(); // Handle continuous input (mouse hold)
    void placeParticles(int mouseGridX, int mouseGridY); // Logic for placing particles
    void update();              // Update game state (world simulation, UI text)
    void render();              // Render the current state to the window
    void updateUIText();        // Update the UI text element
    void prepareVertices();     // Prepare the vertex array for drawing the grid
    void loadResources();       // Helper to load font, etc. in constructor
    void setupInitialState();   // Helper to set initial brush, etc. in constructor
};

