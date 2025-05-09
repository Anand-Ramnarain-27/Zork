#pragma once
#include "Entity.h"
#include "Room.h"

// Base class for all living things in the game (player, NPCs)
class Creature : public Entity {
protected:
    Room* location; // Current room

public:
    Creature(EntityType type, const string& name, const string& description, Room* room);
    virtual ~Creature();

    // Get current room
    Room* getLocation() const;

    // Change room (handles registration automatically)
    void setLocation(Room* newLocation);

    // Try to move in a direction (checks exits)
    virtual void move(Direction direction);

    // Show creature info + location
    virtual void look() const override;
};