#pragma once
#include <string>
#include <list>
#include <vector>
#include <algorithm>

using std::string;
using std::list;
using std::vector;

enum class EntityType {
    ENTITY,     // Base entity type
    ROOM,       // Game locations
    EXIT,       // Connections between rooms
    ITEM,       // Collectible objects
    CREATURE,   // Living entities (base class)
    PLAYER,     // User-controlled character
    NPC         // Non-player characters
};

class Entity {
protected:
    EntityType type;        // Type of this entity
    string name;            // Name of the entity
    string description;     // Description of the entity
    list<Entity*> contains; // Items contained within this entity

public:
    Entity(EntityType type, const string& name, const string& description);
    virtual ~Entity();

    EntityType getType() const;
    const string& getName() const;
    const string& getDescription() const;
    const list<Entity*>& getContains() const;

    // Entity management
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    Entity* findEntity(const string& name) const;
    bool containsEntity(const Entity* entity) const;

    // Name matching (case-insensitive)
    bool nameMatches(const string& nameToMatch) const;

    virtual void update();
    virtual void look() const;
};