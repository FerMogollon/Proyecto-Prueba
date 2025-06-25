#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

using namespace std;

// ===================================================================
// ARCHIVO DE CONSTANTES GLOBALES
// Aquí se definen todos los valores fijos del juego para un fácil acceso y modificación.
// ===================================================================

// --- 1. Configuración General de la Partida ---
// Aquí se ajustan las reglas económicas y de flujo del juego.
// ===================================================================
const int NUM_PLAYERS = 2;          // Número de jugadores.
const int BOARD_PERIMETER = 20;     // No cambiar: El tablero está diseñado para 20 casillas.
const int MAX_PROPERTIES = 11;      // No cambiar: El número total de propiedades en el tablero.
const int STARTING_MONEY = 1500;    // Dinero con el que empieza cada jugador.
const int GO_BONUS = 200;           // Bono por pasar por la Salida.
const int JAIL_POSITION = 5;        // Posición en el array (índice 5) de la casilla de la Cárcel.
const int GO_TO_JAIL_POSITION = 15; // Posición de la casilla "Ir a la Cárcel".
const int BANKRUPTCY_LIMIT = -500;  // Límite de deuda para declarar bancarrota.
const int TAX_AMOUNT = 150;         // Costo del impuesto.
const int JAIL_TURNS_TO_SKIP = 1;   // Turnos que un jugador debe esperar en la cárcel.

// --- 2. Configuración Lógica y Visual del Tablero ---
// Define qué hay en cada casilla.
// ===================================================================
// Lógica interna (no cambiar). Se usan caracteres simples para la lógica del juego.
const char T_START = 'S', T_PROPERTY = 'P', T_CARD = 'C', T_TAX = 'I', T_JAIL = 'J', T_GO_TO_JAIL = 'G', T_PARKING = 'E';
const char BOARD_LAYOUT[BOARD_PERIMETER] = {
    T_START, T_PROPERTY, T_CARD, T_PROPERTY, T_TAX, T_GO_TO_JAIL, T_PROPERTY, T_PROPERTY, T_CARD, T_PROPERTY,
    T_PARKING, T_PROPERTY, T_TAX, T_PROPERTY, T_PROPERTY, T_JAIL, T_CARD, T_PROPERTY, T_PROPERTY, T_PROPERTY};

// NUEVO: Visualización con símbolos. Este array es para mostrar el tablero de forma bonita.
const string BOARD_SYMBOLS[BOARD_PERIMETER] = {
    "🏁", "🏠", "❔", "🏠", "💲", "🚓", "🏠", "🏠", "❔", "🏠",
    "🅿️", "🏠", "💲", "🏠", "🏠", "👮", "❔", "🏠", "🏠", "🏠"};

// --- 3. Configuración de Propiedades ---
// Arrays paralelos: El índice de cada array corresponde a la misma casilla del tablero.
// ===================================================================
// Nombres de las propiedades (Temática de Nicaragua).
const string PROPERTY_NAMES[BOARD_PERIMETER] = {
    "Salida", "Avenida Sandino", "Carta Sorpresa", "Calle Real", "Impuesto al Valor", "Vaya a la Carcel",
    "Plaza Inter", "Rotonda El Gueguense", "Carta Sorpresa", "Avenida Bolivar", "Parqueo Gratis",
    "Carretera Masaya", "Impuesto sobre la Renta", "Camino de Oriente", "Galerias Santo Domingo",
    "Carcel (de visita)", "Carta Sorpresa", "Puerto Salvador Allende", "Teatro Ruben Dario", "Metrocentro"};

// Precios de compra de cada propiedad.
const int PROPERTY_PRICES[BOARD_PERIMETER] = {
    0, 60, 0, 60, 0, 0, 100, 100, 0, 120, 0, 140, 0, 140, 160, 0, 0, 180, 180, 200};

// Renta a pagar por cada propiedad.
const int PROPERTY_RENTS[BOARD_PERIMETER] = {
    0, 10, 0, 12, 0, 0, 15, 15, 0, 18, 0, 20, 0, 20, 22, 0, 0, 25, 25, 30};

// --- 4. NUEVO: Configuración de Sets de Propiedades (Matriz) ---
// Define los grupos de propiedades y el bono por completarlos.
// ===================================================================
const int RENT_MULTIPLIER = 2; // La renta se multiplica por este valor si se tiene el set completo.
const int NUM_PROPERTY_SETS = 4;
const int MAX_PROPS_PER_SET = 3;

// Matriz que define los sets. Cada fila es un set. Los números son las posiciones en el tablero.
// -1 se usa para rellenar sets con menos de 3 propiedades.
const int PROPERTY_SETS[NUM_PROPERTY_SETS][MAX_PROPS_PER_SET] = {
    {1, 3, -1},   // Set 1: Propiedades de $60
    {6, 7, 9},    // Set 2: Propiedades de $100-$120
    {11, 13, 14}, // Set 3: Propiedades de $140-$160
    {17, 18, 19}  // Set 4: Propiedades de $180-$200
};

#endif