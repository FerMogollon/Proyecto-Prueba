#include "Board.h"
#include "Constants.h"
#include "Console.h"
#include <iostream>

using namespace std;

// ===================================================================
// IMPLEMENTATION OF THE BOARD FUNCTIONS
// This file is responsible for all logic related to the visual
// representation of the game board and player movement calculations.
// ===================================================================


// --- Helper Structure: Coordinates ---
// This struct is used to package a row and a column value together.
// Its main purpose is to allow the getCoords function to return two integer
// values without using pointers or pass-by-reference, adhering to the project's rules.
struct Coordinates
{
    int row;
    int col;
};


// --- Helper Function: getCoords ---
// Translates a player's linear board position (an index from 0-19)
// into 2D grid coordinates (row, col) for drawing on the 6x6 board.
Coordinates getCoords(int pos)
{
    Coordinates coords = {0, 0}; // Initialize to a default value to prevent garbage data.

    // Top row (positions 0 to 5) -> maps to grid row 0, columns 0 to 5.
    if (pos >= 0 && pos <= 5)
    {
        coords.row = 0;
        coords.col = pos;
    }
    // Right column (positions 6 to 9) -> maps to grid rows 1 to 4, column 5.
    else if (pos >= 6 && pos <= 9)
    {
        coords.row = pos - 5;
        coords.col = 5;
    }
    // Bottom row (positions 10 to 15) -> maps to grid row 5, columns 5 down to 0.
    else if (pos >= 10 && pos <= 15)
    {
        coords.row = 5;
        coords.col = 15 - pos;
    }
    // Left column (positions 16 to 19) -> maps to grid rows 4 down to 1, column 0.
    else if (pos >= 16 && pos <= 19)
    {
        // This is the corrected logic for the left column.
        coords.row = 4 - (pos - 16);
        coords.col = 0;
    }
    return coords; // Returns the complete struct by value.
}


// --- Main Board Drawing Function ---
// This function renders the entire game board to the console.
// It uses the Console module to print each element at precise (x,y) coordinates,
// which guarantees perfect alignment regardless of emoji/symbol width.
void printBoard(Player p1, Player p2)
{
    // Define the starting top-left corner and the width for each cell on the console.
    const int startX = 2;
    const int startY = 2;
    const int cellWidth = 8; // Visual width for each cell, e.g., "[ 🏠 ] "

    // 1. Draw all 20 board tiles.
    for (int i = 0; i < BOARD_PERIMETER; ++i)
    {
        // Convert the linear position 'i' to 2D grid coordinates.
        Coordinates coords = getCoords(i);
        // Calculate the final screen coordinates for printing.
        int x = startX + coords.col * cellWidth;
        int y = startY + coords.row * 2; // Multiply row by 2 to create vertical space.

        // Create the string for the cell using its symbol.
        string cell = "[ " + BOARD_SYMBOLS[i] + " ]";
        // Use printAt to draw the cell at the exact calculated position.
        printAt(x, y, cell);
    }

    // 2. Draw the player markers on top of the board tiles.
    // Player 1
    Coordinates p1_coords = getCoords(p1.position);
    printAt(startX + p1_coords.col * cellWidth + 5, startY + p1_coords.row * 2, "1");

    // Player 2
    Coordinates p2_coords = getCoords(p2.position);
    // This logic places the '2' marker next to the '1' if they are on the same tile.
    if (p1.position == p2.position)
    {
        printAt(startX + p2_coords.col * cellWidth + 6, startY + p2_coords.row * 2, "2");
    }
    else
    {
        printAt(startX + p2_coords.col * cellWidth + 5, startY + p2_coords.row * 2, "2");
    }
}


// --- Player Movement Logic Function ---
// This function calculates the player's new position after a dice roll.
// It only handles the logic; it does not print messages to the console.
Player movePlayer(Player player, int roll)
{
    int oldPosition = player.position;
    // The modulo operator (%) ensures the movement wraps around the board
    // (e.g., from position 19, a roll of 3 results in position (19+3)%20 = 2).
    player.position = (oldPosition + roll) % BOARD_PERIMETER;

    // If the new position index is smaller than the old one, the player has passed GO.
    if (player.position < oldPosition)
    {
        player.money += GO_BONUS;
    }
    return player; // Return the player struct with the updated position and money.
}