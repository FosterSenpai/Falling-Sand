// ============================================================================
// Project:     Falling Sand Simulation
// File:        Game.cpp
// Author:      Foster Rae
// Date Created:2025-04-23
// Last Update: 2025-04-26
// Version:     1.7 
// Description: Implementation file for the Game class.
//              Handles the main game loop, window management, input handling,
//              UI display and rendering logic.
// ============================================================================

#include "Game.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <ctime>

// **=== Constructors & Destructors ===**

Game::Game() :

    // --- Initialize constants and calculate derived values ---
    m_windowWidth(1280),
    m_windowHeight(720),
    m_cellWidth(4.0f),
    m_gridCols(static_cast<int>(m_windowWidth / m_cellWidth)),
    m_gridRows(static_cast<int>(m_windowHeight / m_cellWidth)),

    // --- Initialize World ---
    m_world(m_gridRows, m_gridCols),

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
    auto windowStyle = sf::Style::Titlebar | sf::Style::Close;
    m_window.create(sf::VideoMode({ m_windowWidth, m_windowHeight }), "Falling Sand OOP", windowStyle);
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
	// Poll for events in the window
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
            if (keyPressed->scancode == sf::Keyboard::Scan::Hyphen) { // Decrease brush size
                if (m_brushSize > 1) {
                    m_brushSize--;
                }
            }
            if (keyPressed->scancode == sf::Keyboard::Scan::Equal) { // Increase brush size
                if (m_brushSize < 50) {
                    m_brushSize++;
                }
            }

			// -- Change Brush Type on Number Key Press --
            if (keyPressed->scancode == sf::Keyboard::Scan::Num0) { m_brushType = ParticleType::EMPTY; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num1) { m_brushType = ParticleType::SAND; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num2) { m_brushType = ParticleType::DIRT; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num3) { m_brushType = ParticleType::WATER; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num4) { m_brushType = ParticleType::SILT; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num5) { m_brushType = ParticleType::OIL; }
            if (keyPressed->scancode == sf::Keyboard::Scan::Num6) { m_brushType = ParticleType::SANDWET; }

        }
    }
}

void Game::handleRealtimeInput() {

	// Spawn particles on mouse click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) // LMB
	{
		//Get mouse position in grid coordinates
		int mouseCol = static_cast<int>(std::floor(sf::Mouse::getPosition(m_window).x / m_cellWidth));
		int mouseRow = static_cast<int>(std::floor(sf::Mouse::getPosition(m_window).y / m_cellWidth));

		// Place particles based on the current brush settings
		placeParticles(mouseCol, mouseRow);
    }
}

void Game::placeParticles(int mouseGridX, int mouseGridY) {
    // Calculate extent of brush (Radius)
    int extent = static_cast<int>(std::floor(static_cast<int>(m_brushSize) / 2.0f));

    // Iterate through region around the mouse position on the grid
	for (int i = -extent; i <= extent; ++i) {     // Iterate rows within extent
        for (int j = -extent; j <= extent; ++j) { // Iterate cols within extent

            // -- Brush Density --
			if (rand() % 100 < Utils::getDensityForType(m_brushType)) { // Fires with a chance of the brush type density
				// Rows/cols brush is in
				int col = mouseGridX + j;
				int row = mouseGridY + i;


				if (row >= 0 && row < m_gridRows && col >= 0 && col < m_gridCols) { // If within bounds
					m_world.requestPlacement(row, col, m_brushType); // Request placement in the world
				}
			}
			// TODO : Could extend this logic here for different shaped brushes
        }
    }
}

void Game::update() {
    // Advance particle sim by one step
	m_world.update();

    // Update the UI
	updateUIText();
}

void Game::render() {
    // Prepare vertex array
    prepareVertices();
    // Clear the window
	m_window.clear(sf::Color::White); // Background is white
	// Draw the grid
	m_window.draw(m_gridVertices);
	// Draw the UI text
	m_window.draw(m_uiText);
	// Display the contents of the window
    m_window.display();
}

void Game::updateUIText() {
    // Get string for ui from brush type
	std::string particleTypeName = Utils::getNameForType(m_brushType);

	// Text to display
    std::string displayText = "BRUSH SETTINGS:\n"
		"Type: " + particleTypeName + "\n" +
        "Size: " + std::to_string(m_brushSize);

	// Set the UI text
    m_uiText.setString(displayText);
}

void Game::prepareVertices() {
    m_gridVertices.clear();
    const auto& currentGrid = m_world.getGridState();

    for (int r = 0; r < m_gridRows; ++r) {
        for (int c = 0; c < m_gridCols; ++c) {
            const std::unique_ptr<Element>& elementPtr = currentGrid[r][c];
            if (elementPtr) {
                Element* element = elementPtr.get();

                // *** CHANGE HERE ***
                // Get the specific varied color for this particle instance
                sf::Color particleColor = element->getRenderColor(); // Use the new getter

                float left = static_cast<float>(c) * m_cellWidth;
                float top = static_cast<float>(r) * m_cellWidth;
                float right = left + m_cellWidth;
                float bottom = top + m_cellWidth;

                // Create vertices with the varied color
                sf::Vertex topLeft(sf::Vector2f(left, top), particleColor);
                sf::Vertex topRight(sf::Vector2f(right, top), particleColor);
                sf::Vertex bottomLeft(sf::Vector2f(left, bottom), particleColor);
                sf::Vertex bottomRight(sf::Vector2f(right, bottom), particleColor);

                m_gridVertices.append(topLeft);
                m_gridVertices.append(topRight);
                m_gridVertices.append(bottomRight);
                m_gridVertices.append(topLeft);
                m_gridVertices.append(bottomRight);
                m_gridVertices.append(bottomLeft);
            }
        }
    }
}
