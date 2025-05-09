#include "Item.h"
#include <iostream>

// Create new item with properties
Item::Item(const string& name, const string& description,
    bool isContainer, int capacity, bool isFragment) :
    Entity(EntityType::ITEM, name, description),
    isContainer(isContainer),
    capacity(capacity),
    isFragment(isFragment) {
}

bool Item::getIsContainer() const { return isContainer; }
int Item::getCapacity() const { return capacity; }
bool Item::getIsFragment() const { return isFragment; }

// Check if this item can hold another entity
bool Item::canContain(const Entity* entity) const {
    if (!isContainer) return false;
    if (capacity > 0 && contains.size() >= capacity) return false;
    return true;
}

// Show detailed item description
void Item::look() const {
    Entity::look(); // Show basic info

    if (isContainer) {
        cout << "Can hold items";
        if (capacity > 0) cout << " (" << capacity << " max)";
        cout << endl;
    }

    if (isFragment) {
        cout << "This is a piece of the ancient amulet!" << endl;
    }
}