#ifndef IN_GAME_FLOW_H
#define IN_GAME_FLOW_H

#include "GameActions.h"
// Initializes all system-level settings.
void initializeSystem();

// Handles the entire startup sequence.
GameState setupGame();

// Renders the complete User Interface.
void drawUI(GameState gs);

// Handles the interactive menu for the turn.
GameState handleTurnMenu(GameState gs);

// Executes the core action of a turn.
GameState performTurnAction(GameState gs);

// Checks for game-over conditions.
GameState checkGameOver(GameState gs);

// Displays the final winner screen.
void showWinner(GameState gs);

// Pauses the screen and waits for the user to press Enter.
void pauseForNextTurn();

#endif // IN_GAME_FLOW_H