// src/Board.cpp
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype> // Para tolower
#include <limits> // Para numeric_limits<streamsize>::max()

#include "Player.h" // Incluimos Player.h para la definición completa de Player
#include "Board.h" // Incluimos Board.h para la definición de Box y los prototipos
#include "Constants.h" // Incluye Constants.h para MAX_PLAYERS, SIZE, TILES

// No necesitamos 'extern' aquí para Players o board porque ya se definen
// en GameIO.cpp. Simplemente podemos acceder a ellos si fueran globales,
// pero dado que los estamos pasando como parámetros, no hay problema.
// El arreglo global 'Players' y 'board' se accederá a través de los parámetros
// que, como arrays, se comportan como punteros al original.

using namespace std;


// Devuelve una casilla según su ID
Box asignBox(int id)
{
    Box box;

    string names[TILES] = {
        "GO", "Prop 1", "Card", "Prop 2", "Tax",
        "Prop 3", "Card", "Prop 4", "Prop 5", "Jail",
        "Parking", "Prop 6", "Tax", "Prop 7", "Prop 8",
        "Go to Jail", "Prop 9", "Card", "Prop 10", "Prop 11"};

    int types[TILES] = {
        TYPE_GO, TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_TAX,
        TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_PROPERTY, TYPE_JAIL,
        TYPE_PARKING, TYPE_TAX, TYPE_PROPERTY, TYPE_PROPERTY, TYPE_GOTOJAIL,
        TYPE_PROPERTY, TYPE_CARD, TYPE_PROPERTY, TYPE_PROPERTY, TYPE_PARKING}; // Corregido: antes había un error de índice con TYPE_TAX

    int prices[TILES] = {
        0, 100, 0, 120, 0,
        150, 0, 180, 200, 0,
        0, 220, 0, 240, 260,
        0, 280, 0, 300, 320};

    int rents[TILES] = {
        0, 10, 0, 12, 50,
        14, 0, 16, 18, 0,
        0, 20, 50, 22, 24,
        0, 26, 0, 28, 30};

    box.Name = names[id];
    box.Type = types[id];
    box.Price = prices[id];
    box.Rent = rents[id];
    box.Owner = -1;
    box.Houses = 0;
    box.Hotel = false;
    box.ID = id;

    return box;
}

// loadBoard modifica el tablero global (pasado como array, que es un puntero)
void loadBoard(Box board[SIZE][SIZE])
{
    // Primero, reinicializa todas las casillas a un estado vacío (para el caso de no carga de archivo)
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            board[r][c] = {"", -1, 0, 0, -1, 0, false, -1};
        }
    }

    int id = 0;

    // Fila inferior (derecha → izquierda)
    for (int col = SIZE - 1; col >= 0 && id < TILES; col--)
    {
        board[SIZE - 1][col] = asignBox(id);
        id++;
    }

    // Columna izquierda (abajo → arriba, sin repetir esquina)
    for (int row = SIZE - 2; row >= 0 && id < TILES; row--)
    {
        board[row][0] = asignBox(id);
        id++;
    }

    // Fila superior (izquierda → derecha, sin repetir esquina)
    for (int col = 1; col < SIZE && id < TILES; col++)
    {
        board[0][col] = asignBox(id);
        id++;
    }

    // Columna derecha (arriba → abajo, sin repetir esquina)
    for (int row = 1; row < SIZE - 1 && id < TILES; row++)
    {
        board[row][SIZE - 1] = asignBox(id);
        id++;
    }
    // NOTA: Después de esta función, si loadGame se llamó antes y encontró
    // board_properties.txt, los Owners de las propiedades se sobrescribirán
    // con los valores cargados.
}

char symbol(Box b)
{
    switch (b.Type)
    {
    case TYPE_GO:
        return 'S';
    case TYPE_PROPERTY:
        return 'P';
    case TYPE_CARD:
        return 'C';
    case TYPE_TAX:
        return '$';
    case TYPE_JAIL:
        return 'J';
    case TYPE_GOTOJAIL:
        return 'G';
    case TYPE_PARKING:
        return 'E';
    default:
        return ' ';
    }
}

void showBoard(Box board[SIZE][SIZE])
{
    cout << "\n=== TABLERO ===\n";
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            cout << "[" << symbol(board[r][c]) << "]";
        }
        cout << "\n";
    }
    cout << "===============\n";
}

int rollDice ()
{
    return rand() % 6 + 1;
}

Player movePlayer (Player player, int Steps)
{
    player.Position = (player.Position + Steps) % TILES;
    return player;
}

void positionToCoords(int position, int coords[2])
{
    if (position < 6)
    {
        coords[0] = SIZE - 1;
        coords[1] = (SIZE - 1) - position;
    }
    else if (position < 11)
    {
        coords[0] = (SIZE - 1) - (position - 5);
        coords[1] = 0;
    }
    else if (position < 16)
    {
        coords[0] = 0;
        coords[1] = (position - 10);
    }
    else
    {
        coords[0] = (position - 15);
        coords[1] = SIZE - 1;
    }
}

Box findBoxByID(Box board[SIZE][SIZE], int id)
{
    int coords[2];
    positionToCoords(id, coords);

    if (coords[0] >= 0 && coords[0] < SIZE && coords[1] >= 0 && coords[1] < SIZE) {
        return board[coords[0]][coords[1]]; // Devuelve una COPIA de la casilla
    } else {
        // En caso de ID inválido, devuelve la casilla GO como fallback.
        // Asumiendo que GO está en la última posición asignada.
        return board[SIZE-1][SIZE-1];
    }
}

