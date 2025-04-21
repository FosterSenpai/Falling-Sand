/*
TODO:

make ui for changing sand type, make other materials (clickable buttons or something)
    make interactions for different materials

Particle spawners? 

if grass been around long enough and above still empty grow flower/vine

Break up to follow OOP

    world class
        init grids
        handles grid update
        set + get particle types of cells

    Game class
        Setup window + variables
        setup clock + variables
        hold cellWidth
        setup brush UI
        setup brush settings
        handle event processing
            key press
            realtime input (mouse click)
        handle placing particles function to be called on click
        handle update of brushUI
        Prepare vetices (same as before)

        render func
            // Prepare vertices based on the current world state
            prepareVerticesInternal(); // Helper to keep render clean

            window.clear(sf::Color::White);
            window.draw(gridVertices);
            window.draw(uiText);
            window.display();

        run func that is main loop
            void run() {
             srand(static_cast<unsigned int>(time(0))); // Seed random
             float lastTimeForFPS = 0.f; // For FPS calculation only

            while (window.isOpen()) {
                 // --- FPS Calculation --- (Can be simplified if not needed)
                float currentTime = clock.getElapsedTime().asSeconds();
                float deltaTime = currentTime - lastTimeForFPS;
                lastTimeForFPS = currentTime;
                float fps = (deltaTime > 0) ? (1.f / deltaTime) : 0.f;
                // -----------------------

                processEvents();         // Handle window events, key presses
                handleRealtimeInput();   // Handle mouse hold
                update();                // Update simulation and UI text
                render();                // Draw everything
            }

    Utils class
        get color for particle helper func


*/
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

// --- Structs & Enums ---
enum class ParticleType {
    EMPTY,
    SAND,
    DIRT,
    GRASS,
};

struct Particle{
    // -- Particle Data --
    ParticleType type = ParticleType::EMPTY; // Default to empty, color tied to type
};

// --- Functions ---
/**
 * @brief Updates the sands positions on the grids, handles gravity etc.
 * @param grid : The main grid.
 * @param nextGrid : The calculation grid.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 */
void updateGrid(std::vector<std::vector<Particle>>& grid, std::vector<std::vector<Particle>>& nextGrid, int rows, int cols, ParticleType brushType);
/**
 * @brief Prepare the sand rectangle shapes, their locations and colors.
 * @param grid : The main grid.
 * @param cells : The vector holding each cell in the grid.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 * @param cellWidth : The width of each cell in the grid.
 */
void prepareVertices(const std::vector<std::vector<Particle>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth);
/**
 * @brief Function that handles spawning sand on mouse click.
 * @param grid : The main grid.
 * @param cellWidth : The width of each cell in the grid.
 * @param window : The sf::RenderWindow being used.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 */
void mousePressed(std::vector<std::vector<Particle>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols, ParticleType brushType, int brushSize);
/**
 * @brief Get corresponding color from particle enum
 * @param type The type of particle
 * @return The corresponding sf::Color to the particle
 */
sf::Color getColorForType(ParticleType type);
/**
 * @brief Update the brush text UI
 * @param text The UI text
 * @param brushType The current brush type
 * @param brushSize The current brush size
 */
void updateUI(sf::Text& text, ParticleType brushType, int brushSize);

// --- Variables ---
unsigned int res_x = 1280.0f;
unsigned int res_y = 720.0f;
float cellWidth = 5.0f;
int cols = static_cast<int>(res_x / cellWidth);
int rows = static_cast<int>(res_y / cellWidth);

// - Grids -
// A grid spans the entire screen and holds the coords for the sand, a vector of vectors holding particles
std::vector<std::vector<Particle>> grid;
std::vector<std::vector<Particle>> nextGrid;
int main()
{
    // Init window
    sf::RenderWindow window(sf::VideoMode({ res_x, res_y }), "Falling Sand :");
    window.setFramerateLimit(60);

    // Init Grids
    grid.assign(rows, std::vector<Particle>(cols));     // Grid of default EMPTY Particles
    nextGrid.assign(rows, std::vector<Particle>(cols)); // Second buffer grid

    // Vertex array for grid
    sf::VertexArray gridVertices(sf::PrimitiveType::Triangles);

    // Initial Brush
    int brushSize = 5;
    ParticleType brushType = ParticleType::SAND;

    // Brush UI Text
    sf::Font font("PixelDigivolveItalic-dV8R.ttf");
    sf::Text brushUI(font);
    brushUI.setString("BRUSH SETTINGS: \nParticle Type: " + std::to_string(static_cast<int>(brushType)) + "\nBrush Size: " + std::to_string(brushSize));
    brushUI.setCharacterSize(24);
    brushUI.setFillColor(sf::Color(100,100,100));

    // Time
    sf::Clock clock;
    float lastTime = 0;

    while (window.isOpen())
    {
        // Calculate FPS
        float currentTime = clock.getElapsedTime().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
        window.setTitle("Falling Sand : " + std::to_string(static_cast<int>(fps)));

        // --- Poll Events ---
        while (const std::optional event = window.pollEvent())
        {
            // Window X button
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Window/UI
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape) { window.close(); }

                // -- Brush Settings --
                if (keyPressed->scancode == sf::Keyboard::Scan::Hyphen) { // Increase brush size
                    if (brushSize > 1) {
                        brushSize--;
                        std::cout << brushSize;
                    }
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Equal) { // Decrease brush dize
                    if (brushSize < 50) {
                        brushSize++;
                        std::cout << brushSize;
                    }
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Num1) { // Change to Sand
                    brushType = ParticleType::SAND;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Num2) { // Change to Dirt
                    brushType = ParticleType::DIRT;
                } 
                if (keyPressed->scancode == sf::Keyboard::Scan::Num0) { // Change to Empty (Erase)
                    brushType = ParticleType::EMPTY;
                }
            }

        }

        // Spawn sand on click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            mousePressed(grid, cellWidth, window, rows, cols, brushType, brushSize);
        }

        // --- UPDATE ---
        updateGrid(grid, nextGrid, rows, cols, brushType);
        updateUI(brushUI ,brushType, brushSize);
        prepareVertices(grid, gridVertices, rows, cols, cellWidth);


        // --- DRAW ---
        window.clear(sf::Color(255,255,255));

        window.draw(gridVertices);
        window.draw(brushUI);

        window.display();
    }
}

void updateGrid(std::vector<std::vector<Particle>>& grid, std::vector<std::vector<Particle>>& nextGrid, int rows, int cols, ParticleType brushType)
{
    // Get current state of grid to calculate from
    nextGrid = grid;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // --- PARTICLE TYPE LOGIC ---
            if (grid[row][col].type == ParticleType::SAND) { // -- Sand Logic --

                if (row + 1 < rows) { // If in bounds:
                    ParticleType belowType = grid[row + 1][col].type;

                    if (belowType == ParticleType::EMPTY) { // If empty below:
                        // -- Fall Down --
                        nextGrid[row + 1][col] = grid[row][col]; // Copy particle data to new spot
                        nextGrid[row][col] = Particle{};         // Reset old spot to default (EMPTY)
                        continue; // Sand moved
                    }
                    else { // If below filled:
                        // -- Fall Diagonally --
                        int direction = (rand() % 2 == 0) ? -1 : 1; // Get random direction to move in
                        // Check if valid cell
                        int check_col_left = col - direction;
                        int check_col_right = col + direction;
                        bool canCheckLeft = (check_col_left >= 0 && check_col_left < cols);
                        bool canCheckRight = (check_col_right >= 0 && check_col_right < cols);

                        ParticleType below_left_type = ParticleType::SAND; // Assume blocked
                        if (canCheckLeft) { below_left_type = grid[row + 1][check_col_left].type; } // Get actual state

                        ParticleType below_right_type = ParticleType::SAND; // Assume blocked
                        if (canCheckRight) { below_right_type = grid[row + 1][check_col_right].type; } // Get actual state

                        if (canCheckLeft && below_left_type == ParticleType::EMPTY) { // Fall diagonally left
                            nextGrid[row + 1][check_col_left] = grid[row][col];
                            nextGrid[row][col] = {};
                            continue;
                        }
                        else if (canCheckRight && below_right_type == ParticleType::EMPTY) { // Fall diagonally right
                            nextGrid[row + 1][check_col_right] = grid[row][col];
                            nextGrid[row][col] = {};
                            continue;
                        }
                    }
                }
            }
            else if (grid[row][col].type == ParticleType::DIRT) { // If dirt

                // - Grass Spread Logic -
                bool canGrowGrass = false;
                if (row > 0) { // Bounds check
                    ParticleType aboveType = grid[row - 1][col].type;

                    // If empty above, turn to grass after awhile
                    if (aboveType == ParticleType::EMPTY) {
                        if (rand() % 100 < 1) {
                            // TODO: Could maybe add delay here
                            canGrowGrass = true;
                        }
                    }
                }

                if (canGrowGrass) {
                    nextGrid[row][col].type = ParticleType::GRASS;
                }
            }
            else if (grid[row][col].type == ParticleType::GRASS) { // If grass
                if (row > 0) { // Bounds check
                    ParticleType aboveType = grid[row - 1][col].type;

                    // If empty above, turn to grass after awhile
                    if (aboveType != ParticleType::EMPTY) {
                        if (rand() % 100 < 1) {
                            // TODO: Could maybe add delay here
                            nextGrid[row][col].type = ParticleType::DIRT;
                        }
                    }
                    //TODO: if above empty grow vine
                }

            }
        }
    }
    grid = nextGrid; // Update main grid
}
void prepareVertices(const std::vector<std::vector<Particle>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth)
{
    vertices.clear(); // Clear prev frames shapes
    // Set primitive type just in case (good practice)
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {

            // Prepare non empty cells
            if (grid[row][col].type != ParticleType::EMPTY) {
                // Get particle color
                sf::Color particleColor = getColorForType(grid[row][col].type);
                // Calculate corner coordinates
                float left = static_cast<float>(col) * cellWidth;
                float top = static_cast<float>(row) * cellWidth;
                float right = left + cellWidth;
                float bottom = top + cellWidth;
                // Define corner vecs
                sf::Vector2f topLeft(left, top);
                sf::Vector2f topRight(right, top);
                sf::Vector2f bottomLeft(left, bottom);
                sf::Vector2f bottomRight(right, bottom);

                // Create vertices for 2 tris
                // - Tri 1 -
                vertices.append(sf::Vertex(topLeft, particleColor));
                vertices.append(sf::Vertex(topRight, particleColor));
                vertices.append(sf::Vertex(bottomRight, particleColor));
                // - Tri 2 -
                vertices.append(sf::Vertex(topLeft, particleColor));
                vertices.append(sf::Vertex(bottomRight, particleColor));
                vertices.append(sf::Vertex(bottomLeft, particleColor));
            }
        }
    }
}
void mousePressed(std::vector<std::vector<Particle>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols, ParticleType brushType, int brushSize)
{
    // Get mouse pos
    int mouseCol = static_cast<int>(std::floor(sf::Mouse::getPosition(window).x / cellWidth));
    int mouseRow = static_cast<int>(std::floor(sf::Mouse::getPosition(window).y / cellWidth));

    int extent = std::floor(brushSize /2); // Define radius of brush

    for (int i = -extent; i <= extent; i++) {           // |
        for (int j = -extent; j <= extent; j++) {       // | Iterate cells within extent of mousepos

            if (rand() % 100 < 54) { // Chance to not spawn (makes random brush pattern)
                // Define rows/cols brush is in
                int col = mouseCol + i;
                int row = mouseRow + j;
                if (row >= 0 && row < rows && col >= 0 && col < cols) { // if within bounds
                    grid[row][col].type = brushType; // Change Particle
                }
            }
        }
    }
}

sf::Color getColorForType(ParticleType type)
{
    switch (type) {
    case ParticleType::SAND:  return sf::Color(194, 178, 128);
    case ParticleType::DIRT:  return sf::Color(133, 94, 66);
    case ParticleType::GRASS: return sf::Color(40, 140, 40);

    case ParticleType::EMPTY: return sf::Color::White;
    default:                  return sf::Color::Black; // Unknown particles are black
    }
}

void updateUI(sf::Text& text, ParticleType brushType, int brushSize)
{
    // Strings from type
    std::string brushTypeName;
    switch (brushType) {
        case ParticleType::SAND:  brushTypeName = "Sand"; break;
        case ParticleType::DIRT:  brushTypeName = "Dirt"; break;
        case ParticleType::GRASS: brushTypeName = "Grass"; break;
        case ParticleType::EMPTY: brushTypeName = "Empty"; break;
        default:                  brushTypeName = "Unknown"; break;
    }

    // Update the UI text with the current brush settings
    text.setString(
        "BRUSH SETTINGS: \n"
        "Particle Type: " + brushTypeName + "\n"
        "Brush Size: " + std::to_string(brushSize)
    );
}
