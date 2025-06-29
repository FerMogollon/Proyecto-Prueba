#include "GameIO.h"
#include <fstream>
#include <iostream>
#include <cstdio> // Required for the remove() function to delete files.

using namespace std;

// ===================================================================
// IMPLEMENTATION OF GAME INPUT/OUTPUT FUNCTIONS (SAVE/LOAD)
// ===================================================================

// Internal helper function to save a single player's data to their .txt file.
void savePlayer(Player player)
{
    ofstream file(player.name + ".txt");
    if (file.is_open())
    {
        file << player.name << endl;
        file << player.money << endl;
        file << player.position << endl;
        file << player.getOutOfJailCards << endl;
        // Saves the indices of the properties owned by the player.
        for (int i = 0; i < BOARD_PERIMETER; ++i)
        {
            if (player.properties[i])
            {
                file << i << " ";
            }
        }
        file << endl;
        file.close();
    }
}

// Checks if a master save file ("savegame.txt") exists.
bool doesSaveExist()
{
    ifstream file("savegame.txt");
    return file.is_open(); // Returns true if the file could be opened (i.e., it exists).
}

// Gets the player names from the master save file.
SavedPlayerNames getSavedPlayerNames()
{
    SavedPlayerNames names;
    ifstream file("savegame.txt");
    if (file.is_open())
    {
        getline(file, names.p1_name);
        getline(file, names.p2_name);
    }
    return names;
}

// Loads a player's data from their file. If the file doesn't exist, it creates a new player.
// NOTE: As requested, this function still contains the bug related to mixing '>>' and 'getline'
// which can cause data corruption upon loading. It will be fixed later.
Player loadPlayer(string name)
{
    ifstream file(name + ".txt");
    if (file.is_open())
    {
        Player loadedPlayer;
        file >> loadedPlayer.name;
        file >> loadedPlayer.money;
        file >> loadedPlayer.position;
        file >> loadedPlayer.getOutOfJailCards;

        // Clear properties before loading the saved ones.
        for (int i = 0; i < BOARD_PERIMETER; ++i)
            loadedPlayer.properties[i] = false;

        // Read the indices of the properties and assign them.
        int propIndex;
        while (file >> propIndex)
        {
            if (propIndex >= 0 && propIndex < BOARD_PERIMETER)
            {
                loadedPlayer.properties[propIndex] = true;
            }
        }
        // Initialize non-saved values to their defaults.
        loadedPlayer.turnsInJail = 0;
        loadedPlayer.isBankrupt = false;
        file.close();
        cout << "Data for " << loadedPlayer.name << " has been loaded." << endl;
        return loadedPlayer;
    }
    cout << "No data found for " << name << ". Creating new player..." << endl;
    return createNewPlayer(name);
}

// Saves the entire game state.
void saveGame(GameState gs)
{
    cout << "Saving game..." << endl;
    // 1. Save each player's data to their respective file.
    savePlayer(gs.players[0]);
    savePlayer(gs.players[1]);
    // 2. Create/overwrite the master save file with the session's player names.
    ofstream file("savegame.txt");
    if (file.is_open())
    {
        file << gs.players[0].name << endl;
        file << gs.players[1].name << endl;
        file.close();
    }
    cout << "Game saved successfully." << endl;
}

// Deletes the files from the previous saved game.
void deleteOldSaveFiles()
{
    if (doesSaveExist())
    {
        cout << "Deleting data from the previous game..." << endl;
        SavedPlayerNames names = getSavedPlayerNames();
        // Delete each player's file.
        remove((names.p1_name + ".txt").c_str());
        remove((names.p2_name + ".txt").c_str());
        // Delete the master control file.
        remove("savegame.txt");
    }
}