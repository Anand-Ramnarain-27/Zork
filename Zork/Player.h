#pragma once
#include "Creature.h"
#include "Entity.h"
#include "GameEnums.h"
#include "Room.h"
#include <vector>

class Player : public Creature {
private:
    vector<Entity*> inventory;
    bool hasBackpack() const;

    int lanternTurnsRemaining;

    int moralAlignment = 0; // -10 to 10 (negative = selfish, positive = altruistic)
    bool hasBetrayedNPCs = false;
    bool hasSacrificed = false;
    int movesTaken;

public:
    Player(const string& name, const string& description, Room* room);

    // Movement commands
    bool moveTo(Direction direction);
    bool moveTo(Room* room);

    // Inventory management
    bool takeItem(const string& itemName);
    bool dropItem(const string& itemName);
    bool hasItem(const string& itemName) const;
    void showInventory() const;

    // NPC interaction
    void addItem(Entity* item);
    bool removeItem(const std::string& itemName);

    // Item interaction
    bool useItem(const string& itemName);
    bool combineItems(const string& item1, const string& item2);

    // Inventory management
    bool canCarryMoreItems() const;

    // Amulet-specific mechanics
    bool combineAmuletFragments();
    bool hasAmuletFragment(const string& fragmentName) const;
    bool placeAmuletOnAltar();

    // Lantern management
    void decrementLanternTurns() { if (lanternTurnsRemaining > 0) lanternTurnsRemaining--; }
    int getLanternTurnsRemaining() const { return lanternTurnsRemaining; }
    bool hasActiveLantern() const;

    // Combat system
    void takeDamage(int amount) override;
    bool attackCreature(const string& creatureName);

    // Stealing and other negative actions
    bool stealItem(const string& itemName);

    // Moral choice tracking
    void makeSelfishChoice() { moralAlignment--; }
    void makeAltruisticChoice() { moralAlignment++; }
    void betrayNPCs() { hasBetrayedNPCs = true; }
    int getAlignment() const { return moralAlignment; }
    bool hasBetrayed() const { return hasBetrayedNPCs; }
    void showAlignment() const;

    // NEW METHODS FOR MULTIPLE ENDINGS
    void sacrificeNPC(const string& npcName);
    void forgiveEnemy(const string& enemyName);
    bool makeMoralChoice(int choiceType);
    bool corruptArtifact(const string& artifactName);

    void incrementMoves() { movesTaken++; }
    int getMovesTaken() const { return movesTaken; }
};