// src/GameIO.cpp
#include <iostream>
#include <fstream> // Para manejo de archivos (ifstream, ofstream)
#include <string>
#include <limits> // Para numeric_limits
#include "GameIO.h"    // Incluimos nuestro propio archivo de cabecera
#include "Constants.h"
#include "Player.h"    // Necesitamos las definiciones completas de Player
#include "Board.h"     // Necesitamos las definiciones completas de Box

using namespace std;

// *** Definición de las variables globales ***
Player Players[MAX_PLAYERS];
Box board[SIZE][SIZE];

// --- Funciones para Guardar el Juego (sin cambios) ---
void saveGame() {
    // ... (código saveGame existente) ...
    // Asegúrate de que este código sea idéntico al que te di antes.
    ofstream playerFile("players.txt");
    if (playerFile.is_open()) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            playerFile << Players[i].ID << ","
                       << Players[i].Name << ","
                       << Players[i].Cash << ","
                       << Players[i].Position << ","
                       << Players[i].totalProperties << ",";
            for (int j = 0; j < Players[i].totalProperties; j++) {
                playerFile << Players[i].Properties[j];
                if (j < Players[i].totalProperties - 1) playerFile << "-";
            }
            playerFile << ","
                       << Players[i].numCards << ",";
            for (int j = 0; j < Players[i].numCards; j++) {
                playerFile << Players[i].Cards[j];
                if (j < Players[i].numCards - 1) playerFile << "-";
            }
            playerFile << ","
                       << (Players[i].inJail ? 1 : 0) << ","
                       << Players[i].turnsInJail << "\n";
        }
        playerFile.close();
        cout << "Datos de jugadores guardados en players.txt" << endl;
    } else {
        cerr << "ERROR: No se pudo abrir players.txt para guardar." << endl;
    }

    ofstream boardFile("board_properties.txt");
    if (boardFile.is_open()) {
        for (int i = 0; i < TILES; i++) {
            Box currentBox = findBoxByID(board, i);
            if (currentBox.Type == TYPE_PROPERTY) {
                boardFile << currentBox.ID << ","
                          << currentBox.Owner << ","
                          << currentBox.Houses << ","
                          << (currentBox.Hotel ? 1 : 0) << "\n";
            }
        }
        boardFile.close();
        cout << "Datos de propiedades del tablero guardados en board_properties.txt" << endl;
    } else {
        cerr << "ERROR: No se pudo abrir board_properties.txt para guardar." << endl;
    }
}


// --- Nueva función: Verifica si existen los archivos de guardado ---
bool checkSavedGame() {
    ifstream playerFile("players.txt");
    ifstream boardFile("board_properties.txt");

    bool playersExist = playerFile.good(); // .good() es una buena forma de verificar si se abrió bien
    bool boardExist = boardFile.good();

    playerFile.close(); // Cierra los archivos inmediatamente después de verificar
    boardFile.close();

    return playersExist && boardExist;
}

// --- Nueva función: Carga los datos del juego si se decide continuar ---
void loadGameData() {
    // Cargar jugadores
    ifstream playerFile("players.txt");
    if (playerFile.is_open()) {
        string line;
        int playerIdx = 0;
        while (getline(playerFile, line) && playerIdx < MAX_PLAYERS) {
            size_t pos = 0;
            size_t nextPos;

            nextPos = line.find(",", pos);
            Players[playerIdx].ID = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            Players[playerIdx].Name = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            Players[playerIdx].Cash = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            Players[playerIdx].Position = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            Players[playerIdx].totalProperties = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            string propList = line.substr(pos, line.find(",", pos) - pos);
            pos = line.find(",", pos) + 1;
            if (!propList.empty()) {
                size_t propPos = 0;
                size_t nextPropPos;
                int propCount = 0;
                while ((nextPropPos = propList.find("-", propPos)) != string::npos && propCount < MAX_PROPERTIES) {
                    Players[playerIdx].Properties[propCount++] = stoi(propList.substr(propPos, nextPropPos - propPos));
                    propPos = nextPropPos + 1;
                }
                if (propCount < MAX_PROPERTIES) {
                    Players[playerIdx].Properties[propCount++] = stoi(propList.substr(propPos));
                }
                for(int i = propCount; i < MAX_PROPERTIES; ++i) {
                    Players[playerIdx].Properties[i] = -1;
                }
            } else {
                for(int i = 0; i < MAX_PROPERTIES; ++i) {
                    Players[playerIdx].Properties[i] = -1;
                }
            }

            nextPos = line.find(",", pos);
            Players[playerIdx].numCards = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            string cardList = line.substr(pos, line.find(",", pos) - pos);
            pos = line.find(",", pos) + 1;
            if (!cardList.empty()) {
                size_t cardPos = 0;
                size_t nextCardPos;
                int cardCount = 0;
                while ((nextCardPos = cardList.find("-", cardPos)) != string::npos && cardCount < MAX_CARDS) {
                    Players[playerIdx].Cards[cardCount++] = stoi(cardList.substr(cardPos, nextCardPos - cardPos));
                    cardPos = nextCardPos + 1;
                }
                if (cardCount < MAX_CARDS) {
                    Players[playerIdx].Cards[cardCount++] = stoi(cardList.substr(cardPos));
                }
                for(int i = cardCount; i < MAX_CARDS; ++i) {
                    Players[playerIdx].Cards[i] = -1;
                }
            } else {
                for(int i = 0; i < MAX_CARDS; ++i) {
                    Players[playerIdx].Cards[i] = -1;
                }
            }

            nextPos = line.find(",", pos);
            Players[playerIdx].inJail = (stoi(line.substr(pos, nextPos - pos)) == 1);
            pos = nextPos + 1;

            Players[playerIdx].turnsInJail = stoi(line.substr(pos));

            playerIdx++;
        }
        playerFile.close();
        cout << "Datos de jugadores cargados desde players.txt" << endl;
    } else {
        // Esto no debería ocurrir si checkSavedGame() devolvió true
        cerr << "ERROR: players.txt no encontrado a pesar de la verificación." << endl;
    }

    // Cargar propiedades del tablero
    ifstream boardFile("board_properties.txt");
    if (boardFile.is_open()) {
        string line;
        while (getline(boardFile, line)) {
            size_t pos = 0;
            size_t nextPos;

            nextPos = line.find(",", pos);
            int boxID = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            int ownerID = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            nextPos = line.find(",", pos);
            int houses = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            bool hotel = (stoi(line.substr(pos)) == 1);

            int coords[2];
            positionToCoords(boxID, coords);
            board[coords[0]][coords[1]].Owner = ownerID;
            board[coords[0]][coords[1]].Houses = houses;
            board[coords[0]][coords[1]].Hotel = hotel;
        }
        boardFile.close();
        cout << "Datos de propiedades del tablero cargados desde board_properties.txt" << endl;
    } else {
        cerr << "ERROR: board_properties.txt no encontrado a pesar de la verificación." << endl;
    }
}