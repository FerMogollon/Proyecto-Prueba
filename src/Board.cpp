#include "Board.h"
#include "Constants.h"
#include <iostream>

using namespace std;

// ===================================================================
// IMPLEMENTACIÓN DE LAS FUNCIONES DEL TABLERO
// ===================================================================


// --- Estructura Auxiliar ---
// Se crea esta estructura para poder devolver dos valores (fila y columna)
// desde la función getCoords sin usar punteros ni referencias.
struct Coordinates {
    int row;
    int col;
};


// --- Función Auxiliar ---
// Convierte la posición lineal del jugador (0-19) a coordenadas de la rejilla.
// Devuelve una estructura 'Coordinates' por valor.
Coordinates getCoords(int pos) {
    Coordinates coords; // Se crea un objeto para guardar las coordenadas.
    if (pos >= 0 && pos <= 5)      { coords.row = 0; coords.col = pos; }
    else if (pos >= 6 && pos <= 9) { coords.row = pos - 5; coords.col = 5; }
    else if (pos >= 10 && pos <= 15) { coords.row = 5; coords.col = 15 - pos; }
    else if (pos >= 16 && pos <= 19) { coords.row = 19 - pos; coords.col = 0; }
    return coords; // Se devuelve la estructura completa.
}


// Implementación de la función mejorada para imprimir el tablero.
void printBoard(Player p1, Player p2) {
    // 1. Se crea una rejilla de 6x6 para representar visualmente el tablero.
    string grid[6][6];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            grid[i][j] = "     "; // Se llena el interior con espacios.
        }
    }

    // 2. Se mapea el array de SÍMBOLOS a las coordenadas de la rejilla.
    // Nota: Esta es la implementación simple que usa los emojis directamente.
    // No intenta corregir la alineación, por lo que puede verse "chueco" en la consola.
    for (int i = 0; i <= 5; ++i) grid[0][i] = "[ " + BOARD_SYMBOLS[i] + " ]";
    for (int i = 0; i < 4; ++i) grid[1 + i][5] = "[ " + BOARD_SYMBOLS[6 + i] + " ]";
    for (int i = 0; i <= 5; ++i) grid[5][5 - i] = "[ " + BOARD_SYMBOLS[10 + i] + " ]";
    for (int i = 0; i < 4; ++i) grid[4 - i][0] = "[ " + BOARD_SYMBOLS[16 + i] + " ]";

    // 3. Se obtiene la posición (fila, columna) de cada jugador usando la nueva función.
    Coordinates p1_coords = getCoords(p1.position);
    Coordinates p2_coords = getCoords(p2.position);

    // 4. Se colocan los indicadores de los jugadores ('1' y '2') en la rejilla.
    // Se accede a los valores a través de la estructura (ej: p1_coords.row).
    grid[p1_coords.row][p1_coords.col][1] = '1';
    grid[p2_coords.row][p2_coords.col][3] = '2';

    // 5. Se imprime la rejilla final y la leyenda.
    cout << "\n================== TABLERO DE JUEGO ==================" << endl;
    for (int i = 0; i < 6; ++i) {
        string line = "";
        for (int j = 0; j < 6; ++j) {
            line += grid[i][j];
        }
        cout << line << endl << endl; // Doble espacio vertical para mejor legibilidad.
    }
    cout << "======================================================" << endl;
    cout << "Leyenda:  1-" << p1.name << "   2-" << p2.name << endl;
}


// La función movePlayer, ahora actualizada para ser consistente.
Player movePlayer(Player player, int roll) {
    int oldPosition = player.position;
    // El operador módulo (%) asegura que la posición se mantenga dentro del rango 0-19.
    player.position = (oldPosition + roll) % BOARD_PERIMETER;

    // Si la nueva posición es menor que la anterior, significa que ha completado una vuelta.
    if (player.position < oldPosition) {
        cout << player.name << " ha pasado por la Salida y cobra $" << GO_BONUS << "!" << endl;
        player.money += GO_BONUS;
    }

    // --- CAMBIO IMPORTANTE ---
    // Ahora el mensaje de movimiento usa BOARD_SYMBOLS para mostrar el emoji,
    // en lugar de la letra de BOARD_LAYOUT, para que sea consistente con el tablero.
    cout << player.name << " ha sacado un " << roll << " y se mueve a la casilla " << player.position
              << " " << BOARD_SYMBOLS[player.position] << endl; 

    // Se muestra el nombre completo de la propiedad.
    if (PROPERTY_NAMES[player.position] != "") {
        cout << "Ha caido en: " << PROPERTY_NAMES[player.position] << endl;
    }

    return player;
}