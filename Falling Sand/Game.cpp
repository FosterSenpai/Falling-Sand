#include "Game.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>  // For std::cout, std::cerr
#include <stdexcept> // For handling errors like font loading
#include <cmath>     // For std::floor
#include <ctime>     // For time() used in srand()

// **=== Constructors & Destructors ===**

Game::Game() :

    // --- Initialize constants and calculate derived values ---
    windowWidth(1280),
    windowHeight(720),
    cellWidth(5.0f),
    gridCols(static_cast<int>(windowWidth / cellWidth)),
    gridRows(static_cast<int>(windowHeight / cellWidth)),

    // --- Initialize World ---
    world(gridRows, gridCols), // Construct world with calculated dimensions

    // --- Initialize other members ---
    isRunning(true),
    lastTimeForFPS(0.f),

    // --- UI ---
    font(),
    uiText(font)
{
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Load resources and setup initial state
    try {
        loadResources();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error loading resources: " << e.what() << std::endl;
        isRunning = false;
        return; // Stop initialization if resources failed
    }
    setupInitialState();

    std::cout << "Game Initialized: " << gridCols << "x" << gridRows << " grid." << std::endl;
}

// --- Setup Helpers (Called in constructor) ----

void Game::loadResources() {
    if (!font.openFromFile("PixelDigivolveItalic-dV8R.ttf")) {
        throw std::runtime_error("Failed to load font: PixelDigivolveItalic-dV8R.ttf");
    }

    // Setup UI text object properties
    uiText.setFont(font);
    uiText.setCharacterSize(20);
    uiText.setFillColor(sf::Color(80, 80, 80));
    uiText.setPosition({ 10.f, 10.f });
}

void Game::setupInitialState() {
    // Set initial brush settings
    brushSize = 5;
    brushType = ParticleType::SAND;

    // Initialize the vertex array for rendering the grid
    gridVertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    // Create the SFML window
    window.create(sf::VideoMode({ windowWidth, windowHeight }), "Falling Sand OOP");
    window.setFramerateLimit(60); // Limit FPS

    // Set initial UI text
	updateUIText(); // Make sure to call this after loading resources for font
}

// **=== Main Public Methods ===**

void Game::run()
{
    while (window.isOpen() && isRunning)
    {
        // --- Timing & FPS calculation --
        float currentTime = clock.getElapsedTime().asSeconds();
		float deltaTime = (lastTimeForFPS > 0.f) ? (currentTime - lastTimeForFPS) : 0.f;
		lastTimeForFPS = currentTime;
		float fps = (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f;

		// Update window title with FPS
        window.setTitle("Falling Sand | FPS: " + std::to_string(static_cast<int>(fps)));

        // **=== Game Loop Phases ===**
        
        // 1. Handle discrete window/keyboard events
        processEvents();

        // 2. Handle continuous real-time input (like mouse being held down)
        handleRealtimeInput();

        // 3. Update the game state (run simulation step, update UI text)
        update();

        // 4. Render the current state to the screen
        render();
    }
}

// **=== Private Methods ===**

void Game::processEvents() { /* Implementation... */ }
void Game::handleRealtimeInput() { /* Implementation... */ }
void Game::placeParticles(int mouseGridX, int mouseGridY) { /* Implementation... */ }
void Game::update() { /* Implementation... */ }
void Game::render() { /* Implementation... */ }
void Game::updateUIText() { /* Implementation... */ }
void Game::prepareVertices() { /* Implementation... */ }