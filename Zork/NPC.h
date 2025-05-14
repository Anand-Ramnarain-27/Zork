#pragma once
#include "Creature.h"
#include "Player.h"
#include <vector>
#include <map>

class NPC : public Creature {
private:
    std::vector<std::string> dialogues;
    std::map<std::string, std::string> responses; // Key: player input, Value: NPC response
    std::string requiredItem;
    std::string rewardItem;
    bool hasGivenReward;
    bool trusts;              // Tracks if NPC trusts the player
    bool hasImportantInfo;    // If NPC has important story information
    bool isEnemy;

public:
    NPC(const std::string& name, const std::string& description, Room* room);

    void addDialogue(const std::string& dialogue);
    void addResponse(const std::string& playerInput, const std::string& npcResponse);
    void setInteraction(const std::string& required, const std::string& reward);
    void setAsEnemy(bool enemy);
    void setHasImportantInfo(bool hasInfo);

    void talk() const;
    void interact(Player* player);
    void handlePlayerInput(const std::string& input, Player* player);
};