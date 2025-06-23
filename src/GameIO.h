// src/GameIO.h
#ifndef GAMEIO_H
#define GAMEIO_H

// Incluimos Board.h y Player.h para que sepa las estructuras Box y Player
#include "Board.h" // Incluye Board.h que a su vez incluye Player.h
#include "Constants.h" // Para MAX_PLAYERS, SIZE, TILES, MAX_PROPERTIES

// Declaramos las variables globales
extern Player Players[MAX_PLAYERS];
extern Box board[SIZE][SIZE];

// Prototipos de funciones
void saveGame(); // No necesita parámetros si Players y board son globales

// Nueva función: verifica si hay archivos de guardado y devuelve true si existen ambos
bool checkSavedGame();

// Nueva función: carga los datos del juego (jugadores y tablero)
void loadGameData();

// Eliminamos el prototipo de deleteSaveFiles() de aquí, ya que la moveremos a main.cpp
// o la encapsularemos mejor si es una decisión de diseño para ti.
// Por ahora, la usaremos directamente en main.cpp, no como función global.

#endif