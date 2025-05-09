#include "Entity.h"
#include <algorithm>
#include <iostream> 

// Create new entity with basic info
Entity::Entity(EntityType type, const string& name, const string& description) :
    type(type), name(name), description(description) {
}

// Clean up all contained items
Entity::~Entity() {
    for (auto entity : contains) {
        delete entity;
    }
}

EntityType Entity::getType() const { return type; }
const string& Entity::getName() const { return name; }
const string& Entity::getDescription() const { return description; }
const list<Entity*>& Entity::getContains() const { return contains; }

// Add item to container
void Entity::addEntity(Entity* entity) {
    contains.push_back(entity);
}

// Remove item from container
void Entity::removeEntity(Entity* entity) {
    contains.remove(entity);
}

// Find item by name (returns nullptr if not found)
Entity* Entity::findEntity(const string& name) const {
    for (auto entity : contains) {
        if (entity->getName() == name) {
            return entity;
        }
    }
    return nullptr;
}

// Check if entity contains specific item
bool Entity::containsEntity(const Entity* entity) const {
    return find(begin(contains), end(contains), entity) != end(contains);
}

// Base update does nothing (override in child classes)
void Entity::update() {
}

// Show entity info and contents
void Entity::look() const {
    cout << name << endl;
    cout << description << endl;

    if (!contains.empty()) {
        cout << "Contains:" << endl;
        for (auto entity : contains) {
            cout << "- " << entity->getName() << endl;
        }
    }
}