// MAIN/main.cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>       // Para numeric_limits
#include <string>
#include <cctype>       // Para tolower
#include <cstdio>       // Necesario para la función remove() y errno

// Asegúrate de que las inclusiones estén en este orden y con las rutas correctas (../src/)
#include "../src/Constants.h"
#include "../src/Player.h"
#include "../src/Board.h"
#include "../src/GameIO.h" // Aquí es donde se declaran checkSavedGame, loadGameData, saveGame

// Incluimos <cerrno> para usar errno y perror de forma más robusta con remove()
#include <cerrno> // Para manejar errores específicos de remove()

using namespace std;

// --- Función auxiliar para eliminar archivos de guardado ---
// NOTA: Esta función asume que los archivos .txt están en el mismo directorio que el ejecutable.
// Si tu ejecutable se crea en PROYECTO_RAIZ, los .txt deben estar allí.
void deleteSaveFiles() {
    int status_players = remove("players.txt");
    int status_board = remove("board_properties.txt");

    if (status_players == 0) {
        cout << "Archivo players.txt eliminado correctamente." << endl;
    } else if (status_players != 0 && errno == ENOENT) { // ENOENT = No such file or directory
        cout << "players.txt no existe (no se pudo eliminar)." << endl;
    } else {
        // En caso de otro tipo de error al eliminar
        perror("Error al eliminar players.txt");
    }

    if (status_board == 0) {
        cout << "Archivo board_properties.txt eliminado correctamente." << endl;
    } else if (status_board != 0 && errno == ENOENT) {
        cout << "board_properties.txt no existe (no se pudo eliminar)." << endl;
    } else {
        perror("Error al eliminar board_properties.txt");
    }
}


int main() {
    srand(time(0)); // Semilla para dados

    cout << "🎲 BIENVENIDO A MINI MONOPOLY 🎲" << endl;

    // --- Lógica de Inicio: Nueva Partida vs. Cargar Partida ---
    bool saved_game_exists = checkSavedGame(); // Verifica si hay archivos de guardado

    string start_choice;
    if (saved_game_exists) {
        cout << "\nSe encontró una partida guardada. ¿Qué deseas hacer?" << endl;
        cout << "1. Continuar partida guardada" << endl;
        cout << "2. Empezar nueva partida (eliminará la guardada)" << endl;
        cout << "Elige una opción (1 o 2): ";
        cin >> start_choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
    } else {
        cout << "\nNo se encontraron partidas guardadas. Se iniciará un nuevo juego." << endl;
        start_choice = "2"; // Forzar nueva partida si no hay guardado
    }

    if (start_choice == "1" && saved_game_exists) {
        loadGameData(); // Cargar la partida guardada
        cout << "Partida guardada cargada exitosamente." << endl;
    } else if (start_choice == "2") {
        deleteSaveFiles(); // Eliminar cualquier partida guardada si se elige nueva partida
        createPlayers(Players, MAX_PLAYERS); // Crear nuevos jugadores
        cout << "Nueva partida iniciada." << endl;
    } else {
        // Opción inválida cuando hay partida guardada
        if (saved_game_exists) {
            cout << "Opción inválida. Se iniciará un nuevo juego por defecto." << endl;
            deleteSaveFiles(); // También borra la guardada si se introdujo una opción errónea
            createPlayers(Players, MAX_PLAYERS);
        } else {
            // Este caso no debería darse, pero como fallback si no hay guardado y no eligió '2'
            createPlayers(Players, MAX_PLAYERS);
        }
    }

    // El tablero base siempre debe cargarse primero.
    // Los owners y estados de las propiedades serán sobrescritos por loadGameData si se cargó un juego.
    loadBoard(board);

    // Mostrar posiciones iniciales
    showBoardWithPlayers(board, Players, MAX_PLAYERS);

    // --- Variables para el control del juego ---
    bool game_over = false;
    int current_player_index = 0; // Empieza con el Jugador 0
    string input; // Para capturar la opción del menú

    // --- Bucle principal del juego ---
    while (!game_over) {
        // Accedemos directamente a Players[current_player_index] (el global)
        // en lugar de crear una referencia local o copia.

        // Declara turn_finished dentro de este bucle para que se reinicie cada turno
        // y esté en el ámbito correcto para toda la lógica de un turno.
        bool turn_finished = false;
        while (!turn_finished) {
            cout << "\n--- TURNO DE " << Players[current_player_index].Name << " ---" << endl;
            cout << "Dinero actual: $" << Players[current_player_index].Cash << endl;
            cout << "Opciones:" << endl;
            cout << "1. Lanzar dado y mover" << endl;
            cout << "2. Ver estado completo del jugador" << endl;
            cout << "3. Ver cartas de 'Salir de la cárcel' (" << Players[current_player_index].numCards << " disponibles)" << endl;
            cout << "4. Salir del juego (sin guardar)" << endl;
            cout << "5. Guardar juego y salir" << endl;

            cout << "Elige una opción: ";
            cin >> input;

            // Limpiar el buffer de entrada para evitar problemas con futuras entradas
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (input == "1") {
                // Lógica para jugadores en la cárcel
                if (Players[current_player_index].inJail) {
                    cout << Players[current_player_index].Name << " está en la cárcel." << endl;
                    Players[current_player_index].turnsInJail++;
                    if (Players[current_player_index].turnsInJail >= 3) {
                        cout << Players[current_player_index].Name << " ha cumplido 3 turnos en la cárcel. Puede pagar $50 para salir." << endl;
                        if (Players[current_player_index].Cash >= 50) {
                            cout << "¿Quieres pagar $50 para salir? (s/n): ";
                            char pay_choice;
                            cin >> pay_choice;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (tolower(pay_choice) == 's') {
                                Players[current_player_index].Cash -= 50;
                                Players[current_player_index].inJail = false;
                                Players[current_player_index].turnsInJail = 0;
                                cout << Players[current_player_index].Name << " pagó $50 y salió de la cárcel." << endl;
                                // Ahora que salió, puede tirar los dados
                                int dice = rollDice();
                                cout << "Sacaste un " << dice << " en el dado." << endl;
                                int oldPosition = Players[current_player_index].Position;
                                Players[current_player_index] = movePlayer(Players[current_player_index], dice); // Actualizar jugador
                                cout << Players[current_player_index].Name << " ahora está en la casilla #" << Players[current_player_index].Position << ": " << findBoxByID(board, Players[current_player_index].Position).Name << endl;
                                if (Players[current_player_index].Position < oldPosition) {
                                     cout << "¡Pasaste por GO! Recibes un bono de $150." << endl;
                                     Players[current_player_index].Cash += 150;
                                }
                                // handleBoxAction devuelve el jugador actualizado
                                Players[current_player_index] = handleBoxAction(Players[current_player_index], board, Players);
                            } else {
                                cout << "No pagaste la fianza. Permaneces en la cárcel." << endl;
                            }
                        } else {
                            cout << "No tienes suficiente dinero para pagar la fianza. Permaneces en la cárcel." << endl;
                        }
                    } else {
                        cout << "Permaneces en la cárcel por " << Players[current_player_index].turnsInJail << " turno(s)." << endl;
                    }
                    turn_finished = true; // El turno termina estando en la cárcel o saliendo
                } else {
                    // Lógica normal de lanzamiento de dado y movimiento
                    int dice = rollDice();
                    cout << "Sacaste un " << dice << " en el dado." << endl;

                    int oldPosition = Players[current_player_index].Position; // Guardamos la posición antigua
                    Players[current_player_index] = movePlayer(Players[current_player_index], dice); // Actualizar jugador
                    cout << Players[current_player_index].Name << " ahora está en la casilla #" << Players[current_player_index].Position << ": " << findBoxByID(board, Players[current_player_index].Position).Name << endl;

                    // Recompensa por pasar por "GO" ($150)
                    if (Players[current_player_index].Position < oldPosition) {
                         cout << "¡Pasaste por GO! Recibes un bono de $150." << endl;
                         Players[current_player_index].Cash += 150;
                    }

                    // Llama a la función de acción de casilla y actualiza el jugador
                    Players[current_player_index] = handleBoxAction(Players[current_player_index], board, Players);
                    turn_finished = true; // El turno termina después de moverse y actuar en la casilla
                }
            } else if (input == "2") {
                showPlayerStatus(Players[current_player_index], board);
            } else if (input == "3") {
                if (Players[current_player_index].numCards > 0) {
                    cout << "Tienes " << Players[current_player_index].numCards << " cartas de 'Salir de la cárcel'." << endl;
                    // Lógica para usar la carta si está en la cárcel
                    if (Players[current_player_index].inJail) {
                        cout << "¿Quieres usar una carta para salir? (s/n): ";
                        char card_choice;
                        cin >> card_choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (tolower(card_choice) == 's') {
                            Players[current_player_index].numCards--;
                            Players[current_player_index].inJail = false;
                            Players[current_player_index].turnsInJail = 0;
                            cout << Players[current_player_index].Name << " usó una carta y salió de la cárcel." << endl;
                        }
                    }
                } else {
                    cout << "No tienes cartas de 'Salir de la cárcel'." << endl;
                }
            } else if (input == "4") {
                cout << "¿Estás seguro de que quieres salir sin guardar? Esto eliminará cualquier progreso guardado. (s/n): ";
                char confirm_exit;
                cin >> confirm_exit;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (tolower(confirm_exit) == 's') {
                    deleteSaveFiles(); // Elimina el guardado al salir sin guardar
                    game_over = true;
                    turn_finished = true; // Termina el turno y el bucle principal
                    cout << "Saliendo del juego." << endl;
                }
            } else if (input == "5") {
                saveGame(); // Guardar el estado actual del juego
                game_over = true;
                turn_finished = true; // Termina el turno y el bucle principal
                cout << "Juego guardado. Saliendo..." << endl;
            }
            else {
                cout << "Opción no válida. Intenta de nuevo." << endl;
            }
        } // Fin del bucle while(!turn_finished)

        // *** Validaciones de Fin de Juego ***
        // 1. Bancarrota:
        // Considera cómo manejar la bancarrota si solo hay un jugador o si quieres que termine el juego.
        if (Players[current_player_index].Cash < -500) {
            cout << "\n¡" << Players[current_player_index].Name << " ha caído en bancarrota y ha sido eliminado del juego!" << endl;
            // Aquí puedes decidir si el juego termina inmediatamente o si el jugador simplemente se retira.
            // Para este ejemplo, haremos que el juego termine si un jugador cae en bancarrota.
            game_over = true;
        }

        // Si el juego no ha terminado por bancarrota o salida manual, actualizamos el tablero y pasamos al siguiente
        if (!game_over) {
            showBoardWithPlayers(board, Players, MAX_PLAYERS);
            // Pasar al siguiente jugador
            current_player_index = (current_player_index + 1) % MAX_PLAYERS;
        }

    } // Fin del bucle while(!game_over)

    cout << "\n--- FIN DEL JUEGO ---" << endl;
    if (game_over) {
        // Puedes añadir lógica aquí para anunciar al ganador si es un juego de 2 jugadores y uno queda en bancarrota
        cout << "¡Gracias por jugar Mini Monopoly!" << endl;
    }

    return 0;
}