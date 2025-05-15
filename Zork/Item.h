#pragma once
#include "Entity.h"

class Item : public Entity {
private:
    bool isContainer;
    int capacity; // For container items
    bool isFragment; // For amulet fragments
    bool isLit;
    bool isFixedInPlace; // New property for immovable objects

public:
    Item(const string& name, const string& description,
        bool isContainer = false, int capacity = 0,
        bool isFragment = false, bool isFixedInPlace = false);

    bool getIsContainer() const;
    int getCapacity() const;
    bool getIsFragment() const;
    bool getIsFixedInPlace() const; 

    // Special actions
    bool canContain(const Entity* entity) const;
    void look() const override;

    // Lantern specific methods
    void setLit(bool lit) { isLit = lit; }
    bool getIsLit() const { return isLit; }
};