#include "NPC.h"
#include "Player.h"
#include "Room.h"
#include "Item.h"
#include <iostream>

// Create new NPC in specified room
NPC::NPC(const std::string& name, const std::string& description, Room* room) :
    Creature(EntityType::NPC, name, description, room),
    hasGivenReward(false) {
}

// Add dialogue line to NPC
void NPC::addDialogue(const string& dialogue) {
    dialogues.push_back(dialogue);
}

void NPC::addResponse(const string& playerInput, const string& npcResponse) {
    responses[playerInput] = npcResponse;
}

// Set item trade requirements
void NPC::setInteraction(const string& required, const string& reward) {
    requiredItem = required;
    rewardItem = reward;
}

// Speak default dialogue
void NPC::talk() const {
    if (dialogues.empty()) {
        std::cout << name << " has nothing to say." << std::endl;
        return;
    }

    // Display the first dialogue by default
    std::cout << name << " says: \"" << dialogues[0] << "\"" << std::endl;

    // If there are specific responses, give a hint
    if (!responses.empty()) {
        std::cout << "You could ask about: ";
        for (const auto& pair : responses) {
            std::cout << pair.first << ", ";
        }
        std::cout << std::endl;
    }
}

// Handle player interaction
void NPC::interact(Player* player) {
    if (!requiredItem.empty() && !hasGivenReward) {
        // Check if player has the required item
        if (player->hasItem(requiredItem)) {
            cout << name << " says: \"I see you have " << requiredItem
                << ". Would you like to give it to me?\" (yes/no)" << endl;

            // Get player response
            string response;
            getline(cin, response);
            transform(response.begin(), response.end(), response.begin(), ::tolower);

            if (response == "yes" || response == "y") {
                if (player->removeItem(requiredItem)) {
                    // Successfully gave item
                    cout << "You give " << requiredItem << " to " << name << "." << endl;

                    // Give reward if specified
                    if (!rewardItem.empty()) {
                        // Check if player can carry more items
                        if (player->canCarryMoreItems()) {
                            Item* reward = new Item(rewardItem, "A reward from " + name);
                            player->addItem(reward);
                            cout << name << " gives you " << rewardItem << " in return." << endl;
                        }
                        else {
                            cout << name << " tries to give you " << rewardItem
                                << ", but you can't carry it!" << endl;


                            if (location) {
                                Item* reward = new Item(rewardItem, "A reward from " + name);
                                location->addEntity(reward);
                                cout << rewardItem << " falls to the ground." << endl;
                            }
                        }
                    }

                    // Show dialogue if available
                    if (dialogues.size() > 1) {
                        cout << name << " says: \"" << dialogues[1] << "\"" << endl;
                    }

                    hasGivenReward = true;
                    return;
                }
            }
            else {
                cout << "You decide to keep " << requiredItem << "." << endl;
                return;
            }
        }
        else {
            // Player doesn't have the required item
            cout << name << " says: \"Bring me " << requiredItem
                << " and I'll help you.\"" << endl;
            if (dialogues.size() > 2) {  // Show hint if available
                cout << name << " adds: \"" << dialogues[2] << "\"" << endl;
            }
            return;
        }
    }

    // Default dialogue if no interaction is needed
    talk();
}

void NPC::handlePlayerInput(const std::string& input, Player* player) {
    // Check if the input matches any of our responses
    auto it = responses.find(input);
    if (it != responses.end()) {
        std::cout << name << " says: \"" << it->second << "\"" << std::endl;
    }
    else {
        std::cout << name << " doesn't understand what you're asking about." << std::endl;
    }
}