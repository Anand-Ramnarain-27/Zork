#include "Room.h"
#include "GameEnums.h"
#include "Entity.h"
#include "Exit.h"
#include <iostream>

// Create new room with name and description
Room::Room(const string& name, const string& description) :
    Entity(EntityType::ROOM, name, description) {
}

// Set exit in specified direction
void Room::setExit(Direction direction, Entity* exit) {
    exits[direction] = exit;
}

// Get exit in specified direction (returns nullptr if none)
Entity* Room::getExit(Direction direction) const {
    auto it = exits.find(direction);
    return it != exits.end() ? it->second : nullptr;
}

// Show full room description
void Room::look() const {
    // First print room name and description
    cout << name << endl;
    cout << description << endl;

    // Print contained items (excluding player)
    bool hasItems = false;
    for (auto entity : contains) {
        if (entity->getType() != EntityType::PLAYER) {
            if (!hasItems) {
                cout << "Contains:" << endl;
                hasItems = true;
            }
            cout << "- " << entity->getName() << endl;
        }
    }

    // Print exits
    cout << "Exits:" << endl;
    for (auto exit : exits) {
        string direction;
        switch (exit.first) {
        case Direction::NORTH: direction = "north"; break;
        case Direction::SOUTH: direction = "south"; break;
        case Direction::EAST: direction = "east"; break;
        case Direction::WEST: direction = "west"; break;
        case Direction::UP: direction = "up"; break;
        case Direction::DOWN: direction = "down"; break;
        }
        cout << "- " << direction << endl;
    }
}