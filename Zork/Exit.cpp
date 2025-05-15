#include "Exit.h"
#include "GameEnums.h"
#include "Room.h" 
#include <iostream>

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
    // Automatically register with both rooms
    source->setExit(direction, this);
}

bool Exit::unlock(const string& key) {
    if (key == keyName) {
        locked = false;
        return true;
    }
    return false;
}

void Exit::look() const {
    Entity::look();
    if (locked) {
        cout << "The exit is locked. You need " << keyName << " to unlock it." << endl;
    }
}