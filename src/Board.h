#ifndef BOARD_H
#define BOARD_H

#include "Player.h"

// ===================================================================
// DECLARACIONES DE FUNCIONES DEL TABLERO
// ===================================================================

// Dibuja el tablero en la consola, mostrando la posición de los jugadores.
void printBoard(Player p1, Player p2);

// Calcula la nueva posición del jugador, le otorga el bono si pasa por Salida y devuelve su estado actualizado.
Player movePlayer(Player player, int roll);

#endif // BOARD_H