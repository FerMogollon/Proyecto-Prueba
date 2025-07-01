#define WIN32_LEAN_AND_MEAN

#include "Console.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

void moveCursor(int x, int y) {
#ifdef _WIN32
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
#endif
}

void clearScreen() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
#else
    cout << "\033[2J\033[1;1H";
#endif
}

void printAt(int x, int y, string text) {
    moveCursor(x, y);
    cout << text << flush;
}