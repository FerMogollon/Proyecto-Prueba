#ifndef PLAYER_H
#define PLAYER_H

#include <string>       // Required to use the 'string' data type for the player's name.
#include "Constants.h" // Required to use constants like 'BOARD_PERIMETER'.

using namespace std;

// ===================================================================
// PLAYER STRUCTURE DEFINITION
// ===================================================================

// The 'Player' struct groups all relevant information for a single
// player into one custom data type.
struct Player
{
    string name;                        // The player's name.
    int money;                          // The amount of cash the player currently has.
    int position;                       // The player's current position on the board (an index from 0-19).
    
    // This boolean array acts as a checklist for property ownership.
    // If properties[i] is 'true', it means the player owns the property
    // located at board position 'i'.
    bool properties[BOARD_PERIMETER];   
    
    int getOutOfJailCards;              // A counter for "Get Out of Jail Free" cards.
    int turnsInJail;                    // A counter for turns remaining to be spent in jail.
    bool isBankrupt;                    // A flag that becomes 'true' if the player goes bankrupt.
};


// --- Function Prototypes (Declarations) ---
// The implementation (the actual code) for these functions is in Player.cpp.

// Creates a new instance of a Player with initial default values.
Player createNewPlayer(string name);

// Displays the current status of a player on the console.
void printPlayerStatus(Player player);

// Counts and returns the total number of properties a player owns.
int countPlayerProperties(Player player);

#endif // PLAYER_H