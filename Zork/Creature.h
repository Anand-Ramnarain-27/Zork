#pragma once
#include "Entity.h"
#include "GameEnums.h"
#include <string>

using std::string;

class Room;

class Creature : public Entity {
protected:
    Room* location;  // Current location of the creature

    int health;      // Current health points
    int maxHealth;   // Maximum health points

public:
    Creature(EntityType type, const string& name, const string& description, Room* room);
    virtual ~Creature();

    // Location management
    Room* getLocation() const;
    void setLocation(Room* newLocation);

    // Movement
    virtual void move(Direction direction);

    // Information display
    virtual void look() const override;

    // Health management
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int newHealth);
    bool isAlive() const;
    virtual void takeDamage(int amount);
    void heal(int amount);
};