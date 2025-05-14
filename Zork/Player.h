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

    int lanternTurnsRemaining;

public:
    Player(const string& name, const string& description, Room* room);

    // Movement commands
    bool moveTo(Direction direction);
    bool moveTo(Room* room);

    // Inventory management
    bool takeItem(const string& itemName);
    bool dropItem(const string& itemName);
    bool hasItem(const string& itemName) const;
    void showInventory() const;

    // New methods for NPC interaction
    void addItem(Entity* item);
    bool removeItem(const std::string& itemName);

    // Special actions
    bool useItem(const string& itemName);
    bool combineItems(const string& item1, const string& item2);

    bool canCarryMoreItems() const;

    bool combineAmuletFragments();
    bool hasAmuletFragment(const string& fragmentName) const;
    bool placeAmuletOnAltar();

    // Lantern management
    void decrementLanternTurns() { if (lanternTurnsRemaining > 0) lanternTurnsRemaining--; }
    int getLanternTurnsRemaining() const { return lanternTurnsRemaining; }
    bool hasActiveLantern() const;

    // Override takeDamage from Creature
    void takeDamage(int amount) override;
};