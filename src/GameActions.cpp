#include "GameActions.h"
#include "Constants.h"
#include "Validation.h" // For getValidInput and getYesNoInput functions
#include <iostream>
#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time()
#include <cctype>  // Required for tolower()
#include <limits>  // Required for numeric_limits

using namespace std;

// ===================================================================
// IMPLEMENTATION OF THE CORE GAME ACTIONS
// This file contains the "rules engine" of the game.
// ===================================================================

// Initializes the random number generator using the current time as a seed.
// This ensures that dice rolls are different in every game session.
void initializeRandom()
{
    srand(time(nullptr));
}

// Returns a random number between 1 and 6, simulating a dice roll.
int rollDice()
{
    return (rand() % 6) + 1;
}

// A helper function to find the owner of a property.
// It loops through all players and checks their 'properties' array.
int getPropertyOwnerIndex(GameState gs, int propertyPosition)
{
    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        if (gs.players[i].properties[propertyPosition])
        {
            return i; // Returns player index 0 or 1
        }
    }
    return -1; // Returns -1 if no owner is found
}

// Checks if the player 'owner' owns all properties of the set
// to which 'propertyPosition' belongs, using the PROPERTY_SETS matrix.
bool ownsFullSet(Player owner, int propertyPosition)
{
    int setToCheck = -1;
    // First, find which set the current property belongs to.
    for (int i = 0; i < NUM_PROPERTY_SETS; ++i)
    {
        for (int j = 0; j < MAX_PROPS_PER_SET; ++j)
        {
            if (PROPERTY_SETS[i][j] == propertyPosition)
            {
                setToCheck = i;
                break;
            }
        }
        if (setToCheck != -1)
            break;
    }

    if (setToCheck == -1)
        return false; // Not part of any set.

    // Now, check if the owner has all properties in that set.
    for (int j = 0; j < MAX_PROPS_PER_SET; ++j)
    {
        int propInSet = PROPERTY_SETS[setToCheck][j];
        if (propInSet != -1)
        { // Ignore padding values (-1)
            if (!owner.properties[propInSet])
            {
                return false; // If even one property is not owned, return false.
            }
        }
    }
    return true; // If the loop completes, the player owns the full set.
}

// Manages the logic when a player lands on a property.
GameState handleProperty(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    int propertyPos = currentPlayer.position;
    int ownerIndex = getPropertyOwnerIndex(gs, propertyPos);

    if (ownerIndex == -1)
    {
        cout << "The property '" << PROPERTY_NAMES[propertyPos] << "' is available." << endl;
        cout << "It costs $" << PROPERTY_PRICES[propertyPos] << ". Your money: $" << currentPlayer.money << endl;
        if (currentPlayer.money >= PROPERTY_PRICES[propertyPos])
        {
            cout << "Do you want to buy it? (y/n): ";
            // --- CÓDIGO REEMPLAZADO ---
            // El antiguo bucle do-while se reemplaza con esta simple llamada.
            char choice = getYesNoInput();

            if (choice == 'y')
            {
                currentPlayer.money -= PROPERTY_PRICES[propertyPos];
                currentPlayer.properties[propertyPos] = true;
                cout << currentPlayer.name << " has purchased '" << PROPERTY_NAMES[propertyPos] << "'." << endl;
            }
        }
        else
        {
            cout << "You don't have enough money for this property." << endl;
        }
    }
    else if (ownerIndex != gs.currentPlayerIndex)
    {
        Player owner = gs.players[ownerIndex];
        int rent = PROPERTY_RENTS[propertyPos];
        if (ownsFullSet(owner, propertyPos))
        {
            cout << "WATCH OUT! " << owner.name << " owns the complete set. Rent is multiplied by " << RENT_MULTIPLIER << "!" << endl;
            rent *= RENT_MULTIPLIER;
        }
        cout << "This property belongs to " << owner.name << ". You must pay a rent of $" << rent << "." << endl;
        currentPlayer.money -= rent;
        owner.money += rent;
        gs.players[ownerIndex] = owner;
    }
    else
    {
        cout << "You landed on your own property. You are safe at home!" << endl;
    }
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}

// Manages the logic for special cards.
GameState handleSpecialCard(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    int effect = rand() % 3; // Random effect from 0 to 2.
    cout << "You landed on a Surprise Card tile!" << endl;
    if (effect == 0)
    {
        cout << "You found a treasure! You earn $100. 💎" << endl;
        currentPlayer.money += 100;
    }
    else if (effect == 1)
    {
        cout << "Pay a speeding ticket. You lose $50. 💸" << endl;
        currentPlayer.money -= 50;
    }
    else
    {
        cout << "You got a 'Get Out of Jail Free' card! 🔑" << endl;
        currentPlayer.getOutOfJailCards++;
    }
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}

// Sends the current player to the jail tile.
GameState handleGoToJail(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << "Go directly to Jail! Do not pass GO, do not collect $" << GO_BONUS << "." << endl;
    currentPlayer.position = JAIL_POSITION;
    currentPlayer.turnsInJail = JAIL_TURNS_TO_SKIP + 1;
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}

// Manages the turn for a player who is currently in jail.
GameState handleJailTurn(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << currentPlayer.name << " is in jail." << endl;
    if (currentPlayer.getOutOfJailCards > 0)
    {
        char choice;
        do
        {
            cout << "You have a Get Out of Jail Free card. Use it? (y/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = tolower(choice);
            if (choice != 'y' && choice != 'n')
                cout << "Error: Invalid option." << endl;
        } while (choice != 'y' && choice != 'n');

        if (choice == 'y')
        {
            currentPlayer.getOutOfJailCards--;
            currentPlayer.turnsInJail = 0; // Player is now free.
            cout << "You used the card. You are free!" << endl;
        }
    }

    // If they are still in jail after the choice, decrement their sentence.
    if (currentPlayer.turnsInJail > 0)
    {
        currentPlayer.turnsInJail--;
        if (currentPlayer.turnsInJail > 0)
        {
            cout << "You remain in jail. You have " << currentPlayer.turnsInJail << " more turn(s) to wait." << endl;
        }
        else
        {
            cout << "You have served your sentence. You can move on your next turn." << endl;
        }
    }

    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}

// Manages tax payment.
GameState handleTax(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << "Income Tax. You must pay $" << TAX_AMOUNT << "." << endl;
    currentPlayer.money -= TAX_AMOUNT;
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}