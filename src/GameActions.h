#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "Player.h"

using namespace std;

// ===================================================================
// DECLARACIONES DE LAS ACCIONES PRINCIPALES DEL JUEGO
// ===================================================================


// La estructura 'GameState' es el contenedor central que mantiene
// todo el estado del juego, facilitando pasarlo entre funciones.
struct GameState {
    Player players[NUM_PLAYERS];    // Array con los dos jugadores.
    int currentPlayerIndex;         // Índice del jugador actual (0 o 1).
    bool isGameOver;                // Flag que indica si el juego ha terminado.
};


// --- Prototipos de Funciones ---

// Inicializa el generador de números aleatorios.
void initializeRandom();

// Simula el lanzamiento de un dado (devuelve 1-6).
int rollDice();

// Devuelve el índice del jugador dueño de una propiedad, o -1 si no tiene dueño.
int getPropertyOwnerIndex(GameState gs, int propertyPosition);

// Lógica para cuando un jugador cae en una propiedad.
GameState handleProperty(GameState gs);

// Lógica para cuando un jugador cae en una carta especial.
GameState handleSpecialCard(GameState gs);

// Lógica para enviar a un jugador a la cárcel.
GameState handleGoToJail(GameState gs);

// Lógica del turno de un jugador que está en la cárcel.
GameState handleJailTurn(GameState gs);

// Lógica para cuando un jugador cae en una casilla de impuesto.
GameState handleTax(GameState gs);

#endif // GAME_ACTIONS_H