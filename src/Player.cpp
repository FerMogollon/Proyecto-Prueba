// src/Player.cpp
#include <iostream>
#include <string>
#include <cctype> // Para tolower
#include "Board.h" // ¡Ahora sí necesitamos la definición completa de Box aquí para findBoxByID!
#include "Constants.h"
#include "Player.h" // Incluimos la cabecera de Player para la definición de la estructura

using namespace std;


void createPlayers(Player Players[], int Amount)
{
    for (int i = 0; i < Amount; i++)
    {
        cout << "Ingresa el nombre del jugador #" << i + 1 << ": ";
        cin >> Players[i].Name;

        // Inicializar datos
        Players[i].ID = i; // Asigna el índice como ID del jugador
        Players[i].Cash = 1500;
        Players[i].Position = 0;
        Players[i].inJail = false;
        Players[i].turnsInJail = 0;

        // Inicializar propiedades
        Players[i].totalProperties = 0;
        for (int j = 0; j < MAX_PROPERTIES; j++)
        {
            Players[i].Properties[j] = -1;
        }

        // Inicializar cartas
        Players[i].numCards = 0;
        for (int j = 0; j < MAX_CARDS; j++)
        {
            Players[i].Cards[j] = -1;
        }
    }
}

void showPlayerStatus(Player player, Box board[SIZE][SIZE])
{
    cout << "\n--- ESTADO DEL JUGADOR: " << player.Name << " ---\n";
    cout << "Dinero: $" << player.Cash << endl;
    cout << "Posición actual: " << player.Position;

    // findBoxByID devuelve una copia de la Box, lo cual es correcto para solo mostrar información.
    Box current = findBoxByID(board, player.Position);
    cout << " (" << current.Name << ")" << endl;

    // Mostrar si está en la cárcel
    cout << "En la cárcel: " << (player.inJail ? "Sí" : "No");
    if (player.inJail) {
        cout << " (Turnos en cárcel: " << player.turnsInJail << ")";
    }
    cout << endl;

    // Mostrar cartas
    cout << "Cartas de salir de la cárcel: " << player.numCards << endl;

    // Mostrar propiedades
    cout << "Propiedades poseídas: " << player.totalProperties << endl;
    if (player.totalProperties > 0)
    {
        cout << "ID de propiedades: ";
        for (int i = 0; i < player.totalProperties; i++)
        {
            // findBoxByID devuelve una copia para mostrar el nombre.
            cout << player.Properties[i] << " (" << findBoxByID(board, player.Properties[i]).Name << ")";
            if (i < player.totalProperties - 1)
                cout << ", ";
        }
        cout << endl;
    }

    cout << "-------------------------------\n";
}