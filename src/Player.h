//Definimos nuestra libreria
#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Constants.h"

#include <string>

using namespace std;


//Data estructural para el jugador
struct Player 
{
    char Name [25];

    int Cash;
    int Position;
    int Properties[MAX_PROPERTIES];
    int totalProperties;
    int Cards [MAX_CARDS]; //Cuantas cartas de tipo salir de carcel puede almacernar (manejar segun id)
    int numCards;   // conteo de cartas, pues el array de arriba no siempre estara lleno.
    bool inJail;
    int turnsInJail;
};

//declaracion de la funcion (Prototipo)
void createPlayers(Player Players[], int Amount);
void showPlayerStatus(Player player, Box board[SIZE][SIZE]);



#endif