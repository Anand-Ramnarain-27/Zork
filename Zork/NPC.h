#pragma once
#include "Creature.h"
#include "Player.h"
#include <vector>
#include <map>

// Non-player characters with dialogue and interactions
class NPC : public Creature {
private:
    vector<string> dialogues;  // Available conversation lines
    map<string, string> responses; // Key: player input, Value: NPC response
    string requiredItem;      // Item needed for special interaction
    string rewardItem;        // Item given when requirement met
    bool hasGivenReward;

public:
    NPC(const string& name, const string& description, Room* room);

    // Dialogue management
    void addDialogue(const string& dialogue);  // Add conversation text
    void addResponse(const string& playerInput, const string& npcResponse);
    void setInteraction(const string& required, const string& reward); // Set trade items

    // Player interactions
    void talk() const;        // Speak default dialogue
    void interact(Player* player); // Handle item exchanges
    void handlePlayerInput(const std::string& input, Player* player);
};