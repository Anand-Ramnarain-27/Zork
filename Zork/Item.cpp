#include "Item.h"
#include "Entity.h"
#include <iostream>

Item::Item(const string& name, const string& description,
    bool isContainer, int capacity, bool isFragment, bool isFixedInPlace) :
    Entity(EntityType::ITEM, name, description),
    isContainer(isContainer),
    capacity(capacity),
    isFragment(isFragment),
    isLit(false),
    isFixedInPlace(isFixedInPlace) {
}

bool Item::getIsContainer() const { return isContainer; }
int Item::getCapacity() const { return capacity; }
bool Item::getIsFragment() const { return isFragment; }
bool Item::getIsFixedInPlace() const { return isFixedInPlace; } // New getter implementation

// Check if this item can hold another entity
bool Item::canContain(const Entity* entity) const {
    if (!isContainer) return false;
    if (capacity > 0 && contains.size() >= capacity) return false;
    return true;
}

// Show detailed item description
void Item::look() const {
    Entity::look();
    if (isContainer) {
        cout << "It can hold items";
        if (capacity > 0) {
            cout << " (capacity: " << capacity << ")";
        }
        cout << "." << endl;
    }
    if (isFragment) {
        cout << "It looks like part of a broken amulet." << endl;
    }
    if (isFixedInPlace) {
        cout << "It appears to be permanently fixed in place." << endl;
    }
}