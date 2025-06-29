// This define slims down the windows.h header to prevent name conflicts (like 'byte').
// It MUST be defined before including windows.h or any file that includes it.
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <limits>   // Required for numeric_limits (used in input validation).
#include <cstdlib>  // Required for system() (used for console resizing).

// Platform-specific include for Windows API functions.
#ifdef _WIN32
#include <windows.h>
#endif

// Include all the custom modules of our project.
#include "../src/Constants.h"
#include "../src/Player.h"
#include "../src/GameIO.h"
#include "../src/Board.h"
#include "../src/GameActions.h"
#include "../src/Console.h"

using namespace std;

// --- Function Prototypes ---
// Declares the helper functions that are defined at the bottom of this file.
GameState checkGameOver(GameState gs);
void showWinner(GameState gs);
int getValidInput(int min, int max);

// --- Main Function ---
// This is the entry point of the program and orchestrates the entire game flow.
int main()
{
    // --- Initial Console Setup ---
    #ifdef _WIN32
        // On Windows, set the console's input and output code pages to UTF-8
        // to ensure emojis and special characters display correctly.
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
    initializeRandom(); // Seed the random number generator for the dice.

    // This struct holds the entire state of the game.
    GameState gameState;

    // --- Welcome Screen and Load/New Game Logic ---
    clearScreen();
    printAt(1, 1, "========================================");
    printAt(1, 2, "      WELCOME TO MINI-MONOPOLY 🏁");
    printAt(1, 3, "========================================");
    
    // Check if a save file exists to offer loading it.
    if (doesSaveExist())
    {
        SavedPlayerNames names = getSavedPlayerNames();
        printAt(1, 5, "A saved game has been found:");
        printAt(1, 6, " -> " + names.p1_name + " vs " + names.p2_name);
        printAt(1, 8, "What do you want to do?");
        printAt(1, 9, "1. Load game 💾");
        printAt(1, 10, "2. Start a new game (previous game will be deleted) 🚮");
        
        moveCursor(1, 12);
        int choice = getValidInput(1, 2);

        if (choice == 1)
        {
            printAt(1, 14, "Loading saved game...");
            gameState.players[0] = loadPlayer(names.p1_name);
            gameState.players[1] = loadPlayer(names.p2_name);
        }
        else
        {
            deleteOldSaveFiles();
            clearScreen();
            printAt(1, 1, "Previous save files deleted. Creating a new game...");
            string name1, name2;
            printAt(1, 3, "Name of Player 1: "); cin >> name1;
            printAt(1, 4, "Name of Player 2: "); cin >> name2;
            gameState.players[0] = createNewPlayer(name1);
            gameState.players[1] = createNewPlayer(name2);
        }
    }
    else
    {
        printAt(1, 5, "No saved games were found.");
        printAt(1, 6, "Starting a new game...");
        string name1, name2;
        printAt(1, 8, "Name of Player 1: "); cin >> name1;
        printAt(1, 9, "Name of Player 2: "); cin >> name2;
        gameState.players[0] = createNewPlayer(name1);
        gameState.players[1] = createNewPlayer(name2);
    }
    
    // Clear any leftover characters from the input buffer before starting the game.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Initialize game state variables.
    gameState.currentPlayerIndex = 0;
    gameState.isGameOver = false;

    // --- Main Game Loop ---
    // The game will continue as long as this condition is true.
    while (!gameState.isGameOver)
    {
        clearScreen(); // Clear the screen at the start of every turn.

        Player currentPlayer = gameState.players[gameState.currentPlayerIndex];
        
        // --- UI Rendering Phase ---
        // All visual elements are drawn to specific coordinates.
        string title = " MINIMOPOLY - TURN OF " + currentPlayer.name + " ";
        printAt(2, 0, title);
        
        // The printBoard function draws the board and player positions.
        // It's always called with Player 0 and Player 1 in the same order
        // to ensure the '1' and '2' icons are consistent.
        printBoard(gameState.players[0], gameState.players[1]);
        
        // Status Panel
        printAt(55, 2, "--- STATUS ---");
        printAt(55, 3, "Money: $" + to_string(currentPlayer.money) + " 💰");
        printAt(55, 4, "Properties: " + to_string(countPlayerProperties(currentPlayer)) + " 🏠");
        printAt(55, 5, "Jail Cards: " + to_string(currentPlayer.getOutOfJailCards) + " 🔑");
        
        // Menu Panel
        printAt(55, 8, "--- MENU ---");
        printAt(55, 9, "1. Roll dice 🎲");
        printAt(55, 10, "2. View my properties");
        printAt(55, 11, "3. Save and Exit");
        printAt(55, 12, "4. Exit without saving");

        // Message Area Header
        printAt(2, 18, "--- MESSAGES OF THE TURN ---");

        // --- Menu Interaction Loop ---
        int choice;
        bool endTurn = false;
        while (!endTurn)
        {
             choice = getValidInput(1, 4);
             
             if (choice == 1)
             {
                 endTurn = true; // Exit the menu loop to proceed with the turn.
             }
             else if (choice == 2)
             {
                 // Clear the message area before displaying the properties.
                 for(int i = 19; i < 28; ++i) printAt(2, i, "                                                         ");
                 
                 printAt(2, 19, "Properties of " + currentPlayer.name + ":");
                 
                 int line = 20; // Initial Y coordinate for the list.
                 int propCount = 0;
                 
                 // Loop through all board positions.
                 for (int i = 0; i < BOARD_PERIMETER; ++i) {
                     // If the player owns the property at index 'i'...
                     if (currentPlayer.properties[i]) {
                         // ...print it to the screen with its corresponding symbol.
                         printAt(4, line++, "-> " + BOARD_SYMBOLS[i] + " " + PROPERTY_NAMES[i]);
                         propCount++;
                     }
                 }
                 
                 if (propCount == 0) {
                     printAt(4, line, "(You have no properties yet)");
                 }
             } 
             else if (choice == 3) { saveGame(gameState); return 0; }
             else if (choice == 4) { return 0; }
        }

        // --- Turn Action Phase ---
        // Clear the message area again before showing turn results.
        for(int i = 19; i < 28; ++i) printAt(2, i, "                                                         ");
        
        moveCursor(2, 19); // Position cursor for game messages.
        if (currentPlayer.turnsInJail > 0)
        {
            gameState = handleJailTurn(gameState);
        }
        else
        {
            int diceRoll = rollDice();
            cout << currentPlayer.name << " rolled a " << diceRoll << "!" << endl;
            
            int oldPos = currentPlayer.position;
            gameState.players[gameState.currentPlayerIndex] = movePlayer(currentPlayer, diceRoll);
            currentPlayer = gameState.players[gameState.currentPlayerIndex]; // Refresh player data after moving.
            
            moveCursor(2, 20);
            if(currentPlayer.position < oldPos) {
                cout << "Passed GO and collects $" << to_string(GO_BONUS) << "!" << endl;
                moveCursor(2, 21);
            }
            cout << "Landed on: " << PROPERTY_NAMES[currentPlayer.position] << endl;
            
            moveCursor(2, 23); // Position cursor for action messages.
            
            // This switch statement handles the logic for landing on different tile types.
            switch (BOARD_LAYOUT[currentPlayer.position])
            {
                case T_PROPERTY:   gameState = handleProperty(gameState); break;
                case T_CARD:       gameState = handleSpecialCard(gameState); break;
                case T_TAX:        gameState = handleTax(gameState); break;
                case T_GO_TO_JAIL: gameState = handleGoToJail(gameState); break;
                // START, JAIL, and PARKING tiles have no special action, so they do nothing.
                default: break;
            }
        }

        // --- End of Turn Phase ---
        gameState = checkGameOver(gameState);
        
        if (!gameState.isGameOver)
        {
            printAt(2, 28, "Press Enter to pass the turn...");
            cin.get(); // Pause the game until the user presses Enter.
            
            // This is the crucial line that switches to the next player.
            gameState.currentPlayerIndex = (gameState.currentPlayerIndex + 1) % NUM_PLAYERS;
        }
    }

    // --- Game Over ---
    clearScreen();
    showWinner(gameState);
    printAt(0, 8, ""); // Move cursor to a clean line before exiting.
    
    return 0;
}


// --- Helper Function Implementations ---

// Gets a valid integer input from the user within a specified range.
int getValidInput(int min, int max)
{
    int choice = 0;
    string input;
    do
    {
        printAt(55, 14, "Choose an option: ");
        printAt(73, 14, "          "); 
        moveCursor(73, 14);
        
        getline(cin, input);
        
        try {
            choice = stoi(input); // Convert string to integer.
        } catch (...) {
            choice = -1; // If conversion fails, it's an invalid input.
        }

    } while (choice < min || choice > max);
    return choice;
}

// Checks for game-over conditions (bankruptcy or all properties sold).
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
            return gs;
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
        printAt(2, 26, "All properties have been purchased!");
        printAt(2, 28, "Press Enter to see the winner...");
        cin.get();
    }
    return gs;
}

// Determines and displays the winner based on the game-over conditions.
void showWinner(GameState gs)
{
    Player winner;
    if (gs.players[0].isBankrupt) { winner = gs.players[1]; } 
    else if (gs.players[1].isBankrupt) { winner = gs.players[0]; } 
    else
    {
        // Tie-breaker logic: most properties, then most money.
        int propsP1 = countPlayerProperties(gs.players[0]);
        int propsP2 = countPlayerProperties(gs.players[1]);
        if (propsP1 > propsP2) { winner = gs.players[0]; } 
        else if (propsP2 > propsP1) { winner = gs.players[1]; } 
        else {
            if (gs.players[0].money > gs.players[1].money) { winner = gs.players[0]; }
            else { winner = gs.players[1]; }
        }
    }
    printAt(1, 1, "==================== GAME OVER ====================");
    printAt(1, 3, "!!! The winner is " + winner.name + " 🏆 !!!");
    printAt(1, 5, "===================================================");
}