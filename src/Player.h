// src/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Constants.h"
#include <string>

// Declaración anticipada de la estructura Box
// Esto es importante para que showPlayerStatus pueda saber que Box existe
// sin tener que incluir Board.h aquí, evitando dependencias circulares.
struct Box;

using namespace std;

// Data estructural para el jugador
struct Player
{
    string Name;
    int ID;             // ¡Este es el campo que corregimos!
    int Cash;
    int Position;
    int Properties[MAX_PROPERTIES];
    int totalProperties;
    int Cards [MAX_CARDS];
    int numCards;
    bool inJail;
    int turnsInJail;
};

// Declaracion de las funciones (Prototipos)
// createPlayers sigue recibiendo Players[] porque lo modifica directamente,
// y como es un array, C++ lo pasa como puntero a su primer elemento,
// lo que permite que se modifique el array original aunque no usemos 'referencias' explícitas.
void createPlayers(Player Players[], int Amount);

// showPlayerStatus puede recibir Player por valor porque solo muestra información, no la modifica.
// El tablero se pasa como arreglo porque es lo que permite C++ sin 'referencias' explícitas a arrays.
void showPlayerStatus(Player player, Box board[SIZE][SIZE]);


#endif