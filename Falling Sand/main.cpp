// ============================================================================
// Project:     Falling Sand Simulation
// File:        main.cpp
// Author:      Foster Rae
// Date Created:2025-04-25
// Last Update: 2025-04-26
// Version:     1.2
// Description: Main entry point for the Falling Sand Simulation application.
//              Creates the Game object and runs the main game loop,
//              handling top-level exceptions.
// ============================================================================

#include "Game.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Main entry point of the application.
 */
int main()
{
    try {
        // Create an instance of the Game class.
        Game fallingSandGame;

        // Start the main game loop.
        fallingSandGame.run();
    }
    // Catch standard library exceptions (e.g., std::runtime_error from resource loading)
    catch (const std::exception& e) {
        std::cerr << "[FATAL ERROR] Exception caught in main: " << e.what() << std::endl;
		std::cerr << "Press Enter to exit..." << std::endl; // Pause so user can read the error
		std::cin.get();                                     // Wait for user input to exit
		return 1; // 1 for standard exceptions
    }
    // Catch any unknown exceptions
    catch (...) {
        std::cerr << "[FATAL ERROR] An unknown exception occurred." << std::endl;
        std::cerr << "Press Enter to exit..." << std::endl; // Pause so user can read the error
        std::cin.get();                                     // Wait for user input to exit
		return 2; // 2 for unknown exceptions
    }

    // If the game ran and exited normally without exceptions
    return 0;     // Return 0 indicating successful execution
}