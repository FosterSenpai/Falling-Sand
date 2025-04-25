#include "Game.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try {
        // Create an instance of the Game class.
        Game fallingSandGame;

        // Start the main game loop.
        fallingSandGame.run();
    }

    // Catch standard exceptions
    catch (const std::exception& e) {
        std::cerr << "[FATAL ERROR] Exception caught in main: " << e.what() << std::endl;
        // Pause to allow user to see error
        std::cerr << "Press Enter to exit..." << std::endl;
        std::cin.get();
		return 1; // Error code 1 for standard exceptions
    }

    // Catch any other potential unknown exceptions
    catch (...) {
        std::cerr << "[FATAL ERROR] An unknown exception occurred." << std::endl;
		return 2; // Error code 2 for unknown exceptions
    }

    return 0;
}