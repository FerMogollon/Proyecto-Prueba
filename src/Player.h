#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Constants.h"

using namespace std;

// ===================================================================
// DEFINICIÓN DE LA ESTRUCTURA DEL JUGADOR
// ===================================================================

// La estructura 'Player' almacena toda la información de un jugador.
struct Player {
    string name;                        // Nombre del jugador.
    int money;                          // Dinero disponible.
    int position;                       // Posición actual en el tablero (0-19).
    bool properties[BOARD_PERIMETER];   // Array que indica las propiedades que posee (true si la posee).
    int getOutOfJailCards;              // Contador de cartas "Salir de la Cárcel".
    int turnsInJail;                    // Turnos restantes que debe pasar en la cárcel.
    bool isBankrupt;                    // Indica si el jugador ha entrado en bancarrota.
};

// --- Prototipos de Funciones (declaraciones) ---
// La implementación de estas funciones está en Player.cpp.

// Crea una nueva instancia de Player con valores iniciales.
Player createNewPlayer(string name);

// Muestra en consola el estado actual de un jugador.
void printPlayerStatus(Player player);

// Devuelve el número total de propiedades que posee un jugador.
int countPlayerProperties(Player player);

#endif // PLAYER_H