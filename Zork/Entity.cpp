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
    // First try exact match
    for (auto entity : contains) {
        if (entity->nameMatches(name)) {
            return entity;
        }
    }

    // Then try partial match if no exact match found
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    Entity* bestMatch = nullptr;
    int bestMatchScore = INT_MAX;

    for (auto entity : contains) {
        string entityLower = entity->getName();
        transform(entityLower.begin(), entityLower.end(), entityLower.begin(), ::tolower);

        if (entityLower.find(lowerName) != string::npos) {
            int diff = static_cast<int>(entityLower.length()) - static_cast<int>(lowerName.length());
            int score = diff < 0 ? -diff : diff;

            if (score < bestMatchScore) {
                bestMatch = entity;
                bestMatchScore = score;
            }
        }
    }

    return bestMatch;
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