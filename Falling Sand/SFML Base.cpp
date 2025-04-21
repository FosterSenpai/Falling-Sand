/*
TODO:
Create sand struct with own color variable to be drawn with

implement change color on scroll

make ui for changing sand type, make other materials
    make interactions for different materials
*/
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

// --- Functions ---
/**
 * @brief Updates the sands positions on the grids, handles gravity etc.
 * @param grid : The main grid.
 * @param nextGrid : The calculation grid.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 */
void updateGrid(std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& nextGrid, int rows, int cols);
/**
 * @brief Prepare the sand rectangle shapes, their locations and colors.
 * @param grid : The main grid.
 * @param cells : The vector holding each cell in the grid.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 * @param cellWidth : The width of each cell in the grid.
 */
void prepareVertices(const std::vector<std::vector<int>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth, sf::Color color);
/**
 * @brief Function that handles spawning sand on mouse click.
 * @param grid : The main grid.
 * @param cellWidth : The width of each cell in the grid.
 * @param window : The sf::RenderWindow being used.
 * @param rows : The number of rows.
 * @param cols : The number of cols.
 */
void mousePressed(std::vector<std::vector<int>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols);

// --- Variables ---
unsigned int res_x = 1280.0f;
unsigned int res_y = 720.0f;
float cellWidth = 1.0f;
int cols = static_cast<int>(res_x / cellWidth);
int rows = static_cast<int>(res_y / cellWidth);

// - Grids -
// A grid spans the entire screen and holds the coords for the sand, a vector of vectors
// each element either 1 (sand) or 0 (empty)
std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));     // Main grid
std::vector<std::vector<int>> nextGrid(rows, std::vector<int>(cols, 0)); // Grid for calculating

sf::Color sandColor = sf::Color(194, 178, 128);
int main()
{
    sf::RenderWindow window(sf::VideoMode({ res_x, res_y }), "Falling Sand :");
    window.setVerticalSyncEnabled(true);

    sf::VertexArray gridVertices(sf::PrimitiveType::Triangles);

    sf::Clock clock;
    float lastTime = 0;

    while (window.isOpen())
    {
        // Calculate FPS
        float currentTime = clock.getElapsedTime().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
        window.setTitle("Falling Sand : " + std::to_string(static_cast<int>(fps))); // Display in title

        // --- Poll Events ---
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // If Lmb held down spawn sand
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            mousePressed(grid, cellWidth, window, rows, cols);
        }



        // --- UPDATE ---
        updateGrid(grid, nextGrid, rows, cols);
        prepareVertices(grid, gridVertices, rows, cols, cellWidth, sandColor);


        // --- DRAW ---
        window.clear(sf::Color(255,255,255));

        window.draw(gridVertices);

        window.display();
    }
}

void updateGrid(std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& nextGrid, int rows, int cols)
{
    // Starting state of grid to calc from
    nextGrid = grid;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {

            if (grid[row][col] == 1) { // If sand

                // Check bounds BELOW the current cell
                if (row + 1 < rows) {
                    // -- Fall Down --
                    // Check cell state BELOW the current cell
                    int below = grid[row + 1][col];
                    if (below == 0) { // If empty below
                        // Move sand DOWN on nextGrid
                        nextGrid[row][col] = 0;       // Current cell becomes empty
                        nextGrid[row + 1][col] = 1;   // Cell below becomes sand
                    }
                    else {
                        // -- Fall Diagonally --
                        // Randomise fall direction
                        int direction = 1; // 1 for right preference, -1 for left
                        if (rand() % 2 == 0) { // Simpler 50/50 check
                            direction = -1;
                        }

                        // Check potential L/R positions *before* accessing grid
                        int check_col_left = col - direction;
                        int check_col_right = col + direction;
                        // Check bounds
                        bool canCheckLeft = (check_col_left >= 0 && check_col_left < cols);
                        bool canCheckRight = (check_col_right >= 0 && check_col_right < cols);

                        int below_left = 1; // Default to blocked
                        if (canCheckLeft) {
                            below_left = grid[row + 1][check_col_left];
                        }
                        int below_right = 1; // Default to blocked
                        if (canCheckRight) {
                            below_right = grid[row + 1][check_col_right];
                        }

                        // Move diagonally
                        if (canCheckLeft && below_left == 0) {
                            nextGrid[row][col] = 0;
                            nextGrid[row + 1][check_col_left] = 1;
                            continue; // Sand moved
                        }
                        else if (canCheckRight && below_right == 0) {
                            nextGrid[row][col] = 0;
                            nextGrid[row + 1][check_col_right] = 1;
                            continue; // Sand moved
                        }
                    }
                }
            }
        }
    }
    // Final state of grid
    grid = nextGrid;
}
void prepareVertices(const std::vector<std::vector<int>>& grid, sf::VertexArray& vertices, int rows, int cols, float cellWidth, sf::Color color)
{
    vertices.clear(); // Clear prev frames shapes
    // Set primitive type just in case (good practice)
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {

            // Only draw sand
            if (grid[row][col] == 1) {
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
                vertices.append(sf::Vertex(topLeft, color));
                vertices.append(sf::Vertex(topRight, color));
                vertices.append(sf::Vertex(bottomRight, color));
                // - Tri 2 -
                vertices.append(sf::Vertex(topLeft, color));
                vertices.append(sf::Vertex(bottomRight, color));
                vertices.append(sf::Vertex(bottomLeft, color));
            }
            //else {
            //    // Calculate corner coordinates
            //    float left = static_cast<float>(col) * cellWidth;
            //    float top = static_cast<float>(row) * cellWidth;
            //    float right = left + cellWidth;
            //    float bottom = top + cellWidth;
            //    // Define corner vecs
            //    sf::Vector2f topLeft(left, top);
            //    sf::Vector2f topRight(right, top);
            //    sf::Vector2f bottomLeft(left, bottom);
            //    sf::Vector2f bottomRight(right, bottom);

            //    // Create vertices for 2 tris
            //    // - Tri 1 -
            //    vertices.append(sf::Vertex(topLeft, sf::Color(255,255,255)));
            //    vertices.append(sf::Vertex(topRight, sf::Color(255, 255, 255)));
            //    vertices.append(sf::Vertex(bottomRight, sf::Color(255, 255, 255)));
            //    // - Tri 2 -
            //    vertices.append(sf::Vertex(topLeft, sf::Color(255, 255, 255)));
            //    vertices.append(sf::Vertex(bottomRight, sf::Color(255, 255, 255)));
            //    vertices.append(sf::Vertex(bottomLeft, sf::Color(255, 255, 255)));
            //}
        }
    }
}
void mousePressed(std::vector<std::vector<int>>& grid, float cellWidth, sf::RenderWindow& window, int rows, int cols)
{
    int mouseCol = static_cast<int>(std::floor(sf::Mouse::getPosition(window).x / cellWidth));
    int mouseRow = static_cast<int>(std::floor(sf::Mouse::getPosition(window).y / cellWidth));

    int brushRadius = 5;
    int extent = std::floor(brushRadius /2);

    for (int i = -extent; i <= extent; i++) {
        for (int j = -extent; j <= extent; j++) {
            if (rand() % 100 < 54) {
                int col = mouseCol + i;
                int row = mouseRow + j;
                if (row >= 0 && row < rows && col >= 0 && col < cols) { // if within bounds
                    if (grid[row][col] == 0) {
                        grid[row][col] = 1;
                    }
                }
            }
        }
    }
}
