#pragma once
#include "Entity.h"
#include "GameEnums.h"

class Room;

class Exit : public Entity {
private:
    Direction direction;  // Direction this exit connects from source room
    Room* source;         // The room this exit leads from
    Room* destination;    // The room this exit leads to
    bool locked;          // Whether this exit is currently locked
    string keyName;       // Name of the key required to unlock this exit

    Direction getReverseDirection() const;  // Gets the opposite direction

public:
    Exit(Direction direction, Room* source, Room* destination,
        const string& name, const string& description,
        bool locked = false, const string& keyName = "");

    Direction getDirection() const;
    Room* getSource() const;
    Room* getDestination() const;
    bool isLocked() const;
    const string& getKeyName() const;

    // Actions
    bool unlock(const string& key);
    void look() const override;
};