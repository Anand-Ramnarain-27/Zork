#include "NPC.h"
#include "Player.h"
#include "Entity.h"
#include <iostream>

// Create new NPC in specified room
NPC::NPC(const string& name, const string& description, Room* room) :
    Creature(EntityType::NPC, name, description, room) {
}

// Add dialogue line to NPC
void NPC::addDialogue(const string& dialogue) {
    dialogues.push_back(dialogue);
}

// Set item trade requirements
void NPC::setInteraction(const string& required, const string& reward) {
    requiredItem = required;
    rewardItem = reward;
}

// Speak default dialogue
void NPC::talk() const {
    if (dialogues.empty()) {
        cout << name << " ignores you." << endl;
        return;
    }
    cout << name << " says: \"" << dialogues[0] << "\"" << endl;
}

// Handle player interaction
void NPC::interact(Player* player) {
    // Check for item trade
    if (!requiredItem.empty() && !rewardItem.empty()) {
        if (player->hasItem(requiredItem)) {
            // Special dialogue if available
            if (dialogues.size() > 1) {
                cout << name << " says: \"" << dialogues[1] << "\"" << endl;
            }
            cout << name << " gives you " << rewardItem
                << " for your " << requiredItem << endl;
            return;
        }
    }
    // Default to regular talk
    talk();
}