#include "Exit.h"
#include "GameEnums.h"
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
    keyName(keyName) {
}

Room* Exit::getSource() const { return source; }
Room* Exit::getDestination() const { return destination; }
bool Exit::isLocked() const { return locked; }
const string& Exit::getKeyName() const { return keyName; }

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
    Entity::look();  // Show base description
    if (locked) {
        cout << "Locked. Needs: " << keyName << endl;
    }
}