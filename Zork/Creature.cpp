#include "Creature.h"
#include "Exit.h"  
#include "GameEnums.h"
#include <iostream>

// Helper: Convert direction enum to string
string directionToString(Direction direction) {
    switch (direction) {
    case Direction::NORTH: return "north";
    case Direction::SOUTH: return "south";
    case Direction::EAST: return "east";
    case Direction::WEST: return "west";
    case Direction::UP: return "up";
    case Direction::DOWN: return "down";
    default: return "unknown direction";
    }
}

// Adds creature to starting room
Creature::Creature(EntityType type, const string& name, const string& description, Room* room) :
    Entity(type, name, description), location(room) {
    if (room) room->addEntity(this);
}

// Removes from current room
Creature::~Creature() {
    if (location) location->removeEntity(this);
}

Room* Creature::getLocation() const { return location; }

// Updates location and handles room registration
void Creature::setLocation(Room* newLocation) {
    if (location) location->removeEntity(this);
    location = newLocation;
    if (location) location->addEntity(this);
}

// Handles movement between rooms
void Creature::move(Direction direction) {
    if (!location) return;

    // Check if exit exists
    Entity* exitEntity = location->getExit(direction);
    Exit* exit = dynamic_cast<Exit*>(exitEntity);

    if (!exit) {
        cout << "Can't go that way." << endl;
        return;
    }

    // Check if locked
    if (exit->isLocked()) {
        cout << "It's locked." << endl;
        return;
    }

    // Move and show new room
    setLocation(exit->getDestination());
    cout << "Moved " << directionToString(direction)
        << " to " << location->getName() << endl;
    location->look();
}

// Shows creature details + current room
void Creature::look() const {
    Entity::look();
    if (location) cout << "In: " << location->getName() << endl;
}