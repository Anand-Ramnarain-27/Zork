#pragma once
#include "Creature.h"
#include <vector>
#include <map>
#include <string>

class Player;
class Room;

class NPC : public Creature {
private:
    std::vector<std::string> dialogues;
    std::map<std::string, std::string> responses; // Key: player input, Value: NPC response
    std::string requiredItem;
    std::string rewardItem;
    bool hasGivenReward;
    bool trusts;              // Tracks if NPC trusts the player
    bool hasImportantInfo;    // If NPC has important story information
    bool hasInteracted;       // Track if player has interacted with this NPC

public:
    // Constructor
    NPC(const std::string& name, const std::string& description, Room* room);

    // Dialogue and response management
    void addDialogue(const std::string& dialogue);
    void addResponse(const std::string& playerInput, const std::string& npcResponse);

    // Setup methods
    void setInteraction(const std::string& required, const std::string& reward);
    void setAsEnemy(bool enemy);
    void setHasImportantInfo(bool hasInfo);
    void setPreventReinteraction(bool prevent);
    void setHasInteracted(bool interacted);

    // Interaction methods
    void talk() const;
    void interact(Player* player);
    void handlePlayerInput(const std::string& input, Player* player);

    // Status query methods
    bool getIsEnemy() const { return isEnemy; }
    bool hasPlayerInteracted() const;

    bool isEnemy;             // Determines if NPC is hostile to player
    bool preventReinteraction; // Prevents multiple interactions if set to true
};