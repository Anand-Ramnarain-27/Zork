#include "Exit.h"
#include "GameEnums.h"
#include "Room.h"
#include <iostream>

// Create new exit between rooms
Exit::Exit(Direction direction, Room* source, Room* destination,
    const string& name, const string& description,
    bool locked, const string& keyName) :
    Entity(EntityType::EXIT, name, description),
    direction(direction),
    source(source),
    destination(destination),
    locked(locked),
    keyName(keyName)
{
    // Register exit with source room
    source->setExit(direction, this);
}

// Returns the opposite direction
Direction Exit::getReverseDirection() const {
    switch (direction) {
    case Direction::NORTH: return Direction::SOUTH;
    case Direction::SOUTH: return Direction::NORTH;
    case Direction::EAST:  return Direction::WEST;
    case Direction::WEST:  return Direction::EAST;
    case Direction::UP:    return Direction::DOWN;
    case Direction::DOWN:  return Direction::UP;
    default: return Direction::NORTH; // Fallback
    }
}

// Attempt to unlock exit with key item
bool Exit::unlock(const string& key) {
    if (key == keyName) {
        locked = false;
        return true;
    }
    return false;
}

// Show exit info including lock status
void Exit::look() const {
    Entity::look(); // Show base description

    if (locked) {
        cout << "Locked (requires " << keyName << ")" << endl;
    }
}