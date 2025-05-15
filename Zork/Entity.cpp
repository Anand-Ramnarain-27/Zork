#include "Entity.h"
#include <algorithm>
#include <iostream>
#include <climits> 

Entity::Entity(EntityType type, const string& name, const string& description) :
    type(type), name(name), description(description) {
}

Entity::~Entity() {
    for (auto entity : contains) {
        delete entity;
    }
}

EntityType Entity::getType() const {
    return type;
}

const string& Entity::getName() const {
    return name;
}

const string& Entity::getDescription() const {
    return description;
}

const list<Entity*>& Entity::getContains() const {
    return contains;
}

// ========== Entity Management ==========

void Entity::addEntity(Entity* entity) {
    if (entity != nullptr) {
        contains.push_back(entity);
    }
}

void Entity::removeEntity(Entity* entity) {
    if (entity != nullptr) {
        contains.remove(entity);
    }
}

Entity* Entity::findEntity(const string& name) const {
    if (name.empty()) {
        return nullptr;
    }

    // First try exact match (case-insensitive)
    for (auto entity : contains) {
        if (entity->nameMatches(name)) {
            return entity;
        }
    }

    // Then try partial match if no exact match found
    string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    Entity* bestMatch = nullptr;
    int bestMatchScore = INT_MAX;

    for (auto entity : contains) {
        string entityLower = entity->getName();
        std::transform(entityLower.begin(), entityLower.end(), entityLower.begin(), ::tolower);

        // Check if the entity name contains the search term
        if (entityLower.find(lowerName) != string::npos) {
            // Score based on length difference - closer to search term is better
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

bool Entity::containsEntity(const Entity* entity) const {
    if (entity == nullptr) {
        return false;
    }
    return std::find(contains.begin(), contains.end(), entity) != contains.end();
}

// ========== Name Matching ==========

bool Entity::nameMatches(const string& nameToMatch) const {
    string thisName = name;
    string otherName = nameToMatch;

    // Convert both names to lowercase for case-insensitive comparison
    std::transform(thisName.begin(), thisName.end(), thisName.begin(), ::tolower);
    std::transform(otherName.begin(), otherName.end(), otherName.begin(), ::tolower);

    return thisName == otherName;
}

void Entity::update() {
}

void Entity::look() const {
    // Display entity name and description
    std::cout << name << std::endl;
    std::cout << description << std::endl;

    // Show contained entities if any
    if (!contains.empty()) {
        std::cout << "Contains:" << std::endl;
        for (auto entity : contains) {
            std::cout << "- " << entity->getName() << std::endl;
        }
    }
}