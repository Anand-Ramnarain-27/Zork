#include "Item.h"
#include <iostream>

Item::Item(const string& name, const string& description,
    bool isContainer, int capacity, bool isFragment, bool isFixedInPlace) :
    Entity(EntityType::ITEM, name, description),
    isContainer(isContainer),
    capacity(capacity),
    isFragment(isFragment),
    isLit(false),  // Items start unlit by default
    isFixedInPlace(isFixedInPlace) {
}

bool Item::getIsContainer() const {
    return isContainer;
}

int Item::getCapacity() const {
    return capacity;
}

bool Item::getIsFragment() const {
    return isFragment;
}

bool Item::getIsFixedInPlace() const {
    return isFixedInPlace;
}

bool Item::getIsLit() const {
    return isLit;
}

void Item::setLit(bool lit) {
    isLit = lit;
}

// ========== Item Behavior ==========

bool Item::canContain(const Entity* entity) const {
    // Check if this item can contain the specified entity
    if (!isContainer) {
        return false; // This item is not a container
    }

    if (entity == nullptr) {
        return false; // Cannot contain a null entity
    }

    if (capacity > 0 && contains.size() >= static_cast<size_t>(capacity)) {
        return false; // Container is at capacity
    }

    return true; // Container has space for the entity
}

// ========== Information Display ==========

void Item::look() const {
    Entity::look(); // Shows name and description
    if (isContainer) {
        std::cout << "It can hold items";
        if (capacity > 0) {
            std::cout << " (capacity: " << capacity << ")";
        }
        std::cout << "." << std::endl;

        // Show contents if any
        if (!contains.empty()) {
            std::cout << "Inside you see:" << std::endl;
            for (auto entity : contains) {
                std::cout << "- " << entity->getName() << std::endl;
            }
        }
        else {
            std::cout << "It is currently empty." << std::endl;
        }
    }

    // Display fragment information if applicable
    if (isFragment) {
        std::cout << "It looks like part of a broken amulet." << std::endl;
    }

    // Display fixed in place information if applicable
    if (isFixedInPlace) {
        std::cout << "It appears to be permanently fixed in place." << std::endl;
    }

    // Display lighting status for light sources
    if (getName() == "lantern" || getName() == "torch") {
        std::cout << "It is currently " << (isLit ? "lit" : "unlit") << "." << std::endl;
    }
}