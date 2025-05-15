#include "Creature.h"
#include "Exit.h"
#include "Room.h"
#include "GameEnums.h"
#include <iostream>
#include <algorithm>

Creature::Creature(EntityType type, const string& name, const string& description, Room* room) :
    Entity(type, name, description),
    location(room),
    health(100),    // Default health to 100
    maxHealth(100) {
    if (room != nullptr) {
        room->addEntity(this);
    }
}

Creature::~Creature() {
    // Remove this creature from its location when destroyed
    if (location != nullptr) {
        location->removeEntity(this);
    }
}

// ========== Location Management ==========

Room* Creature::getLocation() const {
    return location;
}

void Creature::setLocation(Room* newLocation) {
    // Remove from current location
    if (location != nullptr) {
        location->removeEntity(this);
    }

    // Set new location
    location = newLocation;

    // Add to new location
    if (location != nullptr) {
        location->addEntity(this);
    }
}

// ========== Movement ==========

void Creature::move(Direction direction) {
    if (location == nullptr) return;

    // Check if there's an exit in the specified direction
    Entity* exitEntity = location->getExit(direction);
    if (exitEntity == nullptr || exitEntity->getType() != EntityType::EXIT) {
        std::cout << "You can't go that way." << std::endl;
        return;
    }

    // Properly cast to Exit
    Exit* exit = dynamic_cast<Exit*>(exitEntity);
    if (exit == nullptr) {
        std::cout << "You can't go that way." << std::endl;
        return;
    }

    // Check if the exit is locked
    if (exit->isLocked()) {
        std::cout << "The exit is locked." << std::endl;
        return;
    }

    // Move to the destination
    setLocation(exit->getDestination());
    std::cout << "You move " << directionToString(direction) << " to " << location->getName() << std::endl;
    location->look();
}

// ========== Information Display ==========

void Creature::look() const {
    // Display base entity information
    Entity::look();

    // Display location information if available
    if (location != nullptr) {
        std::cout << "Location: " << location->getName() << std::endl;
    }
}

// ========== Health Management ==========

int Creature::getHealth() const {
    return health;
}

int Creature::getMaxHealth() const {
    return maxHealth;
}

void Creature::setHealth(int newHealth) {
    health = std::max(0, std::min(newHealth, maxHealth));
}

bool Creature::isAlive() const {
    return health > 0;
}

void Creature::takeDamage(int amount) {
    // Ensure health doesn't go below 0
    health = std::max(0, health - amount);

    // Display damage information
    std::cout << name << " takes " << amount << " damage! ";
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;

    // Check if creature is defeated
    if (!isAlive()) {
        std::cout << name << " has been defeated!" << std::endl;
    }
}

void Creature::heal(int amount) {
    // Ensure health doesn't exceed maxHealth
    health = std::min(health + amount, maxHealth);

    // Display healing information
    std::cout << name << " heals " << amount << " HP. ";
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
}