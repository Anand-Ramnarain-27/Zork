#pragma once
#include "Entity.h"

// Represents game items that can be picked up/used
class Item : public Entity {
private:
    bool isContainer;  // Can hold other items
	int capacity;      // Max items it can hold for now unlimited
    bool isFragment;   // Special fragment item for amulet to win game

public:
    Item(const string& name, const string& description,
        bool isContainer = false, int capacity = 0,
        bool isFragment = false);

    bool getIsContainer() const;
    int getCapacity() const;
    bool getIsFragment() const;

    bool canContain(const Entity* entity) const; // Check if item can hold another
    void look() const override; // Show item details
};