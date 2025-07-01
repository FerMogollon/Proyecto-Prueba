#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

void clearScreen();
void moveCursor(int x, int y);
void printAt(int x, int y, std::string text);

#endif