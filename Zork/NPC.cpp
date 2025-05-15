#include "NPC.h"
#include "Player.h"
#include "Room.h"
#include "Item.h"
#include <iostream>
#include <algorithm>

NPC::NPC(const std::string& name, const std::string& description, Room* room) :
    Creature(EntityType::NPC, name, description, room),
    hasGivenReward(false),
    trusts(true),
    hasImportantInfo(false),
    isEnemy(false),
    hasInteracted(false),
    preventReinteraction(false) {
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

void NPC::setPreventReinteraction(bool prevent) {
    preventReinteraction = prevent;
}

bool NPC::hasPlayerInteracted() const {
    return hasInteracted;
}

void NPC::setHasInteracted(bool interacted) {
    hasInteracted = interacted;
}

void NPC::talk() const {
    if (dialogues.empty()) {
        std::cout << name << " has nothing to say." << std::endl;
        return;
    }

    // Display ALL dialogue lines
    std::cout << name << " says:" << std::endl;
    for (const auto& line : dialogues) {
        std::cout << "\"" << line << "\"" << std::endl;
    }

    // If there are specific responses, give a hint
    if (!responses.empty()) {
        std::cout << "You could ask about: ";
        bool first = true;
        for (const auto& pair : responses) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << pair.first;
            first = false;
        }
        std::cout << std::endl;
    }
}

void NPC::handlePlayerInput(const std::string& input, Player* player) {
    // Convert input to lowercase for case-insensitive comparison
    std::string lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    auto it = responses.find(lowerInput);
    if (it != responses.end()) {
        std::cout << name << " says: \"" << it->second << "\"" << std::endl;

        // Potentially affect alignment based on input content
        if (lowerInput.find("help") != std::string::npos ||
            lowerInput.find("assist") != std::string::npos ||
            lowerInput.find("save") != std::string::npos) {
            player->makeAltruisticChoice();
        }
        else if (lowerInput.find("threat") != std::string::npos ||
            lowerInput.find("lie") != std::string::npos ||
            lowerInput.find("steal") != std::string::npos) {
            player->makeSelfishChoice();
        }
    }
    else {
        std::cout << name << " doesn't understand what you're asking about." << std::endl;
    }
}

void NPC::interact(Player* player) {
    // Check if NPC prevents reinteraction and has already interacted
    if (hasInteracted && preventReinteraction) {
        std::cout << name << " has nothing more to say to you." << std::endl;
        return;
    }

    // Mark that interaction has occurred
    hasInteracted = true;

    // Trust and alignment check
    if (player->getAlignment() < -3 || player->hasBetrayed()) {
        if (trusts) {
            // First time mistrust - NPC becomes cautious
            std::cout << name << " eyes you warily...\n";

            if (player->hasBetrayed()) {
                std::cout << "\"I've heard about your betrayals, " << player->getName() << ". ";
                std::cout << "Why should I trust you?\"\n";
            }
            else {
                std::cout << "\"There's a darkness about you that I don't trust, " << player->getName() << ".\"\n";
            }

            trusts = false;

            // If enemy NPC, they become more aggressive
            if (isEnemy) {
                std::cout << name << " reaches for their weapon. \"Stay back!\"\n";
                return;
            }

            // If NPC has important info, they hide it
            if (hasImportantInfo) {
                std::cout << "\"I don't think I should tell you what I know.\"\n";
                return;
            }
        }
        else {
            // Already mistrusting
            std::cout << name << " refuses to speak with you further.\n";
            std::cout << "\"Leave me be, " << player->getName() << ".\"\n";
            return;
        }
    }
    else if (!trusts && player->getAlignment() > 2) {
        // Redemption - NPC begins to trust again
        std::cout << name << " seems to relax slightly around you.\n";
        std::cout << "\"Perhaps I misjudged you, " << player->getName() << ".\"\n";
        trusts = true;
    }

    // Item trading interaction
    if (!requiredItem.empty() && !hasGivenReward) {
        // Check if player has the required item
        if (player->hasItem(requiredItem)) {
            std::cout << name << " says: \"I see you have " << requiredItem
                << ". Would you like to give it to me?\" (yes/no)" << std::endl;

            // Get player response
            std::string response;
            std::getline(std::cin, response);
            std::transform(response.begin(), response.end(), response.begin(), ::tolower);

            if (response == "yes" || response == "y") {
                if (player->removeItem(requiredItem)) {
                    // Successfully gave item
                    std::cout << "You give " << requiredItem << " to " << name << "." << std::endl;

                    // Improve alignment for helping
                    player->makeAltruisticChoice();

                    // Give reward if specified
                    if (!rewardItem.empty()) {
                        // Check if player can carry more items
                        if (player->canCarryMoreItems()) {
                            Item* reward = new Item(rewardItem, "A reward from " + name);
                            player->addItem(reward);
                            std::cout << name << " gives you " << rewardItem << " in return." << std::endl;
                        }
                        else {
                            std::cout << name << " tries to give you " << rewardItem
                                << ", but you can't carry it!" << std::endl;
                            // drop the reward in the room if invenotry is full
                            if (location) {
                                Item* reward = new Item(rewardItem, "A reward from " + name);
                                location->addEntity(reward);
                                std::cout << rewardItem << " falls to the ground." << std::endl;
                            }
                        }
                    }

                    // Show additional dialogue if available
                    if (dialogues.size() > 1) {
                        std::cout << name << " says: \"" << dialogues[1] << "\"" << std::endl;
                    }

                    hasGivenReward = true;
                    return;
                }
            }
            else {
                std::cout << "You decide to keep " << requiredItem << "." << std::endl;
                return;
            }
        }
        else {
            // Player doesn't have the required item
            std::cout << name << " says: \"Bring me " << requiredItem
                << " and I'll help you.\"" << std::endl;
            if (dialogues.size() > 2) {  // Show hint if available
                std::cout << name << " adds: \"" << dialogues[2] << "\"" << std::endl;
            }
            return;
        }
    }

    // Special interaction for NPCs with important information
    if (hasImportantInfo && trusts) {
        std::cout << name << " leans in closer and whispers:\n";
        if (dialogues.size() > 3) {
            std::cout << "\"" << dialogues[3] << "\"\n";
        }
        else {
            std::cout << "\"I've heard rumors of an amulet fragment hidden in a nearby cave.\"\n";
        }

        // Choice to pursue this info or not
        std::cout << "\nYou could:\n";
        std::cout << "1. Thank them for the information\n";
        std::cout << "2. Ask for more details\n";
        std::cout << "3. Ignore this seemingly useless gossip\n";
        std::cout << "Choose (1-3): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "You thank " << name << " for the valuable information.\n";
            player->makeAltruisticChoice();
        }
        else if (choice == "2") {
            std::cout << "You eagerly ask for more details.\n";
            std::cout << name << " shares everything they know about the rumor.\n";
            if (dialogues.size() > 4) {
                std::cout << name << ": \"" << dialogues[4] << "\"\n";
            }
            player->makeAltruisticChoice();
        }
        else if (choice == "3") {
            std::cout << "You dismiss " << name << "'s information with a shrug.\n";
            std::cout << name << " looks hurt by your indifference.\n";
            player->makeSelfishChoice();
            trusts = false;
        }
        return;
    }

    // Special NPC-specific interactions
    if (name == "Blacksmith" && !hasGivenReward) {
        if (player->hasItem("bread")) {
            std::cout << name << " eyes the bread hungrily. \"I'll trade my rusty key for that loaf.\" (yes/no)\n";
            std::string response;
            std::getline(std::cin, response);
            std::transform(response.begin(), response.end(), response.begin(), ::tolower);

            if (response == "yes" || response == "y") {
                if (player->removeItem("bread")) {
                    std::cout << "You trade the bread for the rusty key.\n";
                    std::cout << "The blacksmith tears into the loaf. \"This mine key is yours now. Be careful down there.\"\n";

                    Item* key = new Item("rusty key", "An old iron key that opens the mine entrance");
                    player->addItem(key);
                    player->makeAltruisticChoice();
                    hasGivenReward = true;
                    return;
                }
            }
            else {
                std::cout << "You decide to keep your bread for now.\n";
                return;
            }
        }
        else {
            std::cout << "\"Bring me some bread if you want the mine key. A man's got to eat.\"\n";
            return;
        }
    }
    else if (name == "Hermit" && !hasGivenReward && player->hasItem("potion")) {
        std::cout << "The hermit looks at your potion with desperate eyes.\n";
        std::cout << "\"That elixir would ease my suffering greatly. Will you share it?\" (yes/no)\n";

        std::string response;
        std::getline(std::cin, response);
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);

        if (response == "yes" || response == "y") {
            if (player->removeItem("potion")) {
                std::cout << "You give the potion to the hermit.\n";
                std::cout << "He drinks it and his breathing eases. \"Thank you, kind soul.\"\n";
                std::cout << "\"The lantern you found will protect you in the mine's darkness. The shadows flee from its light.\"\n";
                std::cout << "\"But beware - its oil won't last forever. Make haste when in dark places.\"\n";

                Item* scroll = new Item("scroll", "Ancient parchment with instructions for combining the amulet fragments");
                player->addItem(scroll);
                player->makeAltruisticChoice();
                hasGivenReward = true;
                return;
            }
        }
    }
    else if (name == "Dark Spirit") {
        std::cout << "The dark spirit's voice slithers into your mind. What do you do?\n";
        std::cout << "1. Reject its offer\n";
        std::cout << "2. Listen to learn more\n";
        std::cout << "3. Embrace the darkness\n";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "You steel your mind against the spirit's temptations.\n";
            std::cout << "\"You will regret spurning such power!\" it hisses as it fades back into the shadows.\n";
            player->makeAltruisticChoice();

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "2") {
            std::cout << "You cautiously allow the spirit to continue...\n";
            std::cout << "\"The fragments themselves can be corrupted at the shrine,\" it purrs.\n";
            std::cout << "\"Their power twisted to serve only you. Think of the possibilities...\"\n";
            player->makeSelfishChoice();

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "3") {
            std::cout << "You open yourself to the darkness, feeling it seep into your very being.\n";
            std::cout << "\"Excellent,\" the spirit whispers. \"The first step is taken.\"\n";
            std::cout << "\"Sacrifice at the dark shrine to seal your path to power.\"\n";
            player->makeSelfishChoice();
            player->makeSelfishChoice(); // Double impact for embracing darkness

            hasInteracted = true;
            preventReinteraction = true;
        }
        return;
    }
    else if (name == "Elder") {
        std::cout << "The village elder looks at you with hopeful eyes.\n";
        std::cout << "1. Share some of your supplies\n";
        std::cout << "2. Ignore the elder's request\n";
        std::cout << "3. Demand payment for your help\n";
        std::cout << "Choose (1-3): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "You offer some of your supplies to help the villagers.\n";
            std::cout << "\"Bless you, traveler. Your kindness brings light to our darkest hour.\"\n";
            player->makeAltruisticChoice();
            player->makeAltruisticChoice(); // Double impact for significant kindness

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "2") {
            std::cout << "You tell the elder you need to focus on your quest first.\n";
            std::cout << "\"I see. Another who cares only for themselves. May you find what you seek, though it brings you no joy.\"\n";
            // No alignment change - neutral choice

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "3") {
            std::cout << "You demand payment for your services despite their desperate situation.\n";
            std::cout << "\"Even in these desperate times, there are those who would profit from suffering.\"\n";
            std::cout << "The elder reluctantly hands you a few coins. \"It's all we can spare.\"\n";
            player->makeSelfishChoice();

            hasInteracted = true;
            preventReinteraction = true;
        }
        return;
    }
    else if (name == "Bandit") {
        std::cout << "The desperate bandit stands before you, knife trembling in his hand.\n";
        std::cout << "1. Attack the bandit\n";
        std::cout << "2. Forgive and help him\n";
        std::cout << "3. Threaten and rob him instead\n";
        std::cout << "Choose (1-3): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "You draw your weapon as the bandit readies for combat!\n";
            std::cout << "\"I won't go down without a fight!\"\n";
            player->makeSelfishChoice();

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "2") {
            std::cout << "You lower your guard and offer to help the bandit and his family.\n";
            std::cout << "\"You... would help me? After I threatened you?\" Tears form in his eyes.\n";
            std::cout << "\"I won't forget this mercy. Take this - I found it in the forest.\"\n";

            Item* herbalMix = new Item("herbal mix", "A potent mixture of medicinal herbs");
            player->addItem(herbalMix);
            player->makeAltruisticChoice();
            player->makeAltruisticChoice(); // Double positive impact

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "3") {
            std::cout << "You turn the tables and threaten the bandit with your superior weapons.\n";
            std::cout << "\"P-please! Don't take everything! My children will starve!\"\n";
            std::cout << "You take his meager belongings anyway.\n";

            Item* smallPouch = new Item("small pouch", "A pouch containing a few coins");
            player->addItem(smallPouch);
            player->makeSelfishChoice();
            player->makeSelfishChoice(); // Double negative impact

            hasInteracted = true;
            preventReinteraction = true;
        }
        return;
    }
    else if (name == "Corrupted Villager") {
        std::cout << "The corrupted villager stumbles toward you, twisted by the curse.\n";
        std::cout << "Their humanity seems to be fighting against the corruption.\n";
        std::cout << "1. Try to save them with herbs\n";
        std::cout << "2. End their suffering mercifully\n";
        std::cout << "3. Sacrifice their corrupted essence for power\n";
        std::cout << "Choose (1-3): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            if (player->hasItem("herbs") || player->hasItem("herbal mix")) {
                std::string herbItem = player->hasItem("herbal mix") ? "herbal mix" : "herbs";

                std::cout << "You attempt to administer " << herbItem << " to calm the corrupted villager...\n";
                player->removeItem(herbItem);

                std::cout << "The herbs take effect, and the darkness begins to recede from their eyes.\n";
                std::cout << "\"T-thank you,\" they stammer. \"I was lost... but you brought me back.\"\n";
                std::cout << "They hand you a small trinket. \"This may help you on your journey.\"\n";

                Item* amuletShard = new Item("amulet shard", "A tiny fragment that seems to resonate with the larger amulet pieces");
                player->addItem(amuletShard);
                player->makeAltruisticChoice();
                player->makeAltruisticChoice(); // Double positive impact
            }
            else {
                std::cout << "You try to help, but without herbs, there's little you can do.\n";
                std::cout << "The villager lunges at you, fully consumed by the darkness!\n";
            }

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "2") {
            std::cout << "With a heavy heart, you end the villager's suffering quickly and painlessly.\n";
            std::cout << "It was the only humane choice. Their twisted features relax in final peace.\n";
            // No alignment change - this was a mercy

            hasInteracted = true;
            preventReinteraction = true;
        }
        else if (choice == "3") {
            std::cout << "You begin a dark ritual, drawing the corrupted essence from the villager...\n";
            std::cout << "You have made your choice, complete the ritual by typing 'Sacrifice villager'\n";

            // Give player a power but decrease alignment significantly
            player->makeSelfishChoice();
            player->makeSelfishChoice();
            player->makeSelfishChoice(); // Triple negative impact

            hasInteracted = true;
            preventReinteraction = true;
        }
        return;
    }

    // Default dialogue if no special interaction is needed
    talk();
}