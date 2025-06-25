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

    // 2. Se mapea el array lineal del tablero (0-19) a las coordenadas de la rejilla.
    // Fila superior (casillas 0-5)
    for (int i = 0; i <= 5; ++i) grid[0][i] = "[ " + BOARD_SYMBOLS[i] + " ]";
    // Columna derecha (casillas 6-9)
    for (int i = 0; i < 4; ++i) grid[1 + i][5] = "[ " + BOARD_SYMBOLS[6 + i] + " ]";
    // Fila inferior (casillas 10-15), se mapea en reversa para el dibujado.
    for (int i = 0; i <= 5; ++i) grid[5][5 - i] = "[ " + BOARD_SYMBOLS[10 + i] + " ]";
    // Columna izquierda (casillas 16-19), se mapea en reversa.
    for (int i = 0; i < 4; ++i) grid[4 - i][0] = "[ " + BOARD_SYMBOLS[16 + i] + " ]";

    // 3. Se obtiene la posición (fila, columna) de cada jugador usando la nueva función.
    Coordinates p1_coords = getCoords(p1.position);
    Coordinates p2_coords = getCoords(p2.position);

    // 4. Se colocan los indicadores de los jugadores ('1' y '2') en la rejilla.
    // Se accede a los valores a través de la estructura (ej: p1_coords.row).
    grid[p1_coords.row][p1_coords.col][1] = '1';
    grid[p2_coords.row][p2_coords.col][3] = '2';

    // 5. Se imprime la rejilla final y la leyenda.
    cout << "================== TABLERO DE JUEGO ==================" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            cout << grid[i][j];
        }
        cout << endl << endl; // Doble espacio vertical para mejor legibilidad.
    }
    cout << "=======================================================" << endl;
    cout << "Leyenda:  1-" << p1.name << "   2-" << p2.name << endl;
    cout << "Posicion " << p1.name << ": " << p1.position << " (" << PROPERTY_NAMES[p1.position] << ")" << endl;
    cout << "Posicion " << p2.name << ": " << p2.position << " (" << PROPERTY_NAMES[p2.position] << ")" << endl;
}


// La función movePlayer no cambia.
Player movePlayer(Player player, int roll) {
    int oldPosition = player.position;
    // El operador módulo (%) asegura que la posición se mantenga dentro del rango 0-19.
    player.position = (oldPosition + roll) % BOARD_PERIMETER;

    // Si la nueva posición es menor que la anterior, significa que ha completado una vuelta.
    if (player.position < oldPosition) {
        cout << player.name << " ha pasado por la Salida y cobra $" << GO_BONUS << endl;
        player.money += GO_BONUS;
    }

    cout << player.name << " ha sacado un " << roll << " y se mueve a la casilla " << player.position
              << " (" << BOARD_LAYOUT[player.position] << ")" << endl;

    if (PROPERTY_NAMES[player.position] != "") {
        cout << "Ha caido en: " << PROPERTY_NAMES[player.position] << endl;
    }

    return player;
}