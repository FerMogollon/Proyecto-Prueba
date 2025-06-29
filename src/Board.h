#ifndef BOARD_H
#define BOARD_H

// Includes Player.h because the functions in this module
// need to know what a 'Player' is to work with them.
#include "Player.h"

// ===================================================================
// DECLARATIONS FOR BOARD FUNCTIONS
// This file declares the available functions for managing the board.
// ===================================================================

// --- Function Prototypes ---

// Converts a linear position (0-19) into 2D grid coordinates (row, col).
// Draws the complete game board on the console, including the 
// position of both players (p1 and p2).
void printBoard(Player p1, Player p2);

// Calculates a player's new position after a dice roll.
// It also handles the bonus for passing GO. Returns the updated player struct.
Player movePlayer(Player player, int roll);

#endif // BOARD_H