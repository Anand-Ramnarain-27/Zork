#pragma once
#include "Entity.h"
#include "GameEnums.h"

class Room;

// Represents a connection between two rooms
class Exit : public Entity {
private:
    Direction direction; 
    Room* source;        // Starting room
    Room* destination;   // Connecting room
    bool locked;        // Whether exit is locked
    string keyName;     // Item needed to unlock

public:
    Exit(Direction direction, Room* source, Room* destination,
        const string& name, const string& description,
        bool locked = false, const string& keyName = "");

    Room* getSource() const;
    Room* getDestination() const;
    bool isLocked() const;
    const string& getKeyName() const;

    // Try to unlock with an item (returns true if successful)
    bool unlock(const string& key);

    // Show exit description + lock status
    void look() const override;
};