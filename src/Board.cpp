#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Board.h"
#include "Constants.h"

using namespace std;


// Devuelve una casilla según su ID
Box asignBox(int id)
{
    Box box;

    string names[TILES] = {
        "GO", "Prop 1", "Card", "Prop 2", "Tax",
        "Prop 3", "Card", "Prop 4", "Prop 5", "Jail",
        "Parking", "Prop 6", "Tax", "Prop 7", "Prop 8",
        "Go to Jail", "Prop 9", "Card", "Prop 10", "Prop 11"};

    int types[TILES] = {
        TYPE_GO, TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_TAX,
        TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_PROPERTY, TYPE_JAIL,
        TYPE_PARKING, TYPE_PROPERTY, TYPE_TAX, TYPE_PROPERTY, TYPE_PROPERTY,
        TYPE_GOTOJAIL, TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_PROPERTY};

    //Precio de cada casilla 
    int prices[TILES] = {
        0, 100, 0, 120, 0,
        150, 0, 180, 200, 0,
        0, 220, 0, 240, 260,
        0, 280, 0, 300, 320};

    // Renta de cada casilla
    int rents[TILES] = {
        0, 10, 0, 12, 50,
        14, 0, 16, 18, 0,
        0, 20, 50, 22, 24,
        0, 26, 0, 28, 30};

    box.Name = names[id];
    box.Type = types[id];
    box.Price = prices[id];
    box.Rent = rents[id];
    box.Owner = -1;
    box.Houses = 0;
    box.Hotel = false;

    return box;
}

// Carga el tablero con casillas en el borde
void loadBoard(Box board[SIZE][SIZE])
{
    // Inicializar vacíos
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            board[r][c] = {"", -1, 0, 0, -1, 0, false};
        }
    }

    int id = 0;

    // Fila inferior (abajo → izquierda)
    for (int col = SIZE - 1; col >= 0 && id < TILES; col--)
    {
        board[SIZE - 1][col] = asignBox(id);
        id++;
    }

    // Columna izquierda (abajo → arriba, sin repetir esquina)
    for (int row = SIZE - 2; row >= 0 && id < TILES; row--)
    {
        board[row][0] = asignBox(id);
        id++;
    }

    // Fila superior (izquierda → derecha, sin repetir esquina)
    for (int col = 1; col < SIZE && id < TILES; col++)
    {
        board[0][col] = asignBox(id);
        id++;
    }

    // Columna derecha (arriba → abajo, sin repetir esquina)
    for (int row = 1; row < SIZE - 1 && id < TILES; row++)
    {
        board[row][SIZE - 1] = asignBox(id);
        id++;
    }
}

// Representar una casilla con un símbolo
char symbol(Box b)
{
    switch (b.Type)
    {
    case TYPE_GO:
        return 'S';
    case TYPE_PROPERTY:
        return 'P';
    case TYPE_CARD:
        return 'C';
    case TYPE_TAX:
        return '$';
    case TYPE_JAIL:
        return 'J';
    case TYPE_GOTOJAIL:
        return 'X';
    case TYPE_PARKING:
        return 'E';
    default:
        return ' ';
    }
}

// Mostrar tablero completo
void showBoard(Box board[SIZE][SIZE])
{
    cout << "\n=== TABLERO ===\n";
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            cout << "[" << symbol(board[r][c]) << "]";
        }
        cout << "\n";
    }
    cout << "===============\n";
}

/*--------------------------------------------------------------*/
//Movimiento del jugador en el tablero

int rollDice ()
{
    return rand() % 6 + 1;
}

Player movePlayer (Player player, int Steps)
{
    player.Position = (player.Position + Steps) % TILES;
    return player;
} 

void positionToCoords(int position, int coords[2])
{
    if (position < 6) // fila inferior (0 a 5)
    {
        coords[0] = 5;
        coords[1] = 5 - position;
    }
    else if (position < 11) // columna izquierda (6 a 10)
    {
        coords[0] = 5 - (position - 6) - 1;
        coords[1] = 0;
    }
    else if (position < 16) // fila superior (11 a 15)
    {
        coords[0] = 0;
        coords[1] = (position - 11) + 1;
    }
    else // columna derecha (16 a 19)
    {
        coords[0] = (position - 16) + 1;
        coords[1] = 5;
    }
}

Box findBoxByID(Box board[SIZE][SIZE], int id)
{
    int count = 0;
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            if (board[r][c].Type != -1)
            {
                if (count == id)
                {
                    return board[r][c];
                }
                count++;
            }
        }
    }
    return board[0][0]; // fallback
}

void showBoardWithPlayers(Box board[SIZE][SIZE], Player players[], int numPlayers)
{
    char display[SIZE][SIZE];

    // Llenar con símbolo base
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            display[r][c] = symbol(board[r][c]);
        }
    }

    // Colocar jugadores
    for (int i = 0; i < numPlayers; i++)
    {
        int coords[2];
        positionToCoords(players[i].Position, coords);
        display[coords[0]][coords[1]] = '1' + i; // '1' para jugador 0, '2' para jugador 1, etc.
    }

    // Mostrar tablero
    cout << "\n=== TABLERO CON JUGADORES ===\n";
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            cout << "[" << display[r][c] << "]";
        }
        cout << "\n";
    }
    cout << "=============================\n";
}
