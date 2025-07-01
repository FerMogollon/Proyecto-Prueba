#define WIN32_LEAN_AND_MEAN

#include <iostream>

// Platform-specific include for Windows API functions.
#ifdef _WIN32
#include <windows.h>
#endif

// Include the main game flow header file.
#include "../src/InGameFlow.h"

// ===================================================================
// MAIN ENTRY POINT OF THE GAME
// This file serves as the main entry point for the game.
// ===================================================================
int main()
{
    // Phase 1: Initialize system settings like the console.
    initializeSystem();

    // Phase 2: Set up the game. This function handles the entire
    // welcome screen, and the "Load Game" or "New Game" logic.
    // It returns a fully prepared GameState object.
    GameState gameState = setupGame();

    // Phase 3: Main Game Loop. This loop continues as long as
    // the game is not over.
    while (!gameState.isGameOver)
    {
        // 3a: Render the entire user interface for the current turn.
        drawUI(gameState);

        // 3b: Handle the player's pre-turn menu choices (e.g., view properties).
        gameState = handleTurnMenu(gameState);

        // If the menu choice was "Save and Exit" or "Exit", the loop will break.
        if (gameState.isGameOver)
        {
            break;
        }

        // 3c: Execute the main action of the turn (roll, move, land on a tile).
        gameState = performTurnAction(gameState);

        // 3d: Check for game-over conditions like bankruptcy.
        gameState = checkGameOver(gameState);

        // 3e: If the game is still running, pause and switch to the next player.
        if (!gameState.isGameOver)
        {
            // Pause the screen so the user can read the turn's messages.
            pauseForNextTurn();
            // Switch to the next player for the next iteration of the loop.
            gameState.currentPlayerIndex = (gameState.currentPlayerIndex + 1) % NUM_PLAYERS;
        }
    }

    // Phase 4: The game loop has ended, so display the winner.
    showWinner(gameState);

    // End the program successfully.
    return 0;
}

//g++ -finput-charset=UTF-8 -fexec-charset=UTF-8 MAIN/main.cpp src/Player.cpp src/Board.cpp src/GameIO.cpp src/GameActions.cpp src/Console.cpp src/Validation.cpp src/InGameFlow.cpp -o minimonopoly -Isrc