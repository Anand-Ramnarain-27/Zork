#include "World.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void PrintWelcome();  
void PrintHelp();     
vector<string> TokenizeInput(const string& input);  // Splits input into words
void ProcessCommand(const vector<string>& tokens, Player& player);  // Handles commands

int main() {
    // Set up game world and player
    World::InitializeWorld();
    Player player("Adventurer", "A brave soul seeking the amulet", World::GetStartingRoom());

    PrintWelcome();
    player.getLocation()->look();

    // Main game loop
    string input;
    while (true) {
        cout << "\n> ";
        getline(cin, input);

        // Normalize and parse input
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        vector<string> tokens = TokenizeInput(input);

        if (tokens.empty()) continue;

        if (tokens[0] == "quit" || tokens[0] == "exit") {
            cout << "Goodbye!\n";
            break;
        }
        ProcessCommand(tokens, player);
    }
    return 0;
}

void PrintWelcome() {
    cout << "========================================\n";
    cout << "  THE LOST AMULET OF ELDORIA\n";
    cout << "========================================\n";
    cout << "An ancient curse plagues the kingdom.\n";
    cout << "Recover the amulet fragments to lift it!\n";
    cout << "Type 'help' for commands.\n";
    cout << "========================================\n\n";
}

void PrintHelp() {
    cout << "\nCommands:\n";
    cout << "look - View current location\n";
    cout << "go [direction] - Move (north/south/east/west/up/down)\n";
    cout << "take/drop [item] - Manage inventory\n";
    cout << "inventory - View carried items\n";
    cout << "use [item] - Use an item\n";
    cout << "combine [item1] [item2] - Merge items\n";
    cout << "help - Show this list\n";
    cout << "quit - Exit game\n";
}

// Splits input string into words
vector<string> TokenizeInput(const string& input) {
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

// Handles all game commands
void ProcessCommand(const vector<string>& tokens, Player& player) {
    string command = tokens[0];

    if (command == "help") {
        PrintHelp();
    }
    else if (command == "look" || command == "l") {
        player.getLocation()->look();
    }
    else if (command == "go") {
        if (tokens.size() < 2) {
            cout << "Specify direction (north/south/east/west/up/down)\n";
            return;
        }

        // Convert input to direction
        Direction dir;
        string dirStr = tokens[1];
        if (dirStr == "north") dir = Direction::NORTH;
        else if (dirStr == "south") dir = Direction::SOUTH;
        else if (dirStr == "east") dir = Direction::EAST;
        else if (dirStr == "west") dir = Direction::WEST;
        else if (dirStr == "up") dir = Direction::UP;
        else if (dirStr == "down") dir = Direction::DOWN;
        else {
            cout << "Invalid direction\n";
            return;
        }

        player.move(dir);
    }
    else if (command == "take") {
        if (tokens.size() < 2) {
            cout << "Specify item to take\n";
            return;
        }
        player.takeItem(tokens[1]);
    }
    else if (command == "drop") {
        if (tokens.size() < 2) {
            cout << "Specify item to drop\n";
            return;
        }
        player.dropItem(tokens[1]);
    }
    else if (command == "inventory" || command == "i") {
        player.showInventory();
    }
    else if (command == "use") {
        if (tokens.size() < 2) {
            cout << "Specify item to use\n";
            return;
        }
        player.useItem(tokens[1]);
    }
    else if (command == "combine") {
        if (tokens.size() < 3) {
            cout << "Specify two items to combine\n";
            return;
        }
        player.combineItems(tokens[1], tokens[2]);
    }
    else {
        cout << "Unknown command. Type 'help' for options.\n";
    }
}