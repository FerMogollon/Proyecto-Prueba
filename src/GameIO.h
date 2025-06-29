#ifndef GAMEIO_H
#define GAMEIO_H

#include "Player.h"
#include "GameActions.h" // Included to know the GameState struct definition.

// ===================================================================
// HEADER FILE FOR GAME INPUT/OUTPUT FUNCTIONS (SAVE/LOAD)
// This file declares the public interface for all file operations.
// ===================================================================

// A helper struct to return the two player names from a saved game file.
struct SavedPlayerNames
{
    string p1_name;
    string p2_name;
};


// --- Function Prototypes ---

// Returns 'true' if the master save file ("savegame.txt") exists.
bool doesSaveExist();

// Reads and returns the player names from "savegame.txt".
SavedPlayerNames getSavedPlayerNames();

// Loads a single player's data from their corresponding .txt file.
Player loadPlayer(string name);

// Saves the complete state of the current game.
void saveGame(GameState gs);

// Deletes all files from the previous saved game.
void deleteOldSaveFiles();

#endif // GAMEIO_H