#include "Player.h"
#include "Room.h"
#include "Creature.h"
#include "Entity.h"
#include "Exit.h"
#include "GameEnums.h"
#include "Item.h"
#include <iostream>

// Create player in starting room
Player::Player(const string& name, const string& description, Room* room) :
    Creature(EntityType::PLAYER, name, description, room) {
}

// Move to a specific room
bool Player::moveTo(Direction direction) {
    if (location == nullptr) {
        cout << "You are nowhere!" << endl;
        return false;
    }

    // Get the exit in the specified direction
    Entity* exitEntity = location->getExit(direction);
    if (exitEntity == nullptr) {
        cout << "You can't go that way." << endl;
        return false;
    }

    // Check if it's actually an exit
    if (exitEntity->getType() != EntityType::EXIT) {
        cout << "That's not a valid exit." << endl;
        return false;
    }

    Exit* exit = dynamic_cast<Exit*>(exitEntity);
    if (exit == nullptr) {
        cout << "Something strange happened with that exit." << endl;
        return false;
    }

    // Check if exit is locked
    if (exit->isLocked()) {
        cout << "The " << exit->getName() << " is locked. You need a "
            << exit->getKeyName() << " to open it." << endl;
        return false;
    }

    // Move to the destination room
    Room* destination = exit->getDestination();
    if (destination == nullptr) {
        cout << "The exit leads nowhere!" << endl;
        return false;
    }

    cout << "You go " << directionToString(direction) << " to "
        << destination->getName() << "." << endl;

    setLocation(destination);
    destination->look();
    return true;
}

// Take item from current room
bool Player::takeItem(const string& itemName) {
    if (location == nullptr) return false;

    Entity* item = location->findEntity(itemName);
    if (item && item->getType() == EntityType::ITEM) {
        if (canCarryMoreItems()) {
            location->removeEntity(item);
            inventory.push_back(item);
            cout << "You took the " << item->getName() << "." << endl;
            return true;
        }
        else {
            cout << "You can't carry more items right now." << endl;
            return false;
        }
    }

    cout << "You don't see ";
    if (itemName.find(' ') != string::npos) {
        cout << "\"" << itemName << "\"";
    }
    else {
        cout << itemName;
    }
    cout << " here." << endl;

    // Show available items
    cout << "You see: ";
    bool first = true;
    for (auto entity : location->getContains()) {
        if (entity->getType() == EntityType::ITEM) {
            if (!first) cout << ", ";
            cout << entity->getName();
            first = false;
        }
    }
    cout << endl;

    return false;
}

// Drop item to current room
bool Player::dropItem(const string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == itemName) {
            if (location) location->addEntity(*it);
            inventory.erase(it);
            cout << "Dropped " << itemName << endl;
            return true;
        }
    }

    cout << "You don't have " << itemName << endl;
    return false;
}

// Check if item is in inventory
bool Player::hasItem(const string& itemName) const {
    string lowerInput = itemName;
    transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    for (auto item : inventory) {
        string lowerItemName = item->getName();
        transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

        if (lowerItemName == lowerInput) {
            return true;
        }
    }
    return false;
}

// Show all carried items
void Player::showInventory() const {
    if (inventory.empty()) {
        cout << "Inventory empty" << endl;
        return;
    }

    cout << "Carrying:" << endl;
    for (auto item : inventory) {
        cout << "- " << item->getName() << endl;
    }
}

// Adds item to inventory
void Player::addItem(Entity* item) {
    inventory.push_back(item);
}

// Removes item from inventory
bool Player::removeItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == itemName) {
            // Don't delete here - caller will handle ownership
            inventory.erase(it);
            return true;
        }
    }
    return false;
}

// Try to use an item 
bool Player::useItem(const string& itemName) {
    // Find the best matching item in inventory
    Entity* itemToUse = nullptr;
    string lowerInput = itemName;
    transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    // First try exact match
    for (auto item : inventory) {
        string lowerItemName = item->getName();
        transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

        if (lowerItemName == lowerInput) {
            itemToUse = item;
            break;
        }
    }

    // If no exact match, try partial match
    if (!itemToUse) {
        int bestMatchLength = 0;
        for (auto item : inventory) {
            string lowerItemName = item->getName();
            transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

            if (lowerItemName.find(lowerInput) != string::npos) {
                // Prefer the item with the closest name length
                if (itemToUse == nullptr || abs(static_cast<int>(lowerItemName.length()) - static_cast<int>(lowerInput.length()))
                    < bestMatchLength) {
                    itemToUse = item;
                    bestMatchLength = abs(static_cast<int>(lowerItemName.length()) - static_cast<int>(lowerInput.length()));
                }
            }
        }
    }

    // If still no match, show error with suggestions
    if (!itemToUse) {
        cout << "You don't have '" << itemName << "'.";
        if (!inventory.empty()) {
            cout << " You have: ";
            for (auto item : inventory) {
                cout << item->getName();
                if (item != inventory.back()) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
        return false;
    }

    // Check if we're trying to use the item on a locked exit
    Room* currentRoom = getLocation();
    if (currentRoom) {
        // Get all exits from the room
        for (auto exitPair : currentRoom->getExits()) {
            Entity* exitEntity = exitPair.second;
            if (exitEntity->getType() == EntityType::EXIT) {
                Exit* exit = dynamic_cast<Exit*>(exitEntity);
                if (exit && exit->isLocked()) {
                    // Check if this item is the key for this exit
                    if (itemToUse->nameMatches(exit->getKeyName())) {
                        if (exit->unlock(itemToUse->getName())) {
                            cout << "You use the " << itemToUse->getName()
                                << " to unlock the " << exit->getName() << "." << endl;
                            return true;
                        }
                    }
                }
            }
        }
    }

    // Check for other item uses 
    if (itemToUse->getName() == "lantern") {
        cout << "You light the lantern, illuminating the dark area around you." << endl;
        return true;
    }
    else if (itemToUse->getName() == "potion") {
        cout << "You drink the potion and feel rejuvenated." << endl;
        removeItem(itemToUse->getName());
        return true;
    }

    // Default case
    cout << "You're not sure how to use the " << itemToUse->getName() << " here." << endl;
    return false;
}

// Try to combine items 
bool Player::combineItems(const string& item1, const string& item2) {
    cout << "Can't combine " << item1 << " and " << item2 << endl;
    return false; 
}

bool Player::hasBackpack() const {
    for (auto item : inventory) {
        if (item->getName() == "backpack") {
            return true;
        }
    }
    return false;
}

bool Player::canCarryMoreItems() const {
    // If no backpack, can only carry 2 small items
    if (!hasBackpack()) {
        return inventory.size() < 2;
    }
    // With backpack, use its capacity
    return true; 
}

bool Player::hasAmuletFragment(const string& fragmentName) const {
    for (auto item : inventory) {
        if (item->getName() == fragmentName) {
            return true;
        }
    }
    return false;
}

bool Player::combineAmuletFragments() {
    // Check if in correct location (Sorcerer's Tower)
    if (getLocation()->getName() != "Sorcerer's Tower") {
        int fragmentCount = 0;
        if (hasAmuletFragment("amethyst")) fragmentCount++;
        if (hasAmuletFragment("sapphire")) fragmentCount++;
        if (hasAmuletFragment("ruby")) fragmentCount++;

        if (fragmentCount > 0) {
            cout << "\nThe " << (fragmentCount == 1 ? "fragment" : "fragments")
                << " in your inventory pulse weakly but nothing happens.\n";
            cout << "You sense they need to be brought to the Ruined Temple to be combined.\n";

            // Show which fragments player has
            cout << "You have ";
            bool first = true;
            if (hasAmuletFragment("amethyst")) {
                cout << "amethyst";
                first = false;
            }
            if (hasAmuletFragment("sapphire")) {
                if (!first) cout << ", ";
                cout << "sapphire";
                first = false;
            }
            if (hasAmuletFragment("ruby")) {
                if (!first) cout << ", ";
                cout << "ruby";
            }
            cout << " fragment" << (fragmentCount > 1 ? "s" : "") << ".\n";
        }
        else {
            cout << "You don't have any amulet fragments to combine.\n";
        }
        return false;
    }

    // Check if placed on altar first
    bool onAltar = false;
    for (auto entity : getLocation()->getContains()) {
        if (entity->getName() == "altar") {
            onAltar = true;
            break;
        }
    }

    if (!onAltar) {
        cout << "\nThe fragments vibrate intensely but refuse to combine.\n";
        cout << "You notice an ancient altar in the temple - perhaps they need to be placed there?\n";
        return false;
    }

    // Check if player has all three fragments
    bool hasAll = hasAmuletFragment("amethyst") &&
        hasAmuletFragment("sapphire") &&
        hasAmuletFragment("ruby");

    if (!hasAll) {
        cout << "\nYou stand before the altar with ";
        int fragmentCount = 0;
        if (hasAmuletFragment("amethyst")) fragmentCount++;
        if (hasAmuletFragment("sapphire")) fragmentCount++;
        if (hasAmuletFragment("ruby")) fragmentCount++;

        cout << "only " << fragmentCount << " fragment" << (fragmentCount > 1 ? "s" : "") << ".\n";
        cout << "The altar's three depressions mock you with their emptiness.\n";

        // List missing fragments
        cout << "You're missing: ";
        bool first = true;
        if (!hasAmuletFragment("amethyst")) {
            cout << "amethyst";
            first = false;
        }
        if (!hasAmuletFragment("sapphire")) {
            if (!first) cout << ", ";
            cout << "sapphire";
            first = false;
        }
        if (!hasAmuletFragment("ruby")) {
            if (!first) cout << ", ";
            cout << "ruby";
        }
        cout << ".\n";
        return false;
    }

    // All requirements met - combine!
    cout << "\nAs you place the three fragments on the altar:\n";
    cout << "1. The amethyst begins humming at a piercing frequency\n";
    cout << "2. The sapphire emits a wave of comforting warmth\n";
    cout << "3. The ruby burns with sudden intensity\n\n";
    cout << "The fragments rise into the air, spinning rapidly as arcane energy connects them!\n";
    cout << "With a blinding flash and thunderous BOOM, the Amulet of Eldoria is restored!\n";

    // Remove fragments
    removeItem("amethyst");
    removeItem("sapphire");
    removeItem("ruby");

    // Add restored amulet
    Item* amulet = new Item("Amulet of Eldoria",
        "The fully restored artifact thrums with primordial energy.\n"
        "You feel its power resonating with your very soul.",
        false, 0, false);
    addItem(amulet);

    return true;
}