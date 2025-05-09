#pragma once
#include "Creature.h"
#include "Player.h"
#include <vector>

// Non-player characters with dialogue and interactions
class NPC : public Creature {
private:
    vector<string> dialogues;  // Available conversation lines
    string requiredItem;      // Item needed for special interaction
    string rewardItem;        // Item given when requirement met

public:
    NPC(const string& name, const string& description, Room* room);

    // Dialogue management
    void addDialogue(const string& dialogue);  // Add conversation text
    void setInteraction(const string& required, const string& reward); // Set trade items

    // Player interactions
    void talk() const;        // Speak default dialogue
    void interact(Player* player); // Handle item exchanges
};