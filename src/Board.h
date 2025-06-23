// src/Board.h
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include "Constants.h"
#include "Player.h" // <--- ¡Añade o confirma esta línea!

using namespace std;

// Tipos de casilla personalizados
const int TYPE_GO = 0;
const int TYPE_PROPERTY = 1;
const int TYPE_CARD = 2;
const int TYPE_TAX = 3;
const int TYPE_JAIL = 4;
const int TYPE_GOTOJAIL = 5;
const int TYPE_PARKING = 6;

// Estructura de una casilla
struct Box {
    string Name;
    int Type;
    int Price;
    int Rent;
    int Owner;      // -1 = sin dueño, ID del jugador = dueño
    int Houses;
    bool Hotel;
    int ID;         // Campo para almacenar el ID lineal de la casilla
};

// === Prototipos ===

// --- Tablero ---
Box asignBox(int id);
void loadBoard(Box board[SIZE][SIZE]);
void showBoard(Box board[SIZE][SIZE]);

// --- Movimiento ---
int rollDice();
Player movePlayer(Player player, int steps);
void positionToCoords(int position, int coords[2]);
Box findBoxByID(Box board[SIZE][SIZE], int id);

// --- Visualización con jugadores ---
void showBoardWithPlayers(Box board[SIZE][SIZE], Player Players[], int numPlayers);

// --- Acciones de Casilla ---
Player handleBoxAction(Player player, Box board[SIZE][SIZE], Player Players[MAX_PLAYERS]);

#endif