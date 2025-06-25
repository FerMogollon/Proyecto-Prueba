#include "GameIO.h"
#include <fstream>
#include <iostream>
#include <cstdio> // Necesario para la función remove() que borra archivos.

using namespace std;

// ===================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE ENTRADA/SALIDA (GUARDADO/CARGA)
// ===================================================================


// Función auxiliar interna para guardar los datos de un solo jugador en su archivo .txt.
void savePlayer(Player player) {
    ofstream file(player.name + ".txt");
    if (file.is_open()) {
        file << player.name << endl;
        file << player.money << endl;
        file << player.position << endl;
        file << player.getOutOfJailCards << endl;
        // Guarda los índices de las propiedades que posee.
        for (int i = 0; i < BOARD_PERIMETER; ++i) {
            if (player.properties[i]) {
                file << i << " ";
            }
        }
        file << endl;
        file.close();
    }
}


// Revisa si existe un archivo de guardado maestro.
bool doesSaveExist() {
    ifstream file("savegame.txt");
    return file.is_open(); // Devuelve true si el archivo se pudo abrir (existe).
}


// Obtiene los nombres de los jugadores del archivo de guardado maestro.
SavedPlayerNames getSavedPlayerNames() {
    SavedPlayerNames names;
    ifstream file("savegame.txt");
    if (file.is_open()) {
        getline(file, names.p1_name);
        getline(file, names.p2_name);
    }
    return names;
}


// Carga los datos de un jugador. Si su archivo no existe, crea un jugador nuevo.
Player loadPlayer(string name) {
    ifstream file(name + ".txt");
    if (file.is_open()) {
        Player loadedPlayer;
        file >> loadedPlayer.name;
        file >> loadedPlayer.money;
        file >> loadedPlayer.position;
        file >> loadedPlayer.getOutOfJailCards;

        // Limpia las propiedades antes de cargar las guardadas.
        for (int i = 0; i < BOARD_PERIMETER; ++i) loadedPlayer.properties[i] = false;

        // Lee los índices de las propiedades y las asigna.
        int propIndex;
        while(file >> propIndex) {
            if(propIndex >= 0 && propIndex < BOARD_PERIMETER) {
                loadedPlayer.properties[propIndex] = true;
            }
        }
        loadedPlayer.turnsInJail = 0;
        loadedPlayer.isBankrupt = false;
        file.close();
        cout << "Datos de " << loadedPlayer.name << " cargados." << endl;
        return loadedPlayer;
    }
    cout << "No se encontraron datos para " << name << ". Creando nuevo jugador." << endl;
    return createNewPlayer(name);
}


// Guarda la partida completa.
void saveGame(GameState gs) {
    cout << "Guardando partida..." << endl;
    // 1. Guarda los datos de cada jugador en su respectivo archivo.
    savePlayer(gs.players[0]);
    savePlayer(gs.players[1]);
    // 2. Crea/sobrescribe el archivo de control con los nombres de la sesión.
    ofstream file("savegame.txt");
    if (file.is_open()) {
        file << gs.players[0].name << endl;
        file << gs.players[1].name << endl;
        file.close();
    }
    cout << "Partida guardada con exito." << endl;
}


// Elimina los archivos de la partida anterior.
void deleteOldSaveFiles() {
    if (doesSaveExist()) {
        cout << "Eliminando datos de la partida anterior..." << endl;
        SavedPlayerNames names = getSavedPlayerNames();
        // Borra los archivos de cada jugador.
        remove((names.p1_name + ".txt").c_str());
        remove((names.p2_name + ".txt").c_str());
        // Borra el archivo de control.
        remove("savegame.txt");
    }
}