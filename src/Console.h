#ifndef CONSOLE_H
#define CONSOLE_H

#include <string> // Required because the printAt function uses the 'string' type.

// ===================================================================
// HEADER FILE FOR CONSOLE UTILITY FUNCTIONS
// This file declares the public interface for the Console module,
// making its functions available to the rest of the program.
// ===================================================================

using namespace std;

// --- Function Prototypes ---

// Clears the entire console screen and moves the cursor to the top-left.
void clearScreen();

// Moves the console cursor to a specific (x, y) coordinate.
void moveCursor(int x, int y);

// A convenience function that moves the cursor and then prints text.
// The string is passed by value to comply with the "no references" rule.
void printAt(int x, int y, string text);

#endif // CONSOLE_H