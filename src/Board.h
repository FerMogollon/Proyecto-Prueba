#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include "Player.h"
#include "Constants.h" ✅ ¡IMPORTANTE! Para que se reconozca la estructura Player

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
    int Owner;     // -1 = sin dueño
    int Houses;
    bool Hotel;
};

// === Prototipos ===

// --- Tablero ---
Box asignBox(int id);
void loadBoard(Box board[SIZE][SIZE]);
void showBoard(Box board[SIZE][SIZE]);

// --- Movimiento ---
int rollDice(); // Genera un número aleatorio del 1 al 6
Player movePlayer(Player player, int steps); // Devuelve el jugador actualizado
void positionToCoords(int position, int coords[2]); // Convierte posición a coordenadas
Box findBoxByID(Box board[SIZE][SIZE], int id); // Busca casilla por ID lineal

// --- Visualización con jugadores ---
void showBoardWithPlayers(Box board[SIZE][SIZE], Player Players[], int numPlayers);


#endif
