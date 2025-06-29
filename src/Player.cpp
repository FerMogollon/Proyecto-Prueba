#include "Player.h"
#include <iostream>

using namespace std;

// ===================================================================
// IMPLEMENTATION OF THE PLAYER FUNCTIONS
// This file contains the actual logic for the functions declared in Player.h.
// ===================================================================


// Implementation of the function to create a new player.
// This acts as a "factory" for creating Player objects with default values.
Player createNewPlayer(string name)
{
    Player newPlayer;
    newPlayer.name = name;
    newPlayer.money = STARTING_MONEY;
    newPlayer.position = 0;
    
    // Initialize the properties array, ensuring the player owns no properties at the start.
    // This loop sets every spot in the boolean array to 'false'.
    for (int i = 0; i < BOARD_PERIMETER; ++i)
    {
        newPlayer.properties[i] = false;
    }
    
    // Initialize other stats to their default starting values.
    newPlayer.getOutOfJailCards = 0;
    newPlayer.turnsInJail = 0;
    newPlayer.isBankrupt = false;
    
    return newPlayer; // Returns the newly created player struct.
}


// Implementation of the function to display the player's status.
// This is a utility function for showing key information to the user.
void printPlayerStatus(Player player)
{
    cout << "-> Name: " << player.name << endl;
    cout << "-> Money: $" << player.money << endl;
    cout << "-> Get Out of Jail cards: " << player.getOutOfJailCards << endl;
}


// Implementation of the function to count a player's properties.
// This is crucial for determining the winner at the end of the game.
int countPlayerProperties(Player player)
{
    int count = 0;
    
    // Loop through the entire properties array and add 1 for every 'true' value found.
    for (int i = 0; i < BOARD_PERIMETER; ++i)
    {
        if (player.properties[i])
        {
            count++;
        }
    }
    
    return count;
}