#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../src/Board.h"
#include "../src/Player.h"
#include "../src/Constants.h"

using namespace std;

int main() {
    srand(time(0)); // Semilla para dados

    Player Players[MAX_PLAYERS];
    Box board[SIZE][SIZE];

    cout << "🎲 BIENVENIDO A MINI MONOPOLY 🎲" << endl;

    // Crear jugadores
    createPlayers(Players, MAX_PLAYERS);

    // Cargar tablero
    loadBoard(board);
    showBoard(board);

    // Mostrar posiciones iniciales
    showBoardWithPlayers(board, Players, MAX_PLAYERS);

    // Mover a los jugadores
    for (int i = 0; i < MAX_PLAYERS; i++) {
        cout << "\nTurno de " << Players[i].Name << ":\n";
        int dice = rollDice();
        cout << "Sacaste un " << dice << " en el dado." << endl;

        Players[i] = movePlayer(Players[i], dice);
        cout << Players[i].Name << " ahora está en la casilla #" << Players[i].Position << ": " << findBoxByID(board, Players[i].Position).Name << endl;
    }

    // Mostrar tablero actualizado
    showBoardWithPlayers(board, Players, MAX_PLAYERS);

    return 0;
}
