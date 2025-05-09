#pragma once
#include "Entity.h"
#include <map>

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    UP,
    DOWN
};

// Represents a game location with exits to other rooms
class Room : public Entity {
private:
    map<Direction, Entity*> exits;  // All available exits

public:
    Room(const string& name, const string& description);

    // Exit management
    void setExit(Direction direction, Entity* exit);  // Add/update exit
    Entity* getExit(Direction direction) const;      // Get exit if exists

    // Display room info
    void look() const override; 
};