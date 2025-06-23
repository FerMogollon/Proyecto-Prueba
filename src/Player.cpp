#include <iostream>
#include <cstring>     // para strcpy
#include "Board.h"
#include "Constants.h"

using namespace std;

void createPlayers(Player Players[], int Amount)
{
    for (int i = 0; i < Amount; i++)
    {
        cout << "Enter the name of player number " << i + 1 << ": ";
        char inputName[25];
        cin >> inputName;
        strcpy(Players[i].Name, inputName);  // Guardar nombre en el struct

        // Inicializar datos
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

    // Mostrar el nombre de la casilla en la que está
    Box current = findBoxByID(board, player.Position);
    cout << " (" << current.Name << ")" << endl;

    // Mostrar si está en la cárcel
    cout << "En la cárcel: " << (player.inJail ? "Sí" : "No") << endl;

    // Mostrar cartas
    cout << "Cartas de salir de la cárcel: " << player.numCards << endl;

    // Mostrar propiedades
    cout << "Propiedades poseídas: " << player.totalProperties << endl;
    if (player.totalProperties > 0)
    {
        cout << "ID de propiedades: ";
        for (int i = 0; i < player.totalProperties; i++)
        {
            cout << player.Properties[i];
            if (i < player.totalProperties - 1)
                cout << ", ";
        }
        cout << endl;
    }

    cout << "-------------------------------\n";
}
