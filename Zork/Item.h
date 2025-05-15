#pragma once
#include "Entity.h"

class Item : public Entity {
private:
    bool isContainer;      // Whether this item can contain other items
    int capacity;          // Maximum number of items this container can hold (0 = unlimited)
    bool isFragment;       // Whether this is an amulet fragment for the main quest
    bool isLit;            // Whether this item is currently providing light (for lanterns, torches)
    bool isFixedInPlace;   // Whether this item can be picked up or is fixed to its location

public:
    Item(const string& name, const string& description,
        bool isContainer = false, int capacity = 0,
        bool isFragment = false, bool isFixedInPlace = false);

    bool getIsContainer() const;
    int getCapacity() const;
    bool getIsFragment() const;
    bool getIsFixedInPlace() const;
    bool getIsLit() const;

    // Item state modification
    void setLit(bool lit);

    // Item behavior
    bool canContain(const Entity* entity) const;

    // Information display
    void look() const override;
};