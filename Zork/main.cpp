#include "World.h"
#include "Player.h"
#include "GameEnums.h"
#include "Room.h"
#include "NPC.h"
#include "Exit.h"
#include "StatusBar.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

void PrintWelcome();
void PrintHelp();
vector<string> TokenizeInput(const string& input);
bool ProcessCommand(const vector<string>& tokens, Player& player);

int main() {
    // Initialize the game world
    World::InitializeWorld();

    // Create player in the starting room
    Player player("Adventurer", "A brave soul seeking the amulet", World::GetStartingRoom());

    // Game introduction
    PrintWelcome();
    player.getLocation()->look();

    // Main game loop
    string input;
    bool gameRunning = true;
    int darknessTurns = 0;    // Track turns spent in darkness
    bool darknessWarningGiven = false;    // Track if warning has been given

    while (gameRunning) {
        StatusBar::Display(player);
        cout << "\n> ";
        getline(cin, input);

        // Convert input to lowercase and tokenize
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        vector<string> tokens = TokenizeInput(input);

        // Check if the player is in darkness
        bool inDarkness = player.getLocation()->getIsDark() && !player.hasActiveLantern();

        if (!tokens.empty()) {
            // Handle darkness mechanics
            if (inDarkness) {
                // First turn in darkness - give warning and countdown
                if (!darknessWarningGiven) {
                    cout << "\nWARNING: It's pitch black! You sense movement in the darkness.\n";
                    cout << "You have a few seconds to light your lantern before creatures attack!\n";

                    // Visual countdown
                    cout << "3... ";
                    this_thread::sleep_for(chrono::seconds(1));
                    cout << "2... ";
                    this_thread::sleep_for(chrono::seconds(1));
                    cout << "1...\n";
                    this_thread::sleep_for(chrono::seconds(1));

                    darknessWarningGiven = true;
                    darknessTurns++;

                    // Let them execute this command without damage
                    gameRunning = !ProcessCommand(tokens, player);
                }
                // "flee" and "use lantern" commands are safe in darkness
                else if (tokens[0] == "flee" ||
                    (tokens[0] == "use" && tokens.size() > 1 && tokens[1] == "lantern")) {
                    gameRunning = !ProcessCommand(tokens, player);
                }
                // All other commands result in damage after warning
                else {
                    // Apply damage (increases with time spent in darkness)
                    int damage = 10 + (darknessTurns * 5);
                    player.takeDamage(damage);

                    cout << "\nUnseen creatures attack you in the darkness!\n";
                    cout << "You take " << damage << " damage. Health: "
                        << player.getHealth() << "/" << player.getMaxHealth() << endl;

                    // Still process the command
                    gameRunning = !ProcessCommand(tokens, player);
                    darknessTurns++;

                    // Check if player died
                    if (!player.isAlive()) {
                        if (player.getAlignment() < -5) {
                            cout << "\n===== EVIL ENDING =====\n";
                            cout << "Your wickedness led to your demise.\n";
                            cout << "No one mourns your passing.\n";
                        }
                        else {
                            cout << "\n===== GAME OVER =====\n";
                            cout << "Your journey ends here...\n";
                        }
                        gameRunning = false;
                    }
                }
            }
            else {
                // Normal command processing (not in darkness)
                gameRunning = !ProcessCommand(tokens, player);
                darknessWarningGiven = false;
                darknessTurns = 0;
            }

            // Handle lantern duration
            if (player.hasActiveLantern()) {
                player.decrementLanternTurns();
                if (player.getLanternTurnsRemaining() <= 0) {
                    cout << "\nThe lantern's light flickers out...\n";

                    // Only set room to dark if it should be dark naturally
                    if (player.getLocation()->getName() == "Abandoned Mine") {
                        player.getLocation()->setDark(true);
                        cout << "Darkness engulfs you once more!\n";
                    }
                }
                else if (player.getLanternTurnsRemaining() <= 2) {
                    cout << "\nThe lantern's light is growing dim. It will only last "
                        << player.getLanternTurnsRemaining() << " more turns.\n";
                }
            }

            if (!tokens.empty() && tokens[0] != "look" && tokens[0] != "l") {
                player.incrementMoves();
            }
        }
    }

    if (!player.isAlive()) {
        cout << "\n===== GAME OVER =====\n";
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
    cout << "  flee - Escape from dangerous areas (may not always work)\n";
    cout << "\nInventory:\n";
    cout << "  take [item] - Pick up an item\n";
    cout << "  drop [item] - Drop an item\n";
    cout << "  inventory/i - Check your inventory\n";
    cout << "  examine/x [item] - Look closely at an item to see details\n";
    cout << "  use [item] - Use an item (e.g., keys on doors, lantern for light)\n";
    cout << "  combine amulet - Combine fragments at the temple altar\n";
    cout << "  place amulet - Place completed amulet on tower altar\n";
    cout << "\nNPC Interaction:\n";
    cout << "  talk [npc] - Talk to an NPC\n";
    cout << "  attack [npc] - Attack an NPC (may affect your alignment)\n";
    cout << "  forgive [npc] - Show mercy to an NPC (improves alignment)\n";
    cout << "  sacrifice [npc] - Sacrifice an NPC for power (dark path)\n";
    cout << "\nMoral Choices:\n";
    cout << "  corrupt [item] - Corrupt an item for power (dark path)\n";
    cout << "  alignment - Check your current moral standing\n";
    cout << "  steal [item] - Steal an item (affects alignment)\n";
    cout << "\nOther:\n";
    cout << "  help - Show this help\n";
    cout << "  quit - Exit the game\n";
}

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

bool ProcessCommand(const vector<string>& tokens, Player& player) {
    string command = tokens[0];
    bool inDarkness = player.getLocation()->getIsDark() && !player.hasActiveLantern();

    // Movement commands
    if (command == "go") {
        if (tokens.size() < 2) {
            cout << "Go where? (north, south, east, west, up, down)\n";
            return false;
        }

        Direction dir;
        if (tokens[1] == "north") dir = Direction::NORTH;
        else if (tokens[1] == "south") dir = Direction::SOUTH;
        else if (tokens[1] == "east") dir = Direction::EAST;
        else if (tokens[1] == "west") dir = Direction::WEST;
        else if (tokens[1] == "up") dir = Direction::UP;
        else if (tokens[1] == "down") dir = Direction::DOWN;
        else {
            cout << "Invalid direction. Use north, south, east, west, up, or down.\n";
            return false;
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
        if (inDarkness) {
            cout << "It's too dark to see anything. You need a light source.\n";
        }
        else {
            player.getLocation()->look();
        }
    }
    // Inventory management
    else if (command == "take") {
        if (tokens.size() < 2) {
            cout << "Take what?\n";
            return false;
        }

        // Prevent taking items in darkness
        if (inDarkness) {
            cout << "It's too dark to find anything. You need to light your lantern first.\n";
            return false;
        }

        // Combine remaining tokens for item name
        string itemName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) itemName += " ";
            itemName += tokens[i];
        }
        player.takeItem(itemName);
    }
    else if (command == "drop") {
        if (tokens.size() < 2) {
            cout << "Drop what?\n";
            return false;
        }
        string itemName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) itemName += " ";
            itemName += tokens[i];
        }
        player.dropItem(itemName);
    }
    else if (command == "inventory" || command == "i") {
        player.showInventory();
    }
    else if (command == "examine" || command == "x") {
        if (tokens.size() < 2) {
            cout << "Examine what?\n";
            return false;
        }

        if (inDarkness) {
            cout << "It's too dark to examine anything. You need a light source.\n";
            return false;
        }

        string itemName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) itemName += " ";
            itemName += tokens[i];
        }

        // First check inventory
        bool found = false;
        for (auto item : player.getInventory()) {
            if (item->nameMatches(itemName)) {
                item->look();
                found = true;
                break;
            }
        }

        // Then check room if not found in inventory
        if (!found) {
            Entity* entity = player.getLocation()->findEntity(itemName);
            if (entity) {
                entity->look();
            }
            else {
                cout << "You don't see '" << itemName << "' here or in your inventory.\n";
            }
        }
    }
    // Item interaction
    else if (command == "use") {
        if (tokens.size() < 2) {
            cout << "Use what?\n";
            return false;
        }
        string itemName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) itemName += " ";
            itemName += tokens[i];
        }
        player.useItem(itemName);
    }
    // Amulet combination
    else if (command == "combine") {
        if (tokens.size() > 1 && tokens[1] == "amulet") {
            if (inDarkness) {
                cout << "It's too dark to work with the amulet fragments. You need light.\n";
                return false;
            }
            player.combineAmuletFragments();
        }
        else {
            cout << "Combine what? Try 'combine amulet'\n";
        }
    }
    // Amulet placement (win condition)
    else if (command == "place") {
        if (tokens.size() > 1 && tokens[1] == "amulet") {
            if (inDarkness) {
                cout << "It's too dark to find the altar. You need light.\n";
                return false;
            }
            if (player.placeAmuletOnAltar()) {
                return true; // Signal game win
            }
        }
        else {
            cout << "Place what? Try 'place amulet'\n";
        }
    }
    // NPC interaction
    else if (command == "talk") {
        if (tokens.size() < 2) {
            cout << "Talk to whom?\n";
            return false;
        }

        if (inDarkness) {
            cout << "You can't see anyone to talk to in this darkness.\n";
            return false;
        }

        string npcName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) npcName += " ";
            npcName += tokens[i];
        }
        Entity* entity = player.getLocation()->findEntity(npcName);
        if (entity && entity->getType() == EntityType::NPC) {
            NPC* npc = dynamic_cast<NPC*>(entity);
            // Check if NPC has interacted and prevents reinteraction
            if (npc->hasPlayerInteracted()) {
                cout << npc->getName() << " has nothing more to say to you." << endl;
            }
            else {
                npc->interact(&player);
            }
        }
        else {
            cout << "There's no " << npcName << " here to talk to." << endl;
        }
    }
    else if (command == "flee") {
        Room* currentRoom = player.getLocation();

        // Determine if fleeing is possible and where to
        if (currentRoom->getName() == "Abandoned Mine") {
            // Automatically move up from the mine
            player.moveTo(Direction::UP);
            cout << "You flee to safety, heart pounding!\n";
        }
        else {
            // Try to find a valid exit
            bool fled = false;
            for (auto dirPair : { make_pair(Direction::NORTH, "north"),
                                make_pair(Direction::SOUTH, "south"),
                                make_pair(Direction::EAST, "east"),
                                make_pair(Direction::WEST, "west"),
                                make_pair(Direction::UP, "up"),
                                make_pair(Direction::DOWN, "down") }) {

                Entity* exitEntity = currentRoom->getExit(dirPair.first);
                if (exitEntity && exitEntity->getType() == EntityType::EXIT) {
                    Exit* exit = dynamic_cast<Exit*>(exitEntity);
                    if (exit && !exit->isLocked()) {
                        player.moveTo(dirPair.first);
                        cout << "You flee " << dirPair.second << " in a panic!\n";
                        fled = true;
                        break;
                    }
                }
            }

            if (!fled) {
                cout << "There's nowhere to flee to!\n";
            }
        }
    }
    else if (command == "sacrifice") {
        if (tokens.size() < 2) {
            cout << "Sacrifice whom?\n";
            return false;
        }
        string npcName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) npcName += " ";
            npcName += tokens[i];
        }

        // Major negative alignment action
        player.sacrificeNPC(npcName);
    }
    else if (command == "forgive") {
        if (tokens.size() < 2) {
            cout << "Forgive whom?\n";
            return false;
        }
        string enemyName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) enemyName += " ";
            enemyName += tokens[i];
        }

        // Major positive alignment action
        player.forgiveEnemy(enemyName);
    }
    else if (command == "corrupt") {
        if (tokens.size() < 2) {
            cout << "Corrupt what?\n";
            return false;
        }
        string artifactName;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) artifactName += " ";
            artifactName += tokens[i];
        }

        // Major negative alignment action
        player.corruptArtifact(artifactName);
    }
    else if (command == "alignment" || command == "karma") {
        player.showAlignment();
    }
    // Help system
    else if (command == "help") {
        PrintHelp();
    }
    // Quit command
    else if (command == "quit" || command == "exit") {
        cout << "Goodbye, brave adventurer!\n";
        return true;
    }
    else if (command == "attack") {
        if (tokens.size() < 2) {
            cout << "Attack what?\n";
            return false;
        }
        string target;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) target += " ";
            target += tokens[i];
        }
        player.attackCreature(target);
    }
    else if (command == "steal") {
        if (tokens.size() < 2) {
            cout << "Steal what?\n";
            return false;
        }
        string item;
        for (size_t i = 1; i < tokens.size(); i++) {
            if (i > 1) item += " ";
            item += tokens[i];
        }
        player.stealItem(item);
    }
    // Unknown command
    else {
        cout << "I don't understand '" << command << "'.\n";
        cout << "Type 'help' for available commands.\n";
    }

    return false;
}