void showBoardWithPlayers(Box board[SIZE][SIZE], Player players[], int numPlayers)
{
    char display[SIZE][SIZE];

    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            display[r][c] = symbol(board[r][c]);
        }
    }

    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i].Cash > -500) {
            int coords[2];
            positionToCoords(players[i].Position, coords);
            display[coords[0]][coords[1]] = ('1' + i);
        }
    }

    cout << "\n=== TABLERO CON JUGADORES ===\n";
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            cout << "[" << display[r][c] << "]";
        }
        cout << "\n";
    }
    cout << "=============================\n";
}

// --- Acciones de Casilla ---
// Recibe Player por valor (se devolverá modificado)
// Recibe Board por "valor" (es un array, así que es un puntero al original, lo que permite modificarlo)
// Recibe Players[] por "valor" (es un array, así que es un puntero al original, lo que permite modificarlo)
Player handleBoxAction(Player player, Box board[SIZE][SIZE], Player Players[MAX_PLAYERS]) {
    int coords[2];
    positionToCoords(player.Position, coords);

    // Accedemos directamente a la casilla original en el tablero global a través del puntero
    // implícito que C++ usa al pasar el array 'board'.
    // Importante: No uses 'currentBox' para modificar la casilla, usa 'board[coords[0]][coords[1]]' directamente.
    // currentBoxData es una COPIA de la casilla, solo para leer sus propiedades.
    Box currentBoxData = board[coords[0]][coords[1]];

    cout << "\n" << player.Name << " aterrizo en " << currentBoxData.Name << "." << endl;

    switch (currentBoxData.Type) { // Usamos currentBoxData para el tipo, nombre, etc.
        case TYPE_GO:
            cout << "¡Es la casilla de SALIDA! No pasa nada adicional aqui (el bono por pasar por GO ya se manejó)." << endl;
            break;

        case TYPE_PROPERTY:
            if (currentBoxData.Owner == -1) { // La propiedad no tiene dueño
                cout << "Esta propiedad (" << currentBoxData.Name << ") cuesta $" << currentBoxData.Price << " y su renta es $" << currentBoxData.Rent << "." << endl;
                cout << "Quieres comprarla, " << player.Name << "? (s/n): ";
                char choice;
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (tolower(choice) == 's') {
                    if (player.Cash >= currentBoxData.Price) {
                        player.Cash -= currentBoxData.Price;
                        // ¡Aquí modificamos el Owner directamente en el tablero global!
                        board[coords[0]][coords[1]].Owner = player.ID;
                        player.Properties[player.totalProperties] = currentBoxData.ID;
                        player.totalProperties++;
                        cout << player.Name << " compro " << currentBoxData.Name << "." << endl;
                        cout << "Dinero restante: $" << player.Cash << endl;
                    } else {
                        cout << "No tienes suficiente dinero para comprar " << currentBoxData.Name << "." << endl;
                    }
                } else {
                    cout << "No compraste " << currentBoxData.Name << "." << endl;
                }
            } else if (currentBoxData.Owner != player.ID) { // La propiedad tiene dueño y no es este jugador
                cout << "Esta propiedad pertenece a " << Players[currentBoxData.Owner].Name << ". Debes pagar $" << currentBoxData.Rent << " de alquiler." << endl;
                player.Cash -= currentBoxData.Rent;
                // ¡Aquí modificamos el dinero del dueño directamente en el array Players global!
                Players[currentBoxData.Owner].Cash += currentBoxData.Rent;
                cout << player.Name << " pago $" << currentBoxData.Rent << " de alquiler." << endl;
                cout << "Dinero restante: $" << player.Cash << endl;
                cout << Players[currentBoxData.Owner].Name << " recibio $" << currentBoxData.Rent << "." << endl;

            } else { // El jugador es el dueño
                cout << "Eres el dueño de " << currentBoxData.Name << ". Estas a salvo." << endl;
            }
            break;

        case TYPE_CARD: { // <--- Inicia un nuevo bloque de alcance aquí (¡corregido!)
            int card_effect = rand() % 2; // 0 = Gana dinero, 1 = Pierde dinero
            if (card_effect == 0) {
                int amount = 50 + (rand() % 100); // Gana entre $50 y $150
                cout << "Carta: Ganaste $" << amount << "!" << endl;
                player.Cash += amount;
            } else {
                int amount = 30 + (rand() % 80); // Pierde entre $30 y $110
                cout << "Carta: Perdiste $" << amount << "!" << endl;
                player.Cash -= amount;
            }
            if (player.numCards < MAX_CARDS && rand() % 3 == 0) { // 1/3 de probabilidad de obtener una carta de salir de la cárcel
                cout << "Has obtenido una carta de 'Salir de la cárcel'!" << endl;
                player.Cards[player.numCards] = 0; // 0 podría ser el ID para carta de salir de la cárcel
                player.numCards++;
            }
            break;
        } // <--- Finaliza el bloque de alcance aquí

        case TYPE_TAX:
            cout << "Caiste en una casilla de Impuestos. Pagas $" << currentBoxData.Rent << "." << endl;
            player.Cash -= currentBoxData.Rent;
            cout << "Dinero restante: $" << player.Cash << endl;
            break;

        case TYPE_JAIL:
            cout << "Estas de visita en la Cárcel. ¡Solo estas pasando!" << endl;
            break;

        case TYPE_GOTOJAIL:
            cout << "¡Fuiste a la Carcel! Directo a la casilla de Cárcel." << endl;
            player.Position = 9; // Posición de la cárcel (ajusta según tu tablero si es diferente)
            player.inJail = true;
            player.turnsInJail = 0; // Se resetearía al entrar
            break;

        case TYPE_PARKING:
            cout << "Estacionamiento Gratuito. ¡Relajate un turno!" << endl;
            break;

        default:
            cout << "Casilla desconocida. No pasa nada." << endl;
            break;
    }
    return player; // Devuelve el jugador modificado
}