#include "Player.h"
#include <iostream>

// Create player in starting room
Player::Player(const string& name, const string& description, Room* room) :
    Creature(EntityType::PLAYER, name, description, room) {
}

// Take item from current room
bool Player::takeItem(const string& itemName) {
    if (!location) return false;

    Entity* item = location->findEntity(itemName);
    if (item && item->getType() == EntityType::ITEM) {
        location->removeEntity(item);
        inventory.push_back(item);
        cout << "Took " << itemName << endl;
        return true;
    }

    cout << "No " << itemName << " here" << endl;
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
    for (auto item : inventory) {
        if (item->getName() == itemName) return true;
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

// Try to use an item 
bool Player::useItem(const string& itemName) {
    cout << "Can't use " << itemName << " yet" << endl;
    return false; 
}

// Try to combine items 
bool Player::combineItems(const string& item1, const string& item2) {
    cout << "Can't combine " << item1 << " and " << item2 << endl;
    return false; 
}