#include "NPC.h"
#include "Player.h"
#include "Room.h"
#include "Item.h"
#include <iostream>
#include <algorithm>

using namespace std;

NPC::NPC(const std::string& name, const std::string& description, Room* room) :
    Creature(EntityType::NPC, name, description, room),
    hasGivenReward(false),
    trusts(true), // Add trust state - NPCs initially trust player
    hasImportantInfo(false),
    isEnemy(false) {
}

void NPC::addDialogue(const std::string& dialogue) {
    dialogues.push_back(dialogue);
}

void NPC::addResponse(const std::string& playerInput, const std::string& npcResponse) {
    responses[playerInput] = npcResponse;
}

void NPC::setInteraction(const std::string& required, const std::string& reward) {
    requiredItem = required;
    rewardItem = reward;
}

void NPC::setAsEnemy(bool enemy) {
    isEnemy = enemy;
}

void NPC::setHasImportantInfo(bool hasInfo) {
    hasImportantInfo = hasInfo;
}

void NPC::talk() const {
    if (dialogues.empty()) {
        std::cout << name << " has nothing to say." << std::endl;
        return;
    }

    // Display ALL dialogue lines instead of just the first one
    std::cout << name << " says:" << std::endl;
    for (const auto& line : dialogues) {
        std::cout << "\"" << line << "\"" << std::endl;
    }

    // If there are specific responses, give a hint
    if (!responses.empty()) {
        std::cout << "You could ask about: ";
        for (const auto& pair : responses) {
            std::cout << pair.first << ", ";
        }
        std::cout << std::endl;
    }
}

void NPC::interact(Player* player) {
    // Check player's alignment and betrayal status
    if (player->getAlignment() < -3 || player->hasBetrayed()) {
        if (trusts) {
            // First time mistrust - NPC becomes cautious
            cout << name << " eyes you warily...\n";

            if (player->hasBetrayed()) {
                cout << "\"I've heard about your betrayals, " << player->getName() << ". ";
                cout << "Why should I trust you?\"\n";
            }
            else {
                cout << "\"There's a darkness about you that I don't trust, " << player->getName() << ".\"\n";
            }

            trusts = false;

            // If enemy NPC, they become more aggressive
            if (isEnemy) {
                cout << name << " reaches for their weapon. \"Stay back!\"\n";
                return;
            }

            // If NPC has important info, they hide it
            if (hasImportantInfo) {
                cout << "\"I don't think I should tell you what I know.\"\n";
                return;
            }
        }
        else {
            // Already mistrusting
            cout << name << " refuses to speak with you further.\n";
            cout << "\"Leave me be, " << player->getName() << ".\"\n";
            return;
        }
    }
    else if (!trusts && player->getAlignment() > 2) {
        // Redemption - NPC begins to trust again
        cout << name << " seems to relax slightly around you.\n";
        cout << "\"Perhaps I misjudged you, " << player->getName() << ".\"\n";
        trusts = true;
    }

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

                    // Improve alignment for helping
                    player->makeAltruisticChoice();

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
                            // Optionally drop the reward in the room
                            if (location) {
                                Item* reward = new Item(rewardItem, "A reward from " + name);
                                location->addEntity(reward);
                                cout << rewardItem << " falls to the ground." << endl;
                            }
                        }
                    }

                    // Show additional dialogue if available
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

    // If NPC has important info and player is trustworthy
    if (hasImportantInfo && trusts) {
        // Offer special dialogue for quest-related information
        cout << name << " leans in closer and whispers:\n";
        if (dialogues.size() > 3) {
            cout << "\"" << dialogues[3] << "\"\n";
        }
        else {
            cout << "\"I've heard rumors of an amulet fragment hidden in a nearby cave.\"\n";
        }

        // Choice to pursue this info or not
        cout << "\nYou could:\n";
        cout << "1. Thank them for the information\n";
        cout << "2. Ask for more details\n";
        cout << "3. Ignore this seemingly useless gossip\n";
        cout << "Choose (1-3): ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            cout << "You thank " << name << " for the valuable information.\n";
            player->makeAltruisticChoice();
        }
        else if (choice == "2") {
            cout << "You eagerly ask for more details.\n";
            cout << name << " shares everything they know about the rumor.\n";
            if (dialogues.size() > 4) {
                cout << name << ": \"" << dialogues[4] << "\"\n";
            }
            player->makeAltruisticChoice();
        }
        else if (choice == "3") {
            cout << "You dismiss " << name << "'s information with a shrug.\n";
            cout << name << " looks hurt by your indifference.\n";
            player->makeSelfishChoice();
            trusts = false;
        }
        return;
    }

    if (name == "Blacksmith" && !hasGivenReward) {
        if (player->hasItem("bread")) {
            cout << name << " eyes the bread hungrily. \"I'll trade my rusty key for that loaf.\" (yes/no)\n";
            string response;
            getline(cin, response);
            transform(response.begin(), response.end(), response.begin(), ::tolower);

            if (response == "yes" || response == "y") {
                if (player->removeItem("bread")) {
                    cout << "You trade the bread for the rusty key.\n";
                    cout << "The blacksmith tears into the loaf. \"This mine key is yours now. Be careful down there.\"\n";

                    Item* key = new Item("rusty key", "An old iron key that opens the mine entrance");
                    player->addItem(key);
                    player->makeAltruisticChoice();
                    hasGivenReward = true;
                    return;
                }
            }
            else {
                cout << "You decide to keep your bread for now.\n";
                return;
            }
        }
        else {
            cout << "\"Bring me some bread if you want the mine key. A man's got to eat.\"\n";
            return;
        }
    }
    else if (name == "Hermit" && !hasGivenReward && player->hasItem("potion")) {
        cout << "The hermit looks at your potion with desperate eyes.\n";
        cout << "\"That elixir would ease my suffering greatly. Will you share it?\" (yes/no)\n";

        string response;
        getline(cin, response);
        transform(response.begin(), response.end(), response.begin(), ::tolower);

        if (response == "yes" || response == "y") {
            if (player->removeItem("potion")) {
                cout << "You give the potion to the hermit.\n";
                cout << "He drinks it and his breathing eases. \"Thank you, kind soul.\"\n";
                cout << "\"The lantern you found will protect you in the mine's darkness. The shadows flee from its light.\"\n";
                cout << "\"But beware - its oil won't last forever. Make haste when in dark places.\"\n";

                Item* scroll = new Item("scroll", "Ancient parchment with instructions for combining the amulet fragments");
                player->addItem(scroll);
                player->makeAltruisticChoice();
                hasGivenReward = true;
                return;
            }
        }
    }
    else if (name == "Rival Adventurer") {
        cout << "The rival adventurer sizes you up. \"What will it be? Alliance or competition?\"\n";
        cout << "1. Propose an alliance\n";
        cout << "2. Challenge to a competition\n";
        cout << "3. Attack the rival\n";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            cout << "\"An alliance it is,\" the rival agrees with a smile.\n";
            cout << "\"Here's a map piece I found. It shows a secret passage in the tower.\"\n";

            Item* mapPiece = new Item("map piece", "Shows a hidden passage in the sorcerer's tower");
            player->addItem(mapPiece);
            player->makeAltruisticChoice();
            hasGivenReward = true;
        }
        else if (choice == "2") {
            cout << "\"A competition? I like your spirit,\" the rival grins.\n";
            cout << "\"First one to find the ruby fragment wins. No cheating!\"\n";
        }
        else if (choice == "3") {
            cout << "You lunge at the rival adventurer!\n";
            cout << "They dodge your attack. \"So that's how you want to play? Fine!\"\n";
            player->makeSelfishChoice();
            setAsEnemy(true);
        }
        return;
    }
    else if (name == "Dark Spirit") {
        cout << "The dark spirit's voice slithers into your mind. What do you do?\n";
        cout << "1. Reject its offer\n";
        cout << "2. Listen to learn more\n";
        cout << "3. Embrace the darkness\n";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            cout << "You steel your mind against the spirit's temptations.\n";
            cout << "\"You will regret spurning such power!\" it hisses as it fades back into the shadows.\n";
            player->makeAltruisticChoice();
        }
        else if (choice == "2") {
            cout << "You cautiously allow the spirit to continue...\n";
            cout << "\"The fragments themselves can be corrupted at the shrine,\" it purrs.\n";
            cout << "\"Their power twisted to serve only you. Think of the possibilities...\"\n";
            player->makeSelfishChoice();
        }
        else if (choice == "3") {
            cout << "You open yourself to the darkness, feeling it seep into your very being.\n";
            cout << "\"Excellent,\" the spirit whispers. \"The first step is taken.\"\n";
            cout << "\"Sacrifice at the dark shrine to seal your path to power.\"\n";
            player->makeSelfishChoice();
            player->makeSelfishChoice(); // Double impact for embracing darkness
        }
        return;
    }

    // Default dialogue if no special interaction is needed
    talk();
}

void NPC::handlePlayerInput(const std::string& input, Player* player) {
    // Check if the input matches any of our responses
    string lowerInput = input;
    transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    auto it = responses.find(lowerInput);
    if (it != responses.end()) {
        cout << name << " says: \"" << it->second << "\"" << endl;

        // If this is an important response, potentially affect alignment
        if (lowerInput.find("help") != string::npos ||
            lowerInput.find("assist") != string::npos ||
            lowerInput.find("save") != string::npos) {
            player->makeAltruisticChoice();
        }
        else if (lowerInput.find("threat") != string::npos ||
            lowerInput.find("lie") != string::npos ||
            lowerInput.find("steal") != string::npos) {
            player->makeSelfishChoice();
        }
    }
    else {
        cout << name << " doesn't understand what you're asking about." << endl;
    }
}