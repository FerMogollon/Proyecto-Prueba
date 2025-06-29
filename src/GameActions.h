#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "Player.h"
using namespace std;

// This struct holds the entire state of the game, making it easy
// to pass between functions.
struct GameState {
    Player players[NUM_PLAYERS];
    int currentPlayerIndex;
    bool isGameOver;
};

// --- Function Prototypes ---

// Initializes the random number generator.
void initializeRandom();

// Simulates a 6-sided dice roll.
int rollDice();

// Finds the owner of a property. Returns player index (0 or 1), or -1 if unowned.
int getPropertyOwnerIndex(GameState gs, int propertyPosition);

// Checks if a player owns the complete set for a given property.
bool ownsFullSet(Player owner, int propertyPosition);

// Handles the logic when a player lands on a property tile.
GameState handleProperty(GameState gs);

// Handles the logic for drawing a special card.
GameState handleSpecialCard(GameState gs);

// Handles the logic for paying a tax.
GameState handleTax(GameState gs);

// Handles the logic for the "Go to Jail" tile.
GameState handleGoToJail(GameState gs);

// Handles a player's turn while they are in jail.
GameState handleJailTurn(GameState gs);

#endif // GAME_ACTIONS_H