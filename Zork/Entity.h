#pragma once
#include <string>
#include <list>
#include <vector>

using namespace std;

enum class EntityType {
    ENTITY,    // Base type
    ROOM,      // Game locations
    EXIT,      // Connections between rooms
    ITEM,      // Objects that can be picked up
    CREATURE,  // Living beings
    PLAYER,    // The player character
    NPC        // Non-player characters
};

// Base class for all game objects
class Entity {
protected:
    EntityType type;
    string name;
    string description;
    list<Entity*> contains; // Items inside this entity

public:
    Entity(EntityType type, const string& name, const string& description);
    virtual ~Entity();

    // Basic info getters
    EntityType getType() const;
    const string& getName() const;
    const string& getDescription() const;
    const list<Entity*>& getContains() const;

    // Manage contained items
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    Entity* findEntity(const string& name) const; // Find by name
    bool containsEntity(const Entity* entity) const; // Check if exists

    virtual void update(); 
    virtual void look() const; // Show description + contents
};