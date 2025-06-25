#include "Player.h"
#include <iostream>

using namespace std;

// ===================================================================
// IMPLEMENTACIÓN DE LAS FUNCIONES DEL JUGADOR
// ===================================================================


// Implementación de la función para crear un nuevo jugador.
Player createNewPlayer(string name) {
    Player newPlayer;
    newPlayer.name = name;
    newPlayer.money = STARTING_MONEY;
    newPlayer.position = 0;
    // Inicializa el array de propiedades, asegurándose de que el jugador no posea ninguna al inicio.
    for (int i = 0; i < BOARD_PERIMETER; ++i) {
        newPlayer.properties[i] = false;
    }
    newPlayer.getOutOfJailCards = 0;
    newPlayer.turnsInJail = 0;
    newPlayer.isBankrupt = false;
    return newPlayer; // Devuelve la estructura del jugador recién creado.
}


// Implementación de la función para mostrar el estado del jugador.
void printPlayerStatus(Player player) {
    cout << "-> Nombre: " << player.name << endl;
    cout << "-> Dinero: $" << player.money << endl;
    cout << "-> Cartas 'Salir de la Carcel': " << player.getOutOfJailCards << endl;
}


// Implementación de la función para contar las propiedades de un jugador.
int countPlayerProperties(Player player) {
    int count = 0;
    // Recorre el array de propiedades y suma 1 por cada 'true' que encuentre.
    for (int i = 0; i < BOARD_PERIMETER; ++i) {
        if (player.properties[i]) {
            count++;
        }
    }
    return count;
}