#include "World.h"
#include "Player.h"
#include "GameEnums.h"
#include "Room.h"
#include "NPC.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

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
    bool gameRunning = true;
    while (gameRunning) {
        cout << "\n> ";
        getline(cin, input);

        // Convert input to lowercase and tokenize
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        vector<string> tokens = TokenizeInput(input);

        if (tokens.empty()) continue;

        // Check for quit command
        if (tokens[0] == "quit" || tokens[0] == "exit") {
            cout << "Goodbye, brave adventurer!\n";
            break;
        }

        // Process the command
        ProcessCommand(tokens, player);

        // Check win condition
        if (player.hasItem("restored amulet") &&
            player.getLocation()->getName() == "Sorcerer's Tower") {
            cout << "\n\n*** You place the restored amulet on the altar! ***\n";
            cout << "*** The curse is lifted and Eldoria is saved! ***\n";
            cout << "*** CONGRATULATIONS! YOU WIN! ***\n\n";
            gameRunning = false;
        }
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
    cout << "\nAvailable commands:\n";
    cout << "------------------\n";
    cout << "Movement:\n";
    cout << "  go [direction] - Move in specified direction\n";
    cout << "  north/n, south/s, east/e, west/w, up/u, down/d - Quick movement\n";
    cout << "  look/l - Look around the current room\n";
    cout << "\nInventory:\n";
    cout << "  take [item] - Pick up an item\n";
    cout << "  drop [item] - Drop an item\n";
    cout << "  inventory/i - Check your inventory\n";
    cout << "  use [item] - Use an item (e.g., keys on doors)\n";
    cout << "  combine [item1] [item2] - Combine two items\n";
    cout << "\nNPC Interaction:\n";
    cout << "  talk [npc] - Talk to an NPC\n";
    cout << "  talk [npc] [topic] - Ask about specific topic\n";
    cout << "\nOther:\n";
    cout << "  help - Show this help\n";
    cout << "  quit - Exit the game\n";
    cout << "\nInventory Rules:\n";
    cout << "  - Without backpack: Can carry up to 3 small items\n";
    cout << "  - With backpack: Can carry more items\n";
    cout << "  - Some items must be given to NPCs (use 'give' command)\n";
}

// Splits input string into words
vector<string> TokenizeInput(const string& input) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;

    for (char c : input) {
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if ((isspace(c) && !inQuotes)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

// Handles all game commands
void ProcessCommand(const vector<string>& tokens, Player& player) {
    string command = tokens[0];

    // Movement commands
    if (command == "go") {
        if (tokens.size() < 2) {
            cout << "Go where? (north, south, east, west, up, down)\n";
            return;
        }

        // Convert direction string to enum
        Direction dir;
        if (tokens[1] == "north") dir = Direction::NORTH;
        else if (tokens[1] == "south") dir = Direction::SOUTH;
        else if (tokens[1] == "east") dir = Direction::EAST;
        else if (tokens[1] == "west") dir = Direction::WEST;
        else if (tokens[1] == "up") dir = Direction::UP;
        else if (tokens[1] == "down") dir = Direction::DOWN;
        else {
            cout << "Invalid direction. Use north, south, east, west, up, or down.\n";
            return;
        }
        player.moveTo(dir);
    }
    // Quick movement aliases
    else if (command == "north" || command == "n") {
        player.moveTo(Direction::NORTH);
    }
    else if (command == "south" || command == "s") {
        player.moveTo(Direction::SOUTH);
    }
    else if (command == "east" || command == "e") {
        player.moveTo(Direction::EAST);
    }
    else if (command == "west" || command == "w") {
        player.moveTo(Direction::WEST);
    }
    else if (command == "up" || command == "u") {
        player.moveTo(Direction::UP);
    }
    else if (command == "down" || command == "d") {
        player.moveTo(Direction::DOWN);
    }
    // Observation
    else if (command == "look" || command == "l") {
        player.getLocation()->look();
    }
    // Inventory management
    else if (command == "take") {
        if (tokens.size() < 2) {
            cout << "Take what?\n";
            return;
        }
        player.takeItem(tokens[1]);
    }
    else if (command == "drop") {
        if (tokens.size() < 2) {
            cout << "Drop what?\n";
            return;
        }
        player.dropItem(tokens[1]);
    }
    else if (command == "inventory" || command == "i") {
        player.showInventory();
    }
    // Item interaction
    else if (command == "use") {
        if (tokens.size() < 2) {
            cout << "Use what?" << endl;
            return;
        }

        // Combine remaining tokens for item names with spaces
        string itemName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) itemName += " ";
            itemName += tokens[i];
        }

        player.useItem(itemName);
    }
    else if (command == "combine") {
        if (tokens.size() < 3) {
            cout << "Combine what with what?\n";
            return;
        }
        player.combineItems(tokens[1], tokens[2]);
    }
    else if (command == "give") {
        if (tokens.size() < 3) {
            cout << "Give what to whom? (e.g., 'give bread blacksmith')" << endl;
            return;
        }

        string itemName = tokens[1];
        string npcName = tokens[2];

        // Find NPC
        Entity* entity = player.getLocation()->findEntity(npcName);
        if (entity && entity->getType() == EntityType::NPC) {
            NPC* npc = dynamic_cast<NPC*>(entity);
            if (npc) {
                if (player.hasItem(itemName)) {
                    npc->interact(&player); // This will handle the giving
                }
                else {
                    cout << "You don't have " << itemName << " to give." << endl;
                }
            }
        }
        else {
            cout << "There's no " << npcName << " here to give items to." << endl;
        }
    }
    // NPC interaction
    else if (command == "talk") {
        if (tokens.size() < 2) {
            cout << "Talk to whom?\n";
            return;
        }

        // Find the NPC in the current room
        Entity* entity = nullptr;
        for (auto e : player.getLocation()->getContains()) {
            if (e->getType() == EntityType::NPC && e->nameMatches(tokens[1])) {
                entity = e;
                break;
            }
        }

        if (entity) {
            NPC* npc = dynamic_cast<NPC*>(entity);
            if (npc) {
                if (tokens.size() > 2) {
                    // Handle specific questions
                    string question;
                    for (size_t i = 2; i < tokens.size(); i++) {
                        if (i > 2) question += " ";
                        question += tokens[i];
                    }
                    npc->handlePlayerInput(question, &player);
                }
                else {
                    // Default interaction
                    npc->interact(&player);
                }
            }
        }
        else {
            cout << "There's no " << tokens[1] << " here to talk to." << endl;
        }
        }
    // Help system
    else if (command == "help") {
        PrintHelp();
    }
    // Unknown command
    else {
        cout << "I don't understand '" << command << "'.\n";
        cout << "Type 'help' for available commands.\n";
    }
}