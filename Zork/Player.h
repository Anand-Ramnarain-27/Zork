#pragma once
#include "Creature.h"
#include "Entity.h"
#include "GameEnums.h"
#include "Room.h"
#include <vector>

// The player character with inventory system
class Player : public Creature {
private:
    vector<Entity*> inventory;  // Items being carried
    bool hasBackpack() const;

public:
    Player(const string& name, const string& description, Room* room);

    // Movement commands
    bool moveTo(Direction direction);
    bool moveTo(Room* room);

    // Inventory management
    bool takeItem(const string& itemName);    // Pick up from current room
    bool dropItem(const string& itemName);    // Drop to current room
    bool hasItem(const string& itemName) const; // Check inventory
    void showInventory() const;              // List carried items
    void addItem(Entity* item);
    bool removeItem(const std::string& itemName);
    bool canCarryMoreItems() const;

    // Puzzle interactions
    bool useItem(const string& itemName);     // Try to use an item
    bool combineItems(const string& item1, const string& item2); // Merge items
};