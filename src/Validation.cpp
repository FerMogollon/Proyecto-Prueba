#include "Validation.h"
#include "Console.h" // For printAt function
#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// Gets a valid integer from the user within a specified range [min, max].
// This function NO LONGER prints its own prompt. It only reads and validates.
int getValidInput(int min, int max) {
    int choice = 0;
    string input;
    do {
        // The prompt is now printed by the calling function (e.g., handleTurnMenu).
        // This function just waits for the user to type.
        getline(cin, input);
        
        try {
            choice = stoi(input);
        } catch (...) {
            choice = -1;
        }

        if (choice < min || choice > max) {
            // We can print an error message here, which is fine.
            printAt(55, 15, "Invalid option. Try again.  ");
        }

    } while (choice < min || choice > max);
    
    // Clear the error message line after a valid input.
    printAt(55, 15, "                            ");
    return choice;
}

// Gets a valid 'y' (yes) or 'n' (no) character from the user.
char getYesNoInput() {
    char choice;
    string input;
    do {
        getline(cin, input);
        if (input.length() == 1) {
            choice = tolower(input[0]);
        } else {
            choice = ' '; 
        }

        if (choice != 'y' && choice != 'n') {
            cout << " Error: Please enter 'y' or 'n'. Try again: ";
        }
    } while (choice != 'y' && choice != 'n');
    return choice;
}