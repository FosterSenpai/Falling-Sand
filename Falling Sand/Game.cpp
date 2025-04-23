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
    m_windowWidth(1280),
    m_windowHeight(720),
    m_cellWidth(5.0f),
    m_gridCols(static_cast<int>(m_windowWidth / m_cellWidth)),
    m_gridRows(static_cast<int>(m_windowHeight / m_cellWidth)),

    // --- Initialize World ---
    m_world(m_gridRows, m_gridCols), // Construct world with calculated dimensions

    // --- Initialize other members ---
    m_isRunning(true),
    m_lastTimeForFPS(0.f),

    // --- UI ---
    m_font(),
    m_uiText(m_font)
{
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Load resources and setup initial state
    try {
        loadResources();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error loading resources: " << e.what() << std::endl;
        m_isRunning = false;
        return; // Stop initialization if resources failed
    }
    setupInitialState();

    std::cout << "Game Initialized: " << m_gridCols << "x" << m_gridRows << " grid." << std::endl;
}

// --- Setup Helpers (Called in constructor) ----

void Game::loadResources() {
    if (!m_font.openFromFile("PixelDigivolveItalic-dV8R.ttf")) {
        throw std::runtime_error("Failed to load font: PixelDigivolveItalic-dV8R.ttf");
    }

    // Setup UI text object properties
    m_uiText.setFont(m_font);
    m_uiText.setCharacterSize(20);
    m_uiText.setFillColor(sf::Color(80, 80, 80));
    m_uiText.setPosition({ 10.f, 10.f });
}

void Game::setupInitialState() {
    // Set initial brush settings
    m_brushSize = 5;
    m_brushType = ParticleType::SAND;

    // Initialize the vertex array for rendering the grid
    m_gridVertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    // Create the SFML window
    m_window.create(sf::VideoMode({ m_windowWidth, m_windowHeight }), "Falling Sand OOP");
    m_window.setFramerateLimit(60); // Limit FPS

    // Set initial UI text
	updateUIText(); // Make sure to call this after loading resources for font
}

// **=== Main Public Methods ===**

void Game::run() {
    while (m_window.isOpen() && m_isRunning)
    {
        // --- Timing & FPS calculation --
        float currentTime = m_clock.getElapsedTime().asSeconds();
		float deltaTime = (m_lastTimeForFPS > 0.f) ? (currentTime - m_lastTimeForFPS) : 0.f;
		m_lastTimeForFPS = currentTime;
		float fps = (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f;

		// Update window title with FPS
        m_window.setTitle("Falling Sand | FPS: " + std::to_string(static_cast<int>(fps)));

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

void Game::processEvents() {
    // Poll for all pending events accumulated since the last loop iteration
    while (const std::optional event = m_window.pollEvent())
    {
		// - Window 'x' button -
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
            m_isRunning = false;
        }

        // **=== Key Press Event ===**
        // Check if the event is a key being pressed down (discrete event, not hold)
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

            // **=== Window/App Controls ===**
            // Check if the Escape key was pressed
            if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
				// Close Window TODO: Turn this into a menu
                m_window.close();
                m_isRunning = false;
            }

            // **=== Brush Settings Adjustment ===**

			// -- Adjust Brush Size --
            if (keyPressed->scancode == sf::Keyboard::Scan::Hyphen) { // Decrease brush size key
                if (m_brushSize > 1) { // Ensure size doesn't go below 1
                    m_brushSize--;
                }
            }
            if (keyPressed->scancode == sf::Keyboard::Scan::Equal) { // Increase brush size key (often '+' requires shift)
                if (m_brushSize < 50) { // Limit maximum brush size
                    m_brushSize++;
                }
            }

            // -- Adjust Brush Particle Type --
            if (keyPressed->scancode == sf::Keyboard::Scan::Num0) { m_brushType = ParticleType::EMPTY; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num1) { m_brushType = ParticleType::SAND; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num2) { m_brushType = ParticleType::DIRT; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num3) { m_brushType = ParticleType::WATER; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num4) { m_brushType = ParticleType::SILT; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num5) { m_brushType = ParticleType::OIL; }
        }
    }
}

void Game::handleRealtimeInput() { /* Implementation... */ }
void Game::placeParticles(int mouseGridX, int mouseGridY) { /* Implementation... */ }
void Game::update() { /* Implementation... */ }
void Game::render() { /* Implementation... */ }
void Game::updateUIText() { /* Implementation... */ }
void Game::prepareVertices() { /* Implementation... */ }