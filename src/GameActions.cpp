#include "GameActions.h"
#include "Constants.h"
#include <iostream>
#include <cstdlib>      // Necesario para rand() y srand()
#include <ctime>        // Necesario para time()
#include <cctype>       // Necesario para tolower()
#include <limits>       // Necesario para numeric_limits

using namespace std;

// ===================================================================
// IMPLEMENTACIÓN DE LAS ACCIONES PRINCIPALES DEL JUEGO
// ===================================================================


// Inicializa el generador de números aleatorios usando la hora actual como semilla.
void initializeRandom() {
    srand(time(nullptr));
}


// Devuelve un número aleatorio entre 1 y 6.
int rollDice() {
    return (rand() % 6) + 1;
}


// Busca el dueño de una propiedad.
int getPropertyOwnerIndex(GameState gs, int propertyPosition) {
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        if (gs.players[i].properties[propertyPosition]) {
            return i;
        }
    }
    return -1;
}


// Gestiona la lógica cuando un jugador cae en una propiedad.
GameState handleProperty(GameState gs) {
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    int propertyPos = currentPlayer.position;
    int ownerIndex = getPropertyOwnerIndex(gs, propertyPos);

    if (ownerIndex == -1) { // Si no tiene dueño
        cout << "La propiedad '" << PROPERTY_NAMES[propertyPos] << "' esta disponible." << endl;
        cout << "Cuesta $" << PROPERTY_PRICES[propertyPos] << ". Tu dinero: $" << currentPlayer.money << endl;
        if (currentPlayer.money >= PROPERTY_PRICES[propertyPos]) {
            char choice;
            // Bucle de validación de entrada.
            do {
                cout << "Deseas comprarla? (s/n): ";
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = tolower(choice);
                if (choice != 's' && choice != 'n') {
                    cout << "Error: Opcion no valida." << endl;
                }
            } while (choice != 's' && choice != 'n');

            if (choice == 's') { // Si compra
                currentPlayer.money -= PROPERTY_PRICES[propertyPos];
                currentPlayer.properties[propertyPos] = true;
                cout << currentPlayer.name << " ha comprado '" << PROPERTY_NAMES[propertyPos] << "'." << endl;
            }
        } else {
            cout << "No tienes suficiente dinero para comprar esta propiedad." << endl;
        }
    } else if (ownerIndex != gs.currentPlayerIndex) { // Si es de otro jugador
        Player owner = gs.players[ownerIndex];
        int rent = PROPERTY_RENTS[propertyPos];
        cout << "Esta propiedad pertenece a " << owner.name << ". Debes pagar una renta de $" << rent << "." << endl;
        currentPlayer.money -= rent;
        owner.money += rent;
        gs.players[ownerIndex] = owner;
    } else { // Si es del propio jugador
        cout << "Has caido en tu propia propiedad. !Que suerte!" << endl;
    }
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}


// Gestiona la lógica de las cartas especiales.
GameState handleSpecialCard(GameState gs) {
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    int effect = rand() % 3; // Efecto aleatorio
    cout << "Has caido en una casilla de Carta Especial!" << endl;
    if (effect == 0) {
        cout << "!Has encontrado un tesoro! Ganas $100." << endl;
        currentPlayer.money += 100;
    } else if (effect == 1) {
        cout << "Paga una multa por exceso de velocidad. Pierdes $50." << endl;
        currentPlayer.money -= 50;
    } else {
        cout << "!Has conseguido una carta para 'Salir de la Carcel'!" << endl;
        currentPlayer.getOutOfJailCards++;
    }
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}


// Envía al jugador a la cárcel.
GameState handleGoToJail(GameState gs) {
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << "!Directo a la carcel! No pasas por la salida y no cobras." << endl;
    currentPlayer.position = JAIL_POSITION;
    currentPlayer.turnsInJail = JAIL_TURNS_TO_SKIP + 1;
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}


// Gestiona el turno de un jugador que está en la cárcel.
GameState handleJailTurn(GameState gs) {
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << currentPlayer.name << " esta en la carcel." << endl;
    if (currentPlayer.getOutOfJailCards > 0) {
        char choice;
        do {
            cout << "Tienes una carta para salir de la carcel. Quieres usarla? (s/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = tolower(choice);
            if (choice != 's' && choice != 'n') cout << "Error: Opcion no valida." << endl;
        } while (choice != 's' && choice != 'n');

        if (choice == 's') {
            currentPlayer.getOutOfJailCards--;
            currentPlayer.turnsInJail = 0;
            cout << "Has usado la carta. !Eres libre!" << endl;
        }
    }
    if (currentPlayer.turnsInJail > 0) {
        currentPlayer.turnsInJail--;
        if (currentPlayer.turnsInJail > 0) {
             cout << "Te quedas en la carcel. Te quedan " << currentPlayer.turnsInJail << " turnos por esperar." << endl;
        } else {
            cout << "Has cumplido tu condena. En el proximo turno podras moverte." << endl;
        }
    }
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}


// Gestiona el pago de impuestos.
GameState handleTax(GameState gs) {
    Player currentPlayer = gs.players[gs.currentPlayerIndex];
    cout << "Impuesto sobre la renta. Debes pagar $" << TAX_AMOUNT << endl;
    currentPlayer.money -= TAX_AMOUNT;
    gs.players[gs.currentPlayerIndex] = currentPlayer;
    return gs;
}