#include "Exit.h"
#include "GameEnums.h"
#include "Room.h" 
#include <iostream>

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
    // Automatically register this exit with the source room
    if (source != nullptr) {
        source->setExit(direction, this);
    }
}

Direction Exit::getDirection() const {
    return direction;
}

Room* Exit::getSource() const {
    return source;
}

Room* Exit::getDestination() const {
    return destination;
}

bool Exit::isLocked() const {
    return locked;
}

const string& Exit::getKeyName() const {
    return keyName;
}

Direction Exit::getReverseDirection() const {
    switch (direction) {
    case Direction::NORTH: return Direction::SOUTH;
    case Direction::SOUTH: return Direction::NORTH;
    case Direction::EAST:  return Direction::WEST;
    case Direction::WEST:  return Direction::EAST;
    case Direction::UP:    return Direction::DOWN;
    case Direction::DOWN:  return Direction::UP;
    default: return Direction::NORTH;
    }
}

// ========== Actions ==========

bool Exit::unlock(const string& key) {
    // Check if the provided key matches the required key
    if (key == keyName) {
        locked = false;
        return true;
    }
    return false;
}

void Exit::look() const {
    // Display basic entity information
    Entity::look();

    // Display lock status if locked
    if (locked) {
        std::cout << "The exit is locked. You need " << keyName << " to unlock it." << std::endl;
    }
    else {
        std::cout << "This exit leads to " << destination->getName() << "." << std::endl;
    }
}