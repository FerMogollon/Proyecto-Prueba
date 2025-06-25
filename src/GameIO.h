#ifndef GAMEIO_H
#define GAMEIO_H

#include "Player.h"
#include "GameActions.h" // Se incluye para que conozca la estructura GameState.

// ===================================================================
// DECLARACIONES DE FUNCIONES DE ENTRADA/SALIDA (GUARDADO/CARGA)
// ===================================================================

// Estructura auxiliar para devolver los dos nombres de la partida guardada.
struct SavedPlayerNames {
    string p1_name;
    string p2_name;
};


// --- Prototipos de Funciones ---

// Devuelve 'true' si existe el archivo de control "savegame.txt".
bool doesSaveExist();

// Lee y devuelve los nombres de los jugadores desde "savegame.txt".
SavedPlayerNames getSavedPlayerNames();

// Carga los datos de un único jugador desde su archivo .txt.
Player loadPlayer(string name);

// Guarda el estado completo de la partida actual.
void saveGame(GameState gs);

// Elimina todos los archivos de la partida guardada anterior.
void deleteOldSaveFiles();

#endif // GAMEIO_H