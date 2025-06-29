// This MUST be defined before any includes to prevent name conflicts on Windows.
// It slims down the windows.h header, avoiding the 'byte' ambiguity error.
#define WIN32_LEAN_AND_MEAN

#include "Console.h"
#include <iostream>

// ===================================================================
// IMPLEMENTATION OF CONSOLE UTILITY FUNCTIONS
// This file isolates all operating system-specific code required
// to manipulate the console (clearing the screen, moving the cursor).
// ===================================================================

// Use platform-specific headers.
#ifdef _WIN32
#include <windows.h> // The main header for Windows API functions.
#else
#include <sys/ioctl.h> // Required for some console operations on POSIX systems.
#include <unistd.h>    // Required for some console operations on POSIX systems.
#endif

using namespace std;

// Moves the console cursor to a specific (x, y) coordinate.
void moveCursor(int x, int y)
{
    // This block is only compiled on Windows.
    #ifdef _WIN32
        // COORD is a Windows struct that holds screen coordinates.
        COORD coord = {(SHORT)x, (SHORT)y};
        // SetConsoleCursorPosition is the Windows API function that moves the cursor.
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    // This block is compiled on any system that is NOT Windows (e.g., Linux, macOS).
    #else
        // It uses ANSI escape codes, which are special commands most terminals understand.
        // The format is "\033[row;colH".
        cout << "\033[" << y + 1 << ";" << x + 1 << "H";
    #endif
}

// Clears the entire console screen and moves the cursor to the top-left corner.
void clearScreen()
{
    #ifdef _WIN32
        // This is a professional way to clear the screen on Windows that avoids flickering.
        // It gets a "handle" to the console, finds its size, and then fills the entire
        // character buffer with spaces, effectively erasing it.
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {0, 0};
        DWORD count;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hStdOut, &csbi);
        FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        SetConsoleCursorPosition(hStdOut, coord); // Finally, move cursor to the top-left.
    #else
        // The ANSI escape code "\033[2J" clears the entire screen.
        // The code "\033[1;1H" moves the cursor to home (row 1, col 1).
        cout << "\033[2J\033[1;1H";
    #endif
}

// A convenience function that combines moving the cursor and printing text in one call.
void printAt(int x, int y, string text)
{
    moveCursor(x, y);
    // 'flush' ensures the text is written to the console immediately.
    cout << text << flush;
}