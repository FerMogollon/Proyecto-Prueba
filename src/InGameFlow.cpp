#define WIN32_LEAN_AND_MEAN

#include "InGameFlow.h"
#include "Constants.h"
#include "Player.h"
#include "GameIO.h"
#include "Board.h"
#include "Console.h"
#include "Validation.h"

#include <iostream>
#include <string>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ===================================================================
// IMPLEMENTATION OF THE MAIN GAME FLOW
// This file contains the main game loop and the functions that
// combine all system-level initializations.
// handle the game setup, UI drawing, and player turns.
// ===================================================================

// Initializes the system settings like console encoding and random number generator.
void initializeSystem()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    initializeRandom();
}

// Handles the entire welcome and game setup process.
GameState setupGame()
{
    GameState gs;

    clearScreen();
    printAt(2, 1, "========================================");
    printAt(2, 2, "      WELCOME TO MINIMOPOLY 🏁");
    printAt(2, 3, "========================================");

    if (doesSaveExist())
    {
        SavedPlayerNames names = getSavedPlayerNames();
        printAt(2, 5, "A saved game was found:");
        printAt(2, 6, " -> " + names.p1_name + " vs " + names.p2_name);
        printAt(2, 8, "What do you want to do?");
        printAt(2, 9, "1. Load game 💾");
        printAt(2, 10, "2. Start a new game (the previous one will be deleted) 🚮");

        moveCursor(2, 12);
        int choice = getValidInput(1, 2);

        if (choice == 1)
        {
            printAt(2, 14, "Loading saved game...");
            gs.players[0] = loadPlayer(names.p1_name);
            gs.players[1] = loadPlayer(names.p2_name);
            gs.players[0].icon = P1_ICON; // Assign the car icon to player 1
            gs.players[1].icon = P2_ICON; // Assign the hat icon to player 2
        }
        else
        {
            deleteOldSaveFiles();
            clearScreen();
            printAt(2, 1, "Previous files deleted. Creating new game...");
            string name1, name2;
            printAt(2, 3, "Player 1 name: ");
            cin >> name1;
            printAt(2, 4, "Player 2 name: ");
            cin >> name2;
            gs.players[0] = createNewPlayer(name1);
            gs.players[1] = createNewPlayer(name2);
            gs.players[0].icon = P1_ICON; // Assign the car icon to player 1
            gs.players[1].icon = P2_ICON; // Assign the hat icon to player 2
        }
    }
    else
    {
        printAt(2, 5, "No saved games found.");
        printAt(2, 6, "Starting new game...");
        string name1, name2;
        printAt(2, 8, "Player 1 name: ");
        cin >> name1;
        printAt(2, 9, "Player 2 name: ");
        cin >> name2;
        gs.players[0] = createNewPlayer(name1);
        gs.players[1] = createNewPlayer(name2);
        gs.players[0].icon = P1_ICON; // Assign the car icon to player 1
        gs.players[1].icon = P2_ICON; // Assign the hat icon to player 2
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    gs.currentPlayerIndex = 0;
    gs.isGameOver = false;
    return gs;
}

// Draws the entire game UI for the current turn.
void drawUI(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];

    clearScreen();

    string title = " MINIMOPOLY - TURN OF " + currentPlayer.name + " ";
    printAt(2, 0, title);

    printBoard(gs.players[0], gs.players[1]);
    printAt(2, 14, "Leyenda:  " + gs.players[0].icon + " " + gs.players[0].name + "   " + gs.players[1].icon + " " + gs.players[1].name);

    printAt(55, 2, "--- STATUS ---");
    printAt(55, 3, "Money: $" + to_string(currentPlayer.money) + " 💰");
    printAt(55, 4, "Properties: " + to_string(countPlayerProperties(currentPlayer)) + " 🏠");
    printAt(55, 5, "Jail Cards: " + to_string(currentPlayer.getOutOfJailCards) + " 🔑");

    printAt(55, 8, "--- MENU ---");
    printAt(55, 9, "1. Roll dice 🎲");
    printAt(55, 10, "2. View my properties");
    printAt(55, 11, "3. Save and Exit");
    printAt(55, 12, "4. Exit without saving");

    printAt(2, 18, "--- TURN MESSAGES ---");
}

// Handles the interactive menu before the player rolls the dice.
GameState handleTurnMenu(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    bool endTurnMenu = false;
    while (!endTurnMenu)
    {
        // 1. We clear the previous messages and print the menu.
        printAt(55, 14, "Choose an option: ");

        // 2. We call getValidInput to get the user's choice.
        int choice = getValidInput(1, 4);

        // This switch statement handles the user's choice.
        switch (choice)
        {
        case 1: // Roll dice
            endTurnMenu = true;
            break;

        case 2: // View properties
        {
            for (int i = 19; i < 28; ++i)
                printAt(2, i, "                                                         ");
            printAt(2, 19, currentPlayer.name + "'s properties:");
            int line = 20;
            int propCount = 0;
            for (int i = 0; i < BOARD_PERIMETER; ++i)
            {
                if (currentPlayer.properties[i])
                {
                    printAt(4, line++, "-> " + BOARD_SYMBOLS[i] + " " + PROPERTY_NAMES[i]);
                    propCount++;
                }
            }
            if (propCount == 0)
                printAt(4, line, "(You don't have any properties yet)");
            // After showing properties, we wait for the user to press Enter.
            break;
        }

        case 3: // Save and Exit
            saveGame(gs);
            gs.isGameOver = true;
            endTurnMenu = true;
            break;

        case 4: // Exit without saving
            gs.isGameOver = true;
            endTurnMenu = true;
            break;
        }
    }
    return gs;
}

// Executes the main action of a turn (roll, move, land).
GameState performTurnAction(GameState gs)
{
    Player currentPlayer = gs.players[gs.currentPlayerIndex];

    for (int i = 19; i < 28; ++i)
        printAt(2, i, "                                                         ");
    moveCursor(2, 19);

    if (currentPlayer.turnsInJail > 0)
    {
        gs = handleJailTurn(gs);
    }
    else
    {
        int diceRoll = rollDice();
        cout << currentPlayer.name << " rolled a " << diceRoll << "!" << endl;

        int oldPos = currentPlayer.position;
        gs.players[gs.currentPlayerIndex] = movePlayer(currentPlayer, diceRoll);
        currentPlayer = gs.players[gs.currentPlayerIndex];

        moveCursor(2, 20);
        if (currentPlayer.position < oldPos)
        {
            cout << "Passed GO and collects $" << to_string(GO_BONUS) << "!" << endl;
            moveCursor(2, 21);
        }
        cout << "Landed on: " << PROPERTY_NAMES[currentPlayer.position] << endl;

        moveCursor(2, 23);
        switch (BOARD_LAYOUT[currentPlayer.position])
        {
        case T_PROPERTY:
            gs = handleProperty(gs);
            break;
        case T_CARD:
            gs = handleSpecialCard(gs);
            break;
        case T_TAX:
            gs = handleTax(gs);
            break;
        case T_GO_TO_JAIL:
            gs = handleGoToJail(gs);
            break;
        default:
            break;
        }
    }
    return gs;
}

// Checks for game-over conditions.
GameState checkGameOver(GameState gs)
{
    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        if (gs.players[i].money < BANKRUPTCY_LIMIT)
        {
            gs.players[i].isBankrupt = true;
            gs.isGameOver = true;
            printAt(2, 26, "!" + gs.players[i].name + " has gone bankrupt! 📉");
            printAt(2, 28, "Press Enter to see the winner...");
            cin.get();
        }
    }
    int propertiesOwned = 0;
    for (int i = 0; i < BOARD_PERIMETER; ++i)
    {
        if (BOARD_LAYOUT[i] == T_PROPERTY && getPropertyOwnerIndex(gs, i) != -1)
        {
            propertiesOwned++;
        }
    }
    if (propertiesOwned >= MAX_PROPERTIES)
    {
        gs.isGameOver = true;
        printAt(2, 26, "!All properties have been bought!");
        printAt(2, 28, "Press Enter to see the winner...");
        cin.get();
    }
    return gs;
}

// Displays the final winner screen.
void showWinner(GameState gs)
{
    clearScreen();
    Player winner;
    if (gs.players[0].isBankrupt)
    {
        winner = gs.players[1];
    }
    else if (gs.players[1].isBankrupt)
    {
        winner = gs.players[0];
    }
    else
    {
        int propsP1 = countPlayerProperties(gs.players[0]);
        int propsP2 = countPlayerProperties(gs.players[1]);
        if (propsP1 > propsP2)
        {
            winner = gs.players[0];
        }
        else if (propsP2 > propsP1)
        {
            winner = gs.players[1];
        }
        else
        {
            if (gs.players[0].money > gs.players[1].money)
            {
                winner = gs.players[0];
            }
            else
            {
                winner = gs.players[1];
            }
        }
    }
    printAt(1, 1, "==================== END OF GAME ====================");
    printAt(1, 3, "!!! The winner is " + winner.name + " 🏆 !!!");
    printAt(1, 5, "====================================================");
    moveCursor(0, 7);
}

// Pauses the screen and waits for user input before clearing for the next turn.
void pauseForNextTurn()
{
    printAt(2, 28, "Press Enter to pass the turn...");
    cin.get();
}
