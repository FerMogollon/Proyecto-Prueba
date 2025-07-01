#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
using namespace std;

// ===================================================================
// GLOBAL CONSTANTS FILE
// All fixed game values are defined here for easy access and modification.
// This file acts as the game's main configuration panel.
// ===================================================================

// --- 1. General Game Settings ---
// This section adjusts the game's economy and flow.
// ===================================================================
const int NUM_PLAYERS = 2;          // Number of players in the game.
const int BOARD_PERIMETER = 20;     // Do not change: The board logic is designed for 20 tiles.
const int MAX_PROPERTIES = 11;      // Do not change: The total number of purchasable properties.
const int STARTING_MONEY = 500;     // The amount of money each player starts with.
const int GO_BONUS = 150;           // Bonus received when passing the GO tile.
const int JAIL_POSITION = 5;        // The array index (5) for the Jail tile.
const int GO_TO_JAIL_POSITION = 15; // The array index for the "Go to Jail" tile.
const int BANKRUPTCY_LIMIT = -200;  // The negative money limit to declare bankruptcy.
const int TAX_AMOUNT = 150;         // The cost of the Tax tile.
const int JAIL_TURNS_TO_SKIP = 1;   // Number of turns a player must wait in jail.
const string P1_ICON = "🚗";        // Icon for player 1
const string P2_ICON = "🎩";        // Icon for player 2

// --- 2. Board Logic and Visuals Configuration ---
// Defines the type and appearance of each tile.
// ===================================================================
// Internal logic (do not change). These characters are used for game logic (e.g., in switch statements).
const char T_START = 'S', T_PROPERTY = 'P', T_CARD = 'C', T_TAX = 'I', T_JAIL = 'J', T_GO_TO_JAIL = 'G', T_PARKING = 'E';

// This array defines the logical type of each of the 20 perimeter tiles.
const char BOARD_LAYOUT[BOARD_PERIMETER] = {
    T_START, T_PROPERTY, T_CARD, T_PROPERTY, T_TAX, T_GO_TO_JAIL, T_PROPERTY, T_PROPERTY, T_CARD, T_PROPERTY,
    T_PARKING, T_PROPERTY, T_TAX, T_PROPERTY, T_PROPERTY, T_JAIL, T_CARD, T_PROPERTY, T_PROPERTY, T_PROPERTY};

// This array defines the visual symbol for each tile. Each property set has a distinct emoji.
const string BOARD_SYMBOLS[BOARD_PERIMETER] = {
    "🏁", "🏠", "❔", "🏠", "💲", "🚓", // Set 1: 🏠 //
    "🏢", "🏢", "❔", "🏢",             // Set 2: 🏢
    "🅿️", "🏛️", "💲", "🏛️", "🏛️", "👮",  // Set 3: 🏛️
    "❔", "🏙️", "🏙️", "🏙️"              // Set 4: 🏙️
};

// --- 3. Property Configuration ---
// Parallel arrays: The index of each array corresponds to the same tile on the board.
// ===================================================================
// The names of all properties and tiles on the board.
const string PROPERTY_NAMES[BOARD_PERIMETER] = {
    "GO!", "Mediterranean Avenue", "Surprise Card", "Baltic Avenue", "Income Tax", "Go To Jail",
    "St. Charles Place", "Virginia Avenue", "Surprise Card", "St. James Place", "Free Parking",
    "Tenessee Avenue", "Income Tax", "Kentucky Avenue", "Illinois Avenue",
    "Jail (just visiting)", "Surprise Card", "Oregon Avenue", "Arkansas Avenue", "California Avenue"};

// The purchase price for each property.
const int PROPERTY_PRICES[BOARD_PERIMETER] = {
    0, 60, 0, 60, 0, 0, 100, 100, 0, 120, 0, 140, 0, 140, 160, 0, 0, 180, 180, 200};

// The base rent to be paid for each property.
const int PROPERTY_RENTS[BOARD_PERIMETER] = {
    0, 10, 0, 12, 0, 0, 15, 15, 0, 18, 0, 20, 0, 20, 22, 0, 0, 25, 25, 30};

// --- 4. Property Sets Configuration (Matrix) ---
// Defines the property groups and the bonus for completing them.
// ===================================================================
const int RENT_MULTIPLIER = 4; // Rent is multiplied by this value if a player owns the complete set.
const int NUM_PROPERTY_SETS = 4;
const int MAX_PROPS_PER_SET = 3;

// This matrix defines the property sets. Each row is a set. The numbers are board positions.
// -1 is used as a null/padding value for sets with fewer than 3 properties.
const int PROPERTY_SETS[NUM_PROPERTY_SETS][MAX_PROPS_PER_SET] = {
    {1, 3, -1},   // Set 1: The $60 properties
    {6, 7, 9},    // Set 2: The $100-$120 properties
    {11, 13, 14}, // Set 3: The $140-$160 properties
    {17, 18, 19}  // Set 4: The $180-$200 properties
};

#endif // CONSTANTS_H