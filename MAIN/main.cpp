#include <iostream>
#include <string>
#include <limits> // Necesario para la validación de entrada
#include <cstdlib> // Necesario para system()

#include "../src/Constants.h"
#include "../src/Player.h"
#include "../src/GameIO.h"
#include "../src/Board.h"
#include "../src/GameActions.h"

// Le dice a windows.h que no incluya APIs innecesarias que causan el conflicto.
#define WIN32_LEAN_AND_MEAN

// Se incluye solo en Windows para las funciones de consola
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// --- Prototipos de funciones auxiliares del main ---
GameState checkGameOver(GameState gs);
void showWinner(GameState gs);
int getValidInput(int min, int max);


// --- Función Principal ---
int main()
{
    // --- Configuración Inicial de la Consola ---
    #ifdef _WIN32
        // Prepara la consola para mostrar símbolos Unicode (UTF-8)
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        // Ajusta el tamaño de la ventana de la consola en Windows
        system("mode con: cols=90 lines=40");
    #else
        // Ajusta el tamaño en Linux/macOS usando secuencias ANSI
        cout << "\033[8;40;90t";
    #endif

    initializeRandom();
    GameState gameState;

    cout << "========================================" << endl;
    cout << "      BIENVENIDO A MINIMOPOLY 🏁" << endl; // Emoji añadido
    cout << "========================================" << endl;

    // Lógica de inicio: Comprueba si hay una partida guardada.
    if (doesSaveExist())
    {
        SavedPlayerNames names = getSavedPlayerNames();
        cout << "\nSe ha encontrado una partida guardada:" << endl;
        cout << " -> " << names.p1_name << " vs " << names.p2_name << endl;

        cout << "\nQue deseas hacer?" << endl;
        cout << "1. Cargar partida 💾" << endl; // Emoji añadido
        cout << "2. Empezar una nueva partida (se borrara la anterior) 🚮" << endl; // Emoji añadido
        int choice = getValidInput(1, 2);

        if (choice == 1)
        { // Cargar partida
            cout << "\nCargando partida guardada..." << endl;
            gameState.players[0] = loadPlayer(names.p1_name);
            gameState.players[1] = loadPlayer(names.p2_name);
        }
        else
        { // Empezar de cero
            deleteOldSaveFiles();
            cout << "\nIniciando nueva partida..." << endl;
            string name1, name2;
            cout << "Nombre del Jugador 1: ";
            cin >> name1;
            cout << "Nombre del Jugador 2: ";
            cin >> name2;
            gameState.players[0] = createNewPlayer(name1);
            gameState.players[1] = createNewPlayer(name2);
        }
    }
    else
    { // No hay partida guardada
        cout << "\nNo se encontraron partidas guardadas." << endl;
        cout << "Iniciando nueva partida..." << endl;
        string name1, name2;
        cout << "Nombre del Jugador 1: ";
        cin >> name1;
        cout << "Nombre del Jugador 2: ";
        cin >> name2;
        gameState.players[0] = createNewPlayer(name1);
        gameState.players[1] = createNewPlayer(name2);
    }

    // Inicialización del estado del juego
    gameState.currentPlayerIndex = 0;
    gameState.isGameOver = false;

    // --- Bucle Principal del Juego ---
    while (!gameState.isGameOver)
    {
        Player currentPlayer = gameState.players[gameState.currentPlayerIndex];
        Player otherPlayer = gameState.players[(gameState.currentPlayerIndex + 1) % NUM_PLAYERS];

        cout << "\n==================== TURNO DE " << currentPlayer.name << " ====================" << endl;
        printBoard(currentPlayer, otherPlayer);

        // Menú de acciones del turno
        bool endTurn = false;
        while (!endTurn)
        {
            cout << "\n--- MENU ---" << endl;
            cout << "1. Lanzar dado 🎲" << endl; // Emoji añadido
            cout << "2. Ver mi dinero y propiedades 💰" << endl; // Emoji añadido
            cout << "3. Guardar y Salir" << endl;
            cout << "4. Salir sin guardar" << endl;
            int choice = getValidInput(1, 4);

            if (choice == 1)
            {
                endTurn = true;
            }
            else if (choice == 2)
            {
                printPlayerStatus(currentPlayer);
                cout << "Propiedades:" << endl;
                int propCount = countPlayerProperties(currentPlayer);
                if (propCount > 0)
                {
                    for (int i = 0; i < BOARD_PERIMETER; ++i)
                    {
                        if (currentPlayer.properties[i])
                        {
                            cout << " - " << PROPERTY_NAMES[i] << endl;
                        }
                    }
                }
                else
                {
                    cout << " - (Ninguna)" << endl;
                }
            }
            else if (choice == 3)
            {
                saveGame(gameState);
                return 0;
            }
            else if (choice == 4)
            {
                cout << "Saliendo sin guardar. ¡Adios!" << endl;
                return 0;
            }
        }

        // Lógica de acción del turno
        if (currentPlayer.turnsInJail > 0)
        {
            gameState = handleJailTurn(gameState);
        }
        else
        {
            gameState.players[gameState.currentPlayerIndex] = movePlayer(currentPlayer, rollDice());
            currentPlayer = gameState.players[gameState.currentPlayerIndex];
            char tileType = BOARD_LAYOUT[currentPlayer.position];
            
            if (tileType == T_PROPERTY) { gameState = handleProperty(gameState); }
            else if (tileType == T_CARD) { gameState = handleSpecialCard(gameState); }
            else if (tileType == T_TAX) { gameState = handleTax(gameState); }
            else if (tileType == T_GO_TO_JAIL) { gameState = handleGoToJail(gameState); }
            else if (tileType == T_JAIL) { cout << "Estas de visita en la carcel." << endl; }
            else if (tileType == T_PARKING) { cout << "Estacionamiento libre. No pasa nada." << endl; }
            else if (tileType == T_START) { /* La acción ya se maneja en movePlayer */ }
        }

        gameState = checkGameOver(gameState);
        
        if (!gameState.isGameOver)
        {
            gameState.currentPlayerIndex = (gameState.currentPlayerIndex + 1) % NUM_PLAYERS;
        }
    }

    showWinner(gameState);
    return 0;
}

// --- Implementación de Funciones Auxiliares ---

int getValidInput(int min, int max)
{
    int choice;
    do
    {
        cout << "Elige una opcion: ";
        cin >> choice;
        if (cin.fail())
        {
            cout << "Error: Por favor, ingresa solo numeros." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        else if (choice < min || choice > max)
        {
            cout << "Error: Opcion fuera de rango. Intenta de nuevo." << endl;
        }
    } while (choice < min || choice > max);
    return choice;
}

GameState checkGameOver(GameState gs)
{
    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        if (gs.players[i].money < BANKRUPTCY_LIMIT)
        {
            gs.players[i].isBankrupt = true;
            gs.isGameOver = true;
            cout << "!" << gs.players[i].name << " ha entrado en bancarrota! 📉" << endl; // Emoji añadido
            return gs;
        }
    }
    int propertiesOwned = 0;
    for (int i = 0; i < BOARD_PERIMETER; ++i)
    {
        if (BOARD_LAYOUT[i] == T_PROPERTY && getPropertyOwnerIndex(gs, i) != -1)
        {
            propertiesOwned++;
        }
    }
    if (propertiesOwned >= MAX_PROPERTIES)
    {
        gs.isGameOver = true;
        cout << "!Todas las propiedades han sido compradas!" << endl;
    }
    return gs;
}

void showWinner(GameState gs)
{
    Player winner;
    if (gs.players[0].isBankrupt) { winner = gs.players[1]; }
    else if (gs.players[1].isBankrupt) { winner = gs.players[0]; }
    else
    {
        int propsP1 = countPlayerProperties(gs.players[0]);
        int propsP2 = countPlayerProperties(gs.players[1]);
        if (propsP1 > propsP2) { winner = gs.players[0]; }
        else if (propsP2 > propsP1) { winner = gs.players[1]; }
        else
        {
            if (gs.players[0].money > gs.players[1].money) { winner = gs.players[0]; }
            else { winner = gs.players[1]; }
        }
    }
    cout << "\n==================== FIN DEL JUEGO ====================" << endl;
    cout << "!!! El ganador es " << winner.name << " 🏆 !!!" << endl; // Emoji añadido
    cout << "=======================================================" << endl;
}