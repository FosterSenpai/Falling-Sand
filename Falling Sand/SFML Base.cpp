///*
//TODO:
//==========================================================================================
//make ui for changing sand type, make other materials (clickable buttons or something)
//    make interactions for different materials
//==========================================================================================
//Particle spawners? 
//==========================================================================================
//if grass been around long enough and above still empty grow flower/vine
//==========================================================================================
//Add menu on 'esc' with instructions/settings
//==========================================================================================
//Break up to follow OOP
//
//    world class
//        init grids
//        handles grid update
//        set + get particle types of cells
//
//    Game class
//        Setup window + variables
//        setup clock + variables
//        hold cellWidth
//        setup brush UI
//        setup brush settings
//        handle event processing
//            key press
//            realtime input (mouse click)
//        handle placing particles function to be called on click
//        handle update of brushUI
//        Prepare vetices (same as before)
//
//        render func
//            // Prepare vertices based on the current world state
//            prepareVerticesInternal(); // Helper to keep render clean
//
//            window.clear(sf::Color::White);
//            window.draw(gridVertices);
//            window.draw(uiText);
//            window.display();
//
//        run func that is main loop
//            void run() {
//             srand(static_cast<unsigned int>(time(0))); // Seed random
//             float lastTimeForFPS = 0.f; // For FPS calculation only
//
//            while (window.isOpen()) {
//                 // --- FPS Calculation --- (Can be simplified if not needed)
//                float currentTime = clock.getElapsedTime().asSeconds();
//                float deltaTime = currentTime - lastTimeForFPS;
//                lastTimeForFPS = currentTime;
//                float fps = (deltaTime > 0) ? (1.f / deltaTime) : 0.f;
//                // -----------------------
//
//                processEvents();         // Handle window events, key presses
//                handleRealtimeInput();   // Handle mouse hold
//                update();                // Update simulation and UI text
//                render();                // Draw everything
//            }
//
//    Utils class
//        get color for particle helper func
//==========================================================================================
//*/
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <iostream>
//#include <string>
//#include "Particle.h"
//#include "Utils.h"
//
//// --- Functions ---
///**
// * @brief Updates the sands positions on the grids, handles gravity etc.
// * @param grid : The main grid.
// * @param nextGrid : The calculation grid.
// * @param rows : The number of rows.
// * @param cols : The number of cols.
// */
//void updateGrid(std::vector<std::vector<Particle>>& grid, std::vector<std::vector<Particle>>& nextGrid, int rows, int cols, ParticleType brushType);
///**
// * @brief Prepare the sand rectangle shapes, their locations and colors.
// * @param grid : The main grid.
// * @param cells : The vector holding each cell in the grid.
// * @param rows : The number of rows.
// * @param cols : The number of cols.
// * @param cellWidth : The width of each cell in the grid.
// */
//void prepareVertices(const std::vector<std::vector<Particle>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth);
///**
// * @brief Function that handles spawning sand on mouse click.
// * @param grid : The main grid.
// * @param cellWidth : The width of each cell in the grid.
// * @param window : The sf::RenderWindow being used.
// * @param rows : The number of rows.
// * @param cols : The number of cols.
// */
//void mousePressed(std::vector<std::vector<Particle>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols, ParticleType brushType, int brushSize);
///**
// * @brief Update the brush text UI
// * @param text The UI text
// * @param brushType The current brush type
// * @param brushSize The current brush size
// */
//void updateUI(sf::Text& text, ParticleType brushType, int brushSize);
//
//// --- Variables ---
//unsigned int res_x = 1280.0f;
//unsigned int res_y = 720.0f;
//float cellWidth = 5.0f;
//int cols = static_cast<int>(res_x / cellWidth);
//int rows = static_cast<int>(res_y / cellWidth);
//
//// - Grids -
//// A grid spans the entire screen and holds the coords for the sand, a vector of vectors holding particles
//std::vector<std::vector<Particle>> grid;
//std::vector<std::vector<Particle>> nextGrid;
//int main()
//{
//    // Init window
//    sf::RenderWindow window(sf::VideoMode({ res_x, res_y }), "Falling Sand :");
//    window.setFramerateLimit(60);
//
//    // Init Grids
//    grid.assign(rows, std::vector<Particle>(cols));     // Grid of default EMPTY Particles
//    nextGrid.assign(rows, std::vector<Particle>(cols)); // Second buffer grid
//
//    // Vertex array for grid
//    sf::VertexArray gridVertices(sf::PrimitiveType::Triangles);
//
//    // Initial Brush
//    int brushSize = 5;
//    ParticleType brushType = ParticleType::SAND;
//
//    // Brush UI Text
//    sf::Font font("PixelDigivolveItalic-dV8R.ttf");
//    sf::Text brushUI(font);
//    brushUI.setString("BRUSH SETTINGS: \nParticle Type: " + std::to_string(static_cast<int>(brushType)) + "\nBrush Size: " + std::to_string(brushSize));
//    brushUI.setCharacterSize(24);
//    brushUI.setFillColor(sf::Color(100,100,100));
//
//    // Time
//    sf::Clock clock;
//    float lastTime = 0;
//
//    while (window.isOpen())
//    {
//        // Calculate FPS
//        float currentTime = clock.getElapsedTime().asSeconds();
//        float fps = 1.f / (currentTime - lastTime);
//        lastTime = currentTime;
//        window.setTitle("Falling Sand : " + std::to_string(static_cast<int>(fps)));
//
//        // --- Poll Events ---
//        while (const std::optional event = window.pollEvent())
//        {
//            // Window X button
//            if (event->is<sf::Event::Closed>())
//                window.close();
//
//            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//                // Window/UI
//                if (keyPressed->scancode == sf::Keyboard::Scan::Escape) { window.close(); }
//
//                // -- Brush Settings --
//                if (keyPressed->scancode == sf::Keyboard::Scan::Hyphen) { // Increase brush size
//                    if (brushSize > 1) {
//                        brushSize--;
//                        std::cout << brushSize;
//                    }
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Equal) { // Decrease brush dize
//                    if (brushSize < 50) {
//                        brushSize++;
//                        std::cout << brushSize;
//                    }
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num0) { // Change to Empty (Erase)
//                    brushType = ParticleType::EMPTY;
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num1) { // Change to Sand
//                    brushType = ParticleType::SAND;
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num2) { // Change to Dirt
//                    brushType = ParticleType::DIRT;
//                } 
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num3) { // Change to Water
//                    brushType = ParticleType::WATER;
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num4) { // Change to Silt
//                    brushType = ParticleType::SILT;
//                }
//                if (keyPressed->scancode == sf::Keyboard::Scan::Num5) { // Change to Oil
//                    brushType = ParticleType::OIL;
//                }
//            }
//
//        }
//
//        // Spawn sand on click
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
//        {
//            mousePressed(grid, cellWidth, window, rows, cols, brushType, brushSize);
//        }
//
//        // --- UPDATE ---
//        updateGrid(grid, nextGrid, rows, cols, brushType);
//        updateUI(brushUI ,brushType, brushSize);
//        prepareVertices(grid, gridVertices, rows, cols, cellWidth);
//
//
//        // --- DRAW ---
//        window.clear(sf::Color(255,255,255));
//
//        window.draw(gridVertices);
//        window.draw(brushUI);
//
//        window.display();
//    }
//}
//
//void updateGrid(std::vector<std::vector<Particle>>& grid, std::vector<std::vector<Particle>>& nextGrid, int rows, int cols, ParticleType brushType)
//{
//    // TODO: Reading grid left to right every time makes right movement bias
//    // Swapping sweep dir should help
//
//    // Get current state of grid to calculate from
//    nextGrid = grid;
//
//    for (int row = 0; row < rows; ++row) {
//        for (int col = 0; col < cols; ++col) {
//            // --- PARTICLE TYPE LOGIC ---
//            if (grid[row][col].type == ParticleType::SAND) { // -- Sand Logic --
//
//                if (row + 1 < rows) { // If in bounds:
//                    ParticleType belowType = grid[row + 1][col].type;
//
//                    if (belowType == ParticleType::EMPTY && nextGrid[row + 1][col].type == ParticleType::EMPTY) { // If empty below:
//                        // -- Fall Down --
//                        nextGrid[row + 1][col] = grid[row][col]; // Copy data from current spot, into where its moving
//                        nextGrid[row][col] = Particle{};         // Reset old spot
//                        continue;                                // Sand moved
//                    }
//                    else if (belowType == ParticleType::WATER) { // If water below
//                        // Displace water and fall through
//                        // TODO: Could make this and other logics functions
//                        // TODO: Stop water from displacing upward through particles falling from brush
//                        // TODO: diagonal sand fall through water
//                        Particle oldWater = grid[row + 1][col]; // Save water particle incase it hold important data
//                        nextGrid[row + 1][col] = grid[row][col];    // copy sand where water was
//                        nextGrid[row][col] = oldWater;              // Put water where sand was
//                        continue;
//                    }
//                    else { // If below filled:
//
//                        // -- Fall Diagonally --
//                        int direction = (rand() % 2 == 0) ? -1 : 1; // Get random direction to move in
//                        // Check if valid cell
//                        int check_col_left = col - direction;
//                        int check_col_right = col + direction;
//                        bool canCheckLeft = (check_col_left >= 0 && check_col_left < cols);
//                        bool canCheckRight = (check_col_right >= 0 && check_col_right < cols);
//
//                        // --- Diagonal Checks (Handles EMPTY and WATER) ---
//                        ParticleType target_left_type = ParticleType::SAND; // Assume blocked initially
//                        if (canCheckLeft) { target_left_type = grid[row + 1][check_col_left].type; }
//
//                        ParticleType target_right_type = ParticleType::SAND; // Assume blocked initially
//                        if (canCheckRight) { target_right_type = grid[row + 1][check_col_right].type; }
//
//                        bool movedDiagonally = false;
//
//                        // Check preferred direction first (left or right based on 'direction')
//                        int check_col_1 = col + direction;
//                        // Get type of target cell
//                        ParticleType target_type_1 = (direction == -1) ? target_left_type : target_right_type;
//                        bool canCheck1 = (direction == -1) ? canCheckLeft : canCheckRight;
//
//                        if (canCheck1 && (target_type_1 == ParticleType::EMPTY || target_type_1 == ParticleType::WATER)) {
//                            // Check nextGrid to prevent conflicts if possible
//                            if (nextGrid[row + 1][check_col_1].type == ParticleType::EMPTY || nextGrid[row + 1][check_col_1].type == target_type_1) {
//                                if (target_type_1 == ParticleType::EMPTY) { // If target cell empty
//                                    nextGrid[row + 1][check_col_1] = grid[row][col]; // Move sand to empty spot
//                                    nextGrid[row][col] = {};                         // Clear old spot
//                                }
//                                else { // If target cell water
//                                    nextGrid[row + 1][check_col_1] = grid[row][col]; // Move sand to water spot
//                                    nextGrid[row][col] = grid[row + 1][check_col_1]; // Move water to sand spot
//                                }
//                                movedDiagonally = true;
//                            }
//                        }
//
//                        // If didn't move in preferred direction, check the other direction
//                        if (!movedDiagonally) {
//                            int check_col_2 = col - direction;
//                            ParticleType target_type_2 = (direction == -1) ? target_right_type : target_left_type;
//                            bool canCheck2 = (direction == -1) ? canCheckRight : canCheckLeft;
//
//                            if (canCheck2 && (target_type_2 == ParticleType::EMPTY || target_type_2 == ParticleType::WATER)) {
//                                // Check nextGrid to prevent conflicts if possible (optional refinement)
//                                if (nextGrid[row + 1][check_col_2].type == ParticleType::EMPTY || nextGrid[row + 1][check_col_2].type == target_type_2) {
//                                    if (target_type_2 == ParticleType::EMPTY) {
//                                        nextGrid[row + 1][check_col_2] = grid[row][col];
//                                        nextGrid[row][col] = {};                        
//                                    }
//                                    else {
//                                        nextGrid[row + 1][check_col_2] = grid[row][col];
//                                        nextGrid[row][col] = grid[row + 1][check_col_2];
//                                    }
//                                    movedDiagonally = true;
//                                }
//                            }
//                        }
//
//                        if (movedDiagonally) {
//                            continue; // Sand moved diagonally
//                        }
//
//                        // If water above sand turn into wet sand
//                        if (row > 0) { // Bounds check
//                            ParticleType aboveType = grid[row - 1][col].type;
//
//                            if (aboveType == ParticleType::WATER) {
//                                if (rand() % 100 < 10) {
//                                    nextGrid[row][col].type = ParticleType::SANDWET;
//                                }
//                            }
//                        }
//                        // TODO: add logic for wet sand, make movement different and turn back to dry sand after a bit
//
//                    }
//                }
//            }
//            else if (grid[row][col].type == ParticleType::DIRT) { // -- Dirt Logic --
//
//                // - Grass Spread Logic -
//                bool canGrowGrass = false;
//                if (row > 0) { // Bounds check
//                    ParticleType aboveType = grid[row - 1][col].type;
//
//                    // If empty above, turn to grass after awhile
//                    if (aboveType == ParticleType::EMPTY) {
//                        if (rand() % 100 < 1) {
//                            // TODO: Could maybe add delay here
//                            canGrowGrass = true;
//                        }
//                    }
//
//                    // TODO: make dirt blocks have small chance to fall from bottom of dirt structure for a few seconds then stay static
//                }
//
//                if (canGrowGrass) {
//                    nextGrid[row][col].type = ParticleType::GRASS;
//                }
//            }
//            else if (grid[row][col].type == ParticleType::GRASS) { // -- Grass Logic --
//                if (row > 0) { // Bounds check
//                    ParticleType aboveType = grid[row - 1][col].type;
//
//                    // If empty above, turn to grass after awhile
//                    if (aboveType != ParticleType::EMPTY) {
//                        if (rand() % 100 < 1) {
//                            // TODO: Could maybe add delay here
//                            nextGrid[row][col].type = ParticleType::DIRT;
//                        }
//                    }
//                    //TODO: if above empty grow vine
//                }
//
//            }
//            else if (grid[row][col].type == ParticleType::WATER) { // -- Water Logic --
//
//                // If this water particle's current position in the nextGrid has ALREADY
//                // been overwritten by something else (e.g., Oil/Sand/Silt swapping down),
//                // it means this water particle has been displaced. It should NOT try
//                // to execute its own movement logic this frame.
//                if (nextGrid[row][col].type != ParticleType::WATER) {
//                    continue;
//                }
//				// -- Check down --
//                if (row + 1 < rows && grid[row + 1][col].type == ParticleType::EMPTY && nextGrid[row + 1][col].type == ParticleType::EMPTY) {
//                    nextGrid[row + 1][col] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//                // -- Check diagonals down (Randomly) --
//                int direction = (rand() % 100 < 50) ? -1 : 1; // -1 = left, 1 = right
//                // Check first diagonal ( left or right random )
//                int diagCol1 = col + direction;
//                if (row + 1 < rows && diagCol1 >= 0 && diagCol1 < cols && grid[row + 1][diagCol1].type == ParticleType::EMPTY && nextGrid[row + 1][diagCol1].type == ParticleType::EMPTY) {
//                    nextGrid[row + 1][diagCol1] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//                // Check Other diagonal
//                int diagCol2 = col - direction; // The other direction
//                if (row + 1 < rows && diagCol2 >= 0 && diagCol2 < cols && grid[row + 1][diagCol2].type == ParticleType::EMPTY && nextGrid[row + 1][diagCol2].type == ParticleType::EMPTY) {
//                    nextGrid[row + 1][diagCol2] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//
//                // -- Check horizontal --
//                int h_direction = (rand() % 2 == 0) ? -1 : 1;
//                // Check first random horizontal direction
//                int sideCol1 = col + h_direction;
//                if (sideCol1 >= 0 && sideCol1 < cols && grid[row][sideCol1].type == ParticleType::EMPTY && nextGrid[row][sideCol1].type == ParticleType::EMPTY) {
//                    nextGrid[row][sideCol1] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//                // Check other horizontal directions
//                int sideCol2 = col - h_direction; // The other direction
//                if (sideCol2 >= 0 && sideCol2 < cols && grid[row][sideCol2].type == ParticleType::EMPTY && nextGrid[row][sideCol2].type == ParticleType::EMPTY) {
//                    nextGrid[row][sideCol2] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//
//                // --- Check WIDER horizontal (ONLY if immediate failed) ---
//                // Check two cells over in random direction
//                int sideCol1_far = col + 2 * h_direction;
//                if ((sideCol1 < 0 || sideCol1 >= cols || grid[row][sideCol1].type == ParticleType::WATER) && // Check if immediate is water or OOB
//                    (sideCol1_far >= 0 && sideCol1_far < cols && grid[row][sideCol1_far].type == ParticleType::EMPTY && nextGrid[row][sideCol1_far].type == ParticleType::EMPTY) // Check if far is clear
//                    )
//                {
//                    nextGrid[row][sideCol1_far] = grid[row][col]; // Move 2 steps
//                    nextGrid[row][col] = {};
//                    continue; // Moved 2 steps
//                }
//
//                // Check two cells over in other direction
//                int sideCol2_far = col - 2 * h_direction;
//                if ((sideCol2 < 0 || sideCol2 >= cols || grid[row][sideCol2].type == ParticleType::WATER) &&
//                    (sideCol2_far >= 0 && sideCol2_far < cols && grid[row][sideCol2_far].type == ParticleType::EMPTY && nextGrid[row][sideCol2_far].type == ParticleType::EMPTY) // Check if far is clear
//                    )
//                {
//                    nextGrid[row][sideCol2_far] = grid[row][col];
//                    nextGrid[row][col] = {};
//                    continue;
//                }
//            }
//            else if (grid[row][col].type == ParticleType::OIL) { // -- Oil Logic --
//                // Oil is denser than water and should fall through it.
//
//                bool moved = false; // Flag to track if the particle moved this step
//
//                // --- Priority 1: Move Down ---
//                if (row + 1 < rows) { // Check bounds below
//                    ParticleType belowType = grid[row + 1][col].type;
//
//                    if (belowType == ParticleType::EMPTY) {
//                        // Check if the target spot in nextGrid is also empty (standard collision check)
//                        if (nextGrid[row + 1][col].type == ParticleType::EMPTY) {
//                            nextGrid[row + 1][col] = grid[row][col]; // Move oil down
//                            nextGrid[row][col] = {};                // Clear old spot
//                            moved = true;
//                        }
//                    }
//                    else if (belowType == ParticleType::WATER) {
//                        // Attempt to SWAP with water below.
//                        // Perform the swap directly in nextGrid.
//                        // We assume the oil 'wins' the spot.
//                        nextGrid[row + 1][col] = grid[row][col];     // Oil moves down into water's spot
//                        nextGrid[row][col] = grid[row + 1][col];   // Water moves up into oil's spot
//                        moved = true;
//                    }
//                    // If below is neither EMPTY nor WATER (e.g., SAND, DIRT), oil stops vertically for now.
//                }
//
//                // --- Priority 2: Move Diagonally Down ---
//                if (!moved && row + 1 < rows) { // Only try diagonal if couldn't move straight down
//                    int direction = (rand() % 2 == 0) ? -1 : 1; // Random preference: -1=left, 1=right
//
//                    for (int i = 0; i < 2; ++i) { // Check both diagonals (preferred one first)
//                        int check_col = col + direction;
//                        if (check_col >= 0 && check_col < cols) { // Check horizontal bounds
//                            ParticleType targetType = grid[row + 1][check_col].type;
//
//                            if (targetType == ParticleType::EMPTY) {
//                                // Check collision in nextGrid for empty spot
//                                if (nextGrid[row + 1][check_col].type == ParticleType::EMPTY) {
//                                    nextGrid[row + 1][check_col] = grid[row][col]; // Move oil diagonally
//                                    nextGrid[row][col] = {};                      // Clear old spot
//                                    moved = true;
//                                    break; // Exit diagonal check loop once moved
//                                }
//                            }
//                            else if (targetType == ParticleType::WATER) {
//                                // Attempt SWAP with water diagonally
//                                nextGrid[row + 1][check_col] = grid[row][col];     // Oil moves diagonally
//                                nextGrid[row][col] = grid[row + 1][check_col];   // Water moves into oil's old spot
//                                moved = true;
//                                break; // Exit diagonal check loop once moved
//                            }
//                        }
//                        direction *= -1; // Flip direction to check the other side on the next iteration
//                    }
//                }
//
//                // --- Priority 3: Horizontal Movement (Optional) ---
//                bool supportedBelow = (row + 1 < rows && grid[row + 1][col].type != ParticleType::EMPTY);
//
//                if (supportedBelow && !moved) {
//                    // Check first random horizontal direction
//                    int direction = (rand() % 2 == 0) ? -1 : 1; // Random preference: -1=left, 1=right
//                    int sideCol1 = col + direction;
//                    if (sideCol1 >= 0 && sideCol1 < cols && grid[row][sideCol1].type == ParticleType::EMPTY && nextGrid[row][sideCol1].type == ParticleType::EMPTY) {
//                        nextGrid[row][sideCol1] = grid[row][col];
//                        nextGrid[row][col] = {};
//                        continue;
//                    }
//                    // Check other horizontal directions
//                    int sideCol2 = col - direction; // The other direction
//                    if (sideCol2 >= 0 && sideCol2 < cols && grid[row][sideCol2].type == ParticleType::EMPTY && nextGrid[row][sideCol2].type == ParticleType::EMPTY) {
//                        nextGrid[row][sideCol2] = grid[row][col];
//                        nextGrid[row][col] = {};
//                        continue;
//                    }
//                }
//
//                if (moved) {
//                    continue; // Skip to the next particle in the main loop if oil moved/swapped
//                }
//
//                } // End Oil Logic
//            else if (grid[row][col].type == ParticleType::SILT) { // -- Silt Logic --
//                if (row + 1 < rows) { // In bounds
//                    
//                    // Only fall downward, no sideways
//                    if (grid[row + 1][col].type == ParticleType::EMPTY && nextGrid[row + 1][col].type == ParticleType::EMPTY) {
//                        nextGrid[row + 1][col] = grid[row][col];
//                        nextGrid[row][col] = Particle{};
//                        continue;
//                    }
//                    else if (grid[row + 1][col].type == ParticleType::WATER) { // If water below
//                        // Displace water and fall through
//                        Particle oldWater = grid[row + 1][col]; // Save water particle incase it hold important data
//                        nextGrid[row + 1][col] = grid[row][col];    // copy sand where water was
//                        nextGrid[row][col] = oldWater;              // Put water where sand was
//                        continue;
//                    }
//                }
//            }
//        }
//    }
//    grid = nextGrid; // Update main grid
//}
//void prepareVertices(const std::vector<std::vector<Particle>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth)
//{
//    vertices.clear(); // Clear prev frames shapes
//    // Set primitive type just in case (good practice)
//    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
//
//    for (int row = 0; row < rows; ++row) {
//        for (int col = 0; col < cols; ++col) {
//
//            // Prepare non empty cells
//            if (grid[row][col].type != ParticleType::EMPTY) {
//                // Get particle color
//                sf::Color particleColor = Utils::getColorForType(grid[row][col].type);
//                // Calculate corner coordinates
//                float left = static_cast<float>(col) * cellWidth;
//                float top = static_cast<float>(row) * cellWidth;
//                float right = left + cellWidth;
//                float bottom = top + cellWidth;
//                // Define corner vecs
//                sf::Vector2f topLeft(left, top);
//                sf::Vector2f topRight(right, top);
//                sf::Vector2f bottomLeft(left, bottom);
//                sf::Vector2f bottomRight(right, bottom);
//
//                // Create vertices for 2 tris
//                // - Tri 1 -
//                vertices.append(sf::Vertex(topLeft, particleColor));
//                vertices.append(sf::Vertex(topRight, particleColor));
//                vertices.append(sf::Vertex(bottomRight, particleColor));
//                // - Tri 2 -
//                vertices.append(sf::Vertex(topLeft, particleColor));
//                vertices.append(sf::Vertex(bottomRight, particleColor));
//                vertices.append(sf::Vertex(bottomLeft, particleColor));
//            }
//        }
//    }
//}
//void mousePressed(std::vector<std::vector<Particle>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols, ParticleType brushType, int brushSize)
//{
//    // Get mouse pos
//    int mouseCol = static_cast<int>(std::floor(sf::Mouse::getPosition(window).x / cellWidth));
//    int mouseRow = static_cast<int>(std::floor(sf::Mouse::getPosition(window).y / cellWidth));
//
//    int extent = std::floor(brushSize /2); // Define radius of brush
//
//    for (int i = -extent; i <= extent; i++) {           // |
//        for (int j = -extent; j <= extent; j++) {       // | Iterate cells within extent of mousepos
//
//			// TODO: Change random chance (which is pretty much brush density) for each particle type
//            if (rand() % 100 < 20) { // Chance to not spawn (makes random brush pattern)
//                // Define rows/cols brush is in
//                int col = mouseCol + i;
//                int row = mouseRow + j;
//                if (row >= 0 && row < rows && col >= 0 && col < cols) { // if within bounds
//                    grid[row][col].type = brushType; // Change Particle
//                }
//            }
//        }
//    }
//}
//
//void updateUI(sf::Text& text, ParticleType brushType, int brushSize)
//{
//    // Strings from type
//    std::string brushTypeName;
//    switch (brushType) {
//        case ParticleType::SAND:  brushTypeName = "Sand"; break;
//        case ParticleType::SANDWET:  brushTypeName = "Wet Sand"; break;
//        case ParticleType::DIRT:  brushTypeName = "Dirt"; break;
//        case ParticleType::GRASS: brushTypeName = "Grass"; break;
//        case ParticleType::WATER: brushTypeName = "Water"; break;
//        case ParticleType::SILT:  brushTypeName = "Silt"; break;
//        case ParticleType::OIL:  brushTypeName = "Oil"; break;
//        case ParticleType::EMPTY: brushTypeName = "Empty"; break;
//        default:                  brushTypeName = "Unknown"; break;
//    }
//
//    // Update the UI text with the current brush settings
//    text.setString(
//        "BRUSH SETTINGS: \n"
//        "Particle Type: " + brushTypeName + "\n"
//        "Brush Size: " + std::to_string(brushSize)
//    );
//}