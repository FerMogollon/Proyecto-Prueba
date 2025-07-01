// This define MUST be at the top to prevent name conflicts on Windows.
#define WIN32_LEAN_AND_MEAN

#include "GameIO.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

// Internal helper function to save a single player's data.
void savePlayer(Player player)
{
    ofstream file(player.name + ".txt");
    if (file.is_open())
    {
        file << player.name << endl;
        file << player.money << endl;
        file << player.position << endl;
        file << player.getOutOfJailCards << endl;
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

// Checks if a master save file exists.
bool doesSaveExist()
{
    ifstream file("savegame.txt");
    return file.is_open();
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

// Loads a player's data from their file.
// This is the robust version that reads line-by-line to prevent data corruption.
Player loadPlayer(string name)
{
    ifstream file(name + ".txt");
    if (file.is_open())
    {
        Player loadedPlayer;
        string line;

        // Read line by line and convert to the correct data type.
        getline(file, loadedPlayer.name);

        getline(file, line);
        try
        {
            loadedPlayer.money = stoi(line);
        }
        catch (...)
        {
            loadedPlayer.money = 0;
        }

        getline(file, line);
        try
        {
            loadedPlayer.position = stoi(line);
        }
        catch (...)
        {
            loadedPlayer.position = 0;
        }

        getline(file, line);
        try
        {
            loadedPlayer.getOutOfJailCards = stoi(line);
        }
        catch (...)
        {
            loadedPlayer.getOutOfJailCards = 0;
        }

        // Initialize properties to false before loading.
        for (int i = 0; i < BOARD_PERIMETER; ++i)
        {
            loadedPlayer.properties[i] = false;
        }

        // Read the line containing property indices.
        if (getline(file, line))
        {
            stringstream ss(line);
            int propIndex;
            while (ss >> propIndex)
            {
                if (propIndex >= 0 && propIndex < BOARD_PERIMETER)
                {
                    loadedPlayer.properties[propIndex] = true;
                }
            }
        }

        // Initialize non-saved values to their defaults.
        loadedPlayer.turnsInJail = 0;
        loadedPlayer.isBankrupt = false;

        // Assign the correct icon based on the saved player order.
        if (name == getSavedPlayerNames().p1_name)
        {
            loadedPlayer.icon = P1_ICON;
        }
        else
        {
            loadedPlayer.icon = P2_ICON;
        }

        file.close();
        cout << "Data for " << loadedPlayer.name << " loaded successfully." << endl;

        return loadedPlayer;
    }

    // This part only runs if the file could not be opened.
    cout << "No data found for " << name << ". Creating new player." << endl;
    return createNewPlayer(name);
}

// Saves the entire game state.
void saveGame(GameState gs)
{
    cout << "Saving game..." << endl;
    savePlayer(gs.players[0]);
    savePlayer(gs.players[1]);
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
        remove((names.p1_name + ".txt").c_str());
        remove((names.p2_name + ".txt").c_str());
        remove("savegame.txt");
    }
}