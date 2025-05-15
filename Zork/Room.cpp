#include "Room.h"
#include "GameEnums.h"
#include "Exit.h"
#include "Entity.h"
#include <iostream>

void Room::setExit(Direction direction, Entity* exit) {
    exits[direction] = exit;
}

Entity* Room::getExit(Direction direction) const {
    auto it = exits.find(direction);
    return it != exits.end() ? it->second : nullptr;
}

void Room::look() const {
    std::cout << "\n";
    // First print room name and description
    std::cout << name << std::endl;
    std::cout << description << std::endl;

    // Print contained items (excluding player)
    bool hasItems = false;
    for (auto entity : contains) {
        if (entity->getType() != EntityType::PLAYER) {
            if (!hasItems) {
                std::cout << "Contains:" << std::endl;
                hasItems = true;
            }
            std::cout << "- " << entity->getName() << std::endl;
        }
    }

    // Print exits
    std::cout << "Exits:" << std::endl;
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
        std::cout << "- " << direction << std::endl;
    }
}