// Player.cpp
#include "Player.h"
#include "Room.h"
#include "Creature.h"
#include "Entity.h"
#include "Exit.h"
#include "NPC.h"
#include "GameEnums.h"
#include "Item.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& name, const std::string& description, Room* room) :
    Creature(EntityType::PLAYER, name, description, room),
    lanternTurnsRemaining(0),
    movesTaken(0),
    moralAlignment(0),
    hasBetrayedNPCs(false),
    hasSacrificed(false) {
    setHealth(100);
}

// Movement and Location Methods
bool Player::moveTo(Direction direction) {
    if (location == nullptr) {
        std::cout << "You are nowhere!" << std::endl;
        return false;
    }

    Entity* exitEntity = location->getExit(direction);
    if (exitEntity == nullptr) {
        std::cout << "You can't go that way." << std::endl;
        return false;
    }

    if (exitEntity->getType() != EntityType::EXIT) {
        std::cout << "That's not a valid exit." << std::endl;
        return false;
    }

    Exit* exit = dynamic_cast<Exit*>(exitEntity);
    if (exit == nullptr) {
        std::cout << "Something strange happened with that exit." << std::endl;
        return false;
    }

    if (exit->isLocked()) {
        std::cout << "The " << exit->getName() << " is locked. You need a "
            << exit->getKeyName() << " to open it." << std::endl;
        return false;
    }

    Room* destination = exit->getDestination();
    if (destination == nullptr) {
        std::cout << "The exit leads nowhere!" << std::endl;
        return false;
    }

    std::cout << "You go " << directionToString(direction) << " to "
        << destination->getName() << "." << std::endl;

    setLocation(destination);

    if (destination->getIsDark() && !hasActiveLantern()) {
        std::cout << "\nYou step into pitch darkness. You can't see a thing!\n";
        std::cout << "You should use your lantern if you have one, or flee immediately.\n";
        std::cout << "You sense movement in the shadows around you...\n";
    }
    else {
        destination->look();
    }

    return true;
}

// Inventory Management Methods
bool Player::takeItem(const std::string& itemName) {
    if (location == nullptr) return false;

    if (location->getIsDark() && !hasActiveLantern()) {
        std::cout << "You blindly grope around in the darkness, but can't find anything.\n";
        std::cout << "Using your lantern would be much safer than fumbling in the dark.\n";
        return false;
    }

    Entity* entity = location->findEntity(itemName);
    if (entity && entity->getType() == EntityType::ITEM) {
        Item* item = dynamic_cast<Item*>(entity);

        // Check if the item is fixed in place
        if (item->getIsFixedInPlace()) {
            std::cout << "The " << item->getName() << " is firmly fixed in place and cannot be moved." << std::endl;
            return false;
        }

        if (canCarryMoreItems()) {
            location->removeEntity(entity);
            inventory.push_back(entity);
            std::cout << "You took the " << entity->getName() << "." << std::endl;
            return true;
        }
        else {
            std::cout << "You can't carry more items right now." << std::endl;
            return false;
        }
    }

    std::cout << "You don't see ";
    if (itemName.find(' ') != std::string::npos) {
        std::cout << "\"" << itemName << "\"";
    }
    else {
        std::cout << itemName;
    }
    std::cout << " here." << std::endl;

    if (!location->getIsDark() || hasActiveLantern()) {
        std::cout << "You see: ";
        bool first = true;
        for (auto entity : location->getContains()) {
            if (entity->getType() == EntityType::ITEM) {
                if (!first) std::cout << ", ";
                std::cout << entity->getName();
                first = false;
            }
        }

        if (first) {
            std::cout << "no items";
        }
        std::cout << std::endl;
    }

    return false;
}

bool Player::dropItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == itemName) {
            if (itemName == "lantern" && dynamic_cast<Item*>(*it)->getIsLit() &&
                location && location->getIsDark()) {
                std::cout << "You hesitate to drop your lit lantern in this darkness.\n";
                std::cout << "That would leave you vulnerable to whatever lurks here.\n";

                std::cout << "Are you sure? (y/n): ";
                std::string response;
                std::getline(std::cin, response);
                std::transform(response.begin(), response.end(), response.begin(), ::tolower);

                if (response != "y" && response != "yes") {
                    std::cout << "Wise decision. You keep the lantern.\n";
                    return false;
                }

                std::cout << "You drop the lantern. Its light continues to illuminate the area...\n";
                std::cout << "...but you realize that would be a terrible idea. You pick it back up.\n";
                return false;
            }

            if (location != nullptr) {
                location->addEntity(*it);
            }
            inventory.erase(it);
            std::cout << "You dropped the " << itemName << "." << std::endl;
            return true;
        }
    }

    std::cout << "You don't have a " << itemName << "." << std::endl;
    return false;
}

void Player::showInventory() const {
    if (inventory.empty()) {
        std::cout << "You're not carrying anything." << std::endl;
        return;
    }

    std::cout << "You're carrying:" << std::endl;
    for (auto item : inventory) {
        if (item->getName() == "lantern") {
            Item* lantern = dynamic_cast<Item*>(item);
            std::cout << "- " << item->getName();
            if (lantern && lantern->getIsLit()) {
                std::cout << " (lit, " << lanternTurnsRemaining << " turns remaining)";
            }
            else {
                std::cout << " (unlit)";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "- " << item->getName() << std::endl;
        }
    }
}

bool Player::hasItem(const std::string& itemName) const {
    std::string lowerInput = itemName;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    for (auto item : inventory) {
        std::string lowerItemName = item->getName();
        std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

        if (lowerItemName == lowerInput) {
            return true;
        }
    }
    return false;
}

void Player::addItem(Entity* item) {
    inventory.push_back(item);
}

bool Player::removeItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == itemName) {
            inventory.erase(it);
            return true;
        }
    }
    return false;
}

bool Player::hasBackpack() const {
    for (auto item : inventory) {
        if (item->getName() == "backpack") {
            return true;
        }
    }
    return false;
}

bool Player::canCarryMoreItems() const {
    if (!hasBackpack()) {
        return inventory.size() < 2;
    }
    return inventory.size() < 10;
}

// Item Interaction Methods
bool Player::useItem(const std::string& itemName) {
    Entity* itemToUse = nullptr;
    std::string lowerInput = itemName;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    for (auto item : inventory) {
        std::string lowerItemName = item->getName();
        std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

        if (lowerItemName == lowerInput) {
            itemToUse = item;
            break;
        }
    }

    if (!itemToUse) {
        int bestMatchLength = 0;
        for (auto item : inventory) {
            string lowerItemName = item->getName();
            transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

            if (lowerItemName.find(lowerInput) != string::npos) {
                // Prefer the item with the closest name length
                if (itemToUse == nullptr || abs(static_cast<int>(lowerItemName.length()) - static_cast<int>(lowerInput.length()))
                    < bestMatchLength) {
                    itemToUse = item;
                    bestMatchLength = abs(static_cast<int>(lowerItemName.length()) - static_cast<int>(lowerInput.length()));
                }
            }
        }
    }

    // Lantern handling
    if (lowerInput == "lantern" || (itemToUse && itemToUse->getName() == "lantern")) {
        Item* lantern = nullptr;

        if (!itemToUse || itemToUse->getName() != "lantern") {
            for (auto item : inventory) {
                if (item->getName() == "lantern") {
                    lantern = dynamic_cast<Item*>(item);
                    break;
                }
            }
        }
        else {
            lantern = dynamic_cast<Item*>(itemToUse);
        }

        if (lantern) {
            if (lantern->getIsLit()) {
                std::cout << "The lantern is already lit.\n";

                if (lanternTurnsRemaining > 0) {
                    std::cout << "It will last for " << lanternTurnsRemaining << " more turns.\n";
                }
                else {
                    std::cout << "But it seems to be out of fuel.\n";
                }
            }
            else {
                lantern->setLit(true);
                lanternTurnsRemaining = 10;

                if (location->getIsDark()) {
                    std::cout << "The lantern flames to life, its golden light pushing back the darkness!\n";
                    std::cout << "Shadows flee to the corners as you can now see clearly.\n";
                    std::cout << "The lantern will last for " << lanternTurnsRemaining << " turns.\n";
                    location->look();
                }
                else {
                    std::cout << "You light the lantern. It will last for about " << lanternTurnsRemaining << " turns.\n";
                }
            }
            return true;
        }
        else {
            std::cout << "You don't have a lantern.\n";
            return false;
        }
    }

    if (!itemToUse) {
        std::cout << "You don't have '" << itemName << "'.";
        if (!inventory.empty()) {
            std::cout << " You have: ";
            for (auto item : inventory) {
                std::cout << item->getName();
                if (item != inventory.back()) {
                    std::cout << ", ";
                }
            }
        }
        std::cout << std::endl;
        return false;
    }

    // Check for locked exits
    Room* currentRoom = getLocation();
    if (currentRoom) {
        for (auto exitPair : currentRoom->getExits()) {
            Entity* exitEntity = exitPair.second;
            if (exitEntity->getType() == EntityType::EXIT) {
                Exit* exit = dynamic_cast<Exit*>(exitEntity);
                if (exit && exit->isLocked()) {
                    if (itemToUse->nameMatches(exit->getKeyName())) {
                        if (currentRoom->getIsDark() && !hasActiveLantern()) {
                            std::cout << "You fumble with the " << itemToUse->getName() << " in the darkness,\n";
                            std::cout << "but can't find the keyhole. You need light to unlock doors here.\n";
                            return false;
                        }

                        if (exit->unlock(itemToUse->getName())) {
                            std::cout << "You use the " << itemToUse->getName()
                                << " to unlock the " << exit->getName() << "." << std::endl;
                            return true;
                        }
                    }
                }
            }
        }
    }

    // Potion handling
    if (itemToUse->getName() == "potion") {
        int previousHealth = getHealth();
        heal(25);
        std::cout << "You drink the potion. A surge of warmth floods your veins, healing your wounds.\n";
        std::cout << "Health restored: " << getHealth() - previousHealth << " points. ";
        std::cout << "Current health: " << getHealth() << "/" << getMaxHealth() << std::endl;

        removeItem(itemToUse->getName());
        return true;
    }

    std::cout << "You're not sure how to use the " << itemToUse->getName() << " here." << std::endl;
    return false;
}

bool Player::combineItems(const std::string& item1, const std::string& item2) {
    std::cout << "Nothing happens when you try to combine " << item1 << " and " << item2 << "." << std::endl;
    return false;
}

// Amulet Fragment Methods
bool Player::hasAmuletFragment(const std::string& fragmentName) const {
    for (auto item : inventory) {
        if (item->getName() == fragmentName) {
            return true;
        }
    }
    return false;
}

bool Player::combineAmuletFragments() {
    if (getLocation()->getName() != "Ruined Temple") {
        int fragmentCount = 0;
        if (hasAmuletFragment("amethyst")) fragmentCount++;
        if (hasAmuletFragment("sapphire")) fragmentCount++;
        if (hasAmuletFragment("ruby")) fragmentCount++;

        if (fragmentCount > 0) {
            std::cout << "\nThe " << (fragmentCount == 1 ? "fragment" : "fragments")
                << " in your inventory pulse weakly but nothing happens.\n";
            std::cout << "You sense they need to be brought to the Ruined Temple to be combined.\n";

            std::cout << "You have ";
            bool first = true;
            if (hasAmuletFragment("amethyst")) {
                std::cout << "amethyst";
                first = false;
            }
            if (hasAmuletFragment("sapphire")) {
                if (!first) std::cout << ", ";
                std::cout << "sapphire";
                first = false;
            }
            if (hasAmuletFragment("ruby")) {
                if (!first) std::cout << ", ";
                std::cout << "ruby";
            }
            std::cout << " fragment" << (fragmentCount > 1 ? "s" : "") << ".\n";
        }
        else {
            std::cout << "You don't have any amulet fragments to combine.\n";
        }
        return false;
    }

    bool onAltar = false;
    for (auto entity : getLocation()->getContains()) {
        if (entity->getName() == "forge") {
            onAltar = true;
            break;
        }
    }

    if (!onAltar) {
        std::cout << "\nThe fragments vibrate intensely but refuse to combine.\n";
        std::cout << "You noticed an ancient forge in the temple - perhaps they need to be placed there?\n";
        return false;
    }

    bool hasAll = hasAmuletFragment("amethyst") &&
        hasAmuletFragment("sapphire") &&
        hasAmuletFragment("ruby");

    if (!hasAll) {
        std::cout << "\nYou stand before the altar with ";
        int fragmentCount = 0;
        if (hasAmuletFragment("amethyst")) fragmentCount++;
        if (hasAmuletFragment("sapphire")) fragmentCount++;
        if (hasAmuletFragment("ruby")) fragmentCount++;

        std::cout << "only " << fragmentCount << " fragment" << (fragmentCount > 1 ? "s" : "") << ".\n";
        std::cout << "The altar's three depressions mock you with their emptiness.\n";

        std::cout << "You're missing: ";
        bool first = true;
        if (!hasAmuletFragment("amethyst")) {
            std::cout << "amethyst";
            first = false;
        }
        if (!hasAmuletFragment("sapphire")) {
            if (!first) std::cout << ", ";
            std::cout << "sapphire";
            first = false;
        }
        if (!hasAmuletFragment("ruby")) {
            if (!first) std::cout << ", ";
            std::cout << "ruby";
        }
        std::cout << ".\n";
        return false;
    }

    std::cout << "\nAs you place the three fragments in the forge:\n";
    std::cout << "1. The amethyst begins humming at a piercing frequency\n";
    std::cout << "2. The sapphire emits a wave of comforting warmth\n";
    std::cout << "3. The ruby burns with sudden intensity\n\n";
    std::cout << "The fragments rise into the air, spinning rapidly as arcane energy connects them!\n";
    std::cout << "With a blinding flash and thunderous BOOM, the Amulet of Eldoria is restored!\n";

    removeItem("amethyst");
    removeItem("sapphire");
    removeItem("ruby");

    Item* amulet = new Item("Amulet of Eldoria",
        "The fully restored artifact thrums with primordial energy.\n"
        "You feel its power resonating with your very soul.",
        false, 0, false);
    addItem(amulet);

    return true;
}

bool Player::placeAmuletOnAltar() {
    if (getLocation()->getName() != "Sorcerer's Tower") {
        if (hasItem("Amulet of Eldoria")) {
            std::cout << "\nThe amulet pulses powerfully but nothing happens.\n";
            std::cout << "You sense it must be placed in the Sorcerer's Tower to break the curse.\n";
        }
        return false;
    }

    bool altarExists = false;
    for (auto entity : getLocation()->getContains()) {
        if (entity->getName() == "altar") {
            altarExists = true;
            break;
        }
    }

    if (!altarExists) {
        std::cout << "There's no altar here to place the amulet on!\n";
        return false;
    }

    if (!hasItem("Amulet of Eldoria")) {
        std::cout << "You need the restored Amulet of Eldoria to break the curse.\n";
        return false;
    }

    std::cout << "\nAs you approach the altar with the amulet, you feel immense power flowing through your veins.\n";
    std::cout << "The amulet grows warm in your hands, whispering temptations of unlimited power.\n\n";

    std::cout << "What will you choose?\n";
    std::cout << "1. Place the amulet on the altar to break the curse (Selfless)\n";
    std::cout << "2. Absorb the amulet's power for yourself (Selfish)\n";
    std::cout << "3. Destroy the amulet forever (Neutral)\n";
    std::cout << "4. Hesitate and reconsider your options\n";
    std::cout << "Choose (1-4): ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1") {
        std::cout << "\nWith trembling hands, you place the Amulet of Eldoria on the ancient altar...\n\n";
        std::cout << "A brilliant light erupts from the amulet, filling the room!\n";
        std::cout << "The tower shakes as dark energy swirls around you, drawn into the amulet.\n";

        if (getAlignment() > 3) {
            std::cout << "The amulet recognizes your pure heart and amplifies your noble intentions.\n";
            std::cout << "Not only is the curse broken, but the land begins to heal at an astonishing rate!\n";
            std::cout << "The villagers will speak of your heroism for generations to come.\n";
            std::cout << "*** PERFECT ENDING: LEGENDARY HERO ***\n";
        }
        else {
            std::cout << "The amulet's glow intensifies, absorbing all the cursed energy.\n\n";
            std::cout << "Suddenly - SILENCE.\n\n";
            std::cout << "The curse is broken! Eldoria is saved!\n";
            std::cout << "*** GOOD ENDING: SAVIOR OF ELDORIA ***\n";
        }
        return true;
    }
    else if (choice == "2") {
        std::cout << "\nYou clutch the amulet tightly, feeling its power coursing through you...\n";

        if (getAlignment() < -5 || hasBetrayed()) {
            std::cout << "The amulet recognizes the darkness in your heart and bonds with your corrupt soul.\n";
            std::cout << "Immense power floods your body as your eyes turn pitch black.\n";
            std::cout << "The villagers of Eldoria will serve their new dark master - YOU.\n";
            std::cout << "*** DARK ENDING: RISE OF A TYRANT ***\n";
        }
        else {
            std::cout << "You attempt to absorb the amulet's power, but it resists your will!\n";
            std::cout << "The power is too great for your unprepared body and mind.\n";
            std::cout << "The amulet shatters in your hands, releasing a blast that consumes you.\n";
            std::cout << "*** BAD ENDING: CONSUMED BY POWER ***\n";
            takeDamage(getHealth());
        }
        return true;
    }
    else if (choice == "3") {
        std::cout << "\nYou raise the amulet high and slam it down onto the altar with all your might.\n";
        std::cout << "The amulet shatters with a deafening crack!\n\n";

        if (getAlignment() > 0) {
            std::cout << "The magical energies disperse harmlessly, washing over the land.\n";
            std::cout << "The curse fades gradually, though Eldoria will never regain its former glory.\n";
            std::cout << "The age of magic in this realm has come to an end.\n";
            std::cout << "*** NEUTRAL ENDING: AGE OF BALANCE ***\n";
        }
        else {
            std::cout << "The magical energies explode outward violently!\n";
            std::cout << "The curse is broken, but much of Eldoria's natural magic is forever lost.\n";
            std::cout << "You survive, but wonder if your choice was truly the best one.\n";
            std::cout << "*** NEUTRAL ENDING: PYRRHIC VICTORY ***\n";
        }
        return true;
    }
    else {
        std::cout << "\nYou step back from the altar, the weight of your decision heavy on your shoulders.\n";
        std::cout << "Perhaps you need more time to consider your options...\n";
        return false;
    }
}

// Combat and Interaction Methods
bool Player::attackCreature(const std::string& creatureName) {
    if (location == nullptr) return false;

    Entity* target = location->findEntity(creatureName);
    if (!target || target->getType() != EntityType::NPC) {
        std::cout << "You can't attack that!\n";
        return false;
    }

    NPC* npc = dynamic_cast<NPC*>(target);
    std::cout << "You attack " << npc->getName() << "!\n";

    if (rand() % 2 == 0) {
        std::cout << "You defeat " << npc->getName() << "!\n";

        if (npc->getName() == "Blacksmith") {
            Item* rustyKey = new Item("rusty key", "An old rusted key that might open something.");
            addItem(rustyKey);
            std::cout << "You found a rusty key on the Blacksmith!\n";
            makeSelfishChoice();
        }
        else if (npc->getName() == "Hermit") {
            Item* fragment = new Item("sapphire", "Stolen fragment", true, 0, true);
            addItem(fragment);
            std::cout << "You found a sapphire fragment on the Hermit!\n";
            makeSelfishChoice();
            betrayNPCs();
        }

        location->removeEntity(npc);
        delete npc;
        return true;
    }
    else {
        std::cout << npc->getName() << " fights back!\n";
        takeDamage(30);
        return false;
    }
}

bool Player::stealItem(const std::string& itemName) {
    if (location == nullptr) return false;

    Entity* itemEntity = location->findEntity(itemName);

    if (!itemEntity) {
        if (itemName == "rusty key") {
            Entity* blacksmith = location->findEntity("Blacksmith");
            if (blacksmith && blacksmith->getType() == EntityType::NPC) {
                if (rand() % 10 < 6) {
                    std::cout << "You successfully steal the rusty key from the Blacksmith!\n";
                    Item* rustyKey = new Item("rusty key", "An old rusted key that might open something.");
                    addItem(rustyKey);
                    makeSelfishChoice();
                    return true;
                }
                else {
                    std::cout << "You got caught trying to steal from the Blacksmith!\n";
                    std::cout << "The Blacksmith glares at you. \"I saw that! Keep your hands to yourself!\"\n";
                    return false;
                }
            }
        }
        else if (itemName == "sapphire" || itemName == "sapphire fragment") {
            Entity* hermit = location->findEntity("Hermit");
            if (hermit && hermit->getType() == EntityType::NPC) {
                if (rand() % 10 < 5) {
                    std::cout << "You successfully steal the sapphire fragment from the Hermit!\n";
                    Item* fragment = new Item("sapphire", "Stolen fragment", true, 0, true);
                    addItem(fragment);
                    makeSelfishChoice();
                    betrayNPCs();
                    return true;
                }
                else {
                    std::cout << "The Hermit catches you trying to steal his precious fragment!\n";
                    std::cout << "\"Thief! You are not worthy of this power!\"\n";
                    return false;
                }
            }
        }
    }

    if (itemEntity && itemEntity->getType() == EntityType::ITEM) {
        if (rand() % 10 < 6) {
            std::cout << "You successfully steal the " << itemName << "!\n";
            makeSelfishChoice();
            return takeItem(itemName);
        }
        else {
            std::cout << "You got caught stealing!\n";
            std::cout << "The villagers look at you disapprovingly.\n";
            return false;
        }
    }

    std::cout << "You don't see a " << itemName << " that you can steal here.\n";
    return false;
}

void Player::sacrificeNPC(const std::string& npcName) {
    bool atDarkShrine = false;
    if (location->getName() == "Abandoned Mine") {
        for (auto entity : location->getContains()) {
            if (entity->getName() == "dark shrine") {
                atDarkShrine = true;
                break;
            }
        }
    }

    if (!atDarkShrine) {
        std::cout << "The dark ritual can only be performed at the shrine in the mine.\n";
        return;
    }

    Entity* entity = location->findEntity(npcName);
    if (!entity || entity->getType() != EntityType::NPC) {
        std::cout << "There is no " << npcName << " here to sacrifice.\n";
        return;
    }

    std::cout << "You perform a dark ritual, sacrificing " << npcName << " at the shrine!\n";
    std::cout << "Malevolent energy courses through you as " << npcName << "'s life force is consumed.\n";

    moralAlignment -= 10;
    hasSacrificed = true;

    maxHealth += 20;
    health = maxHealth;
    std::cout << "Your maximum health increases to " << maxHealth << "!\n";

    location->removeEntity(entity);
    delete entity;

    if (moralAlignment < -20) {
        std::cout << "\nThe darkness has fully claimed your soul. There may be no redemption for you now.\n";
    }
}

void Player::forgiveEnemy(const std::string& enemyName) {
    Entity* entity = location->findEntity(enemyName);
    if (!entity || entity->getType() != EntityType::NPC) {
        std::cout << "There is no " << enemyName << " here to forgive.\n";
        return;
    }

    NPC* npc = dynamic_cast<NPC*>(entity);
    if (!npc->isEnemy) {
        std::cout << enemyName << " has not wronged you. There is nothing to forgive.\n";
        return;
    }

    std::cout << "You choose to forgive " << enemyName << " despite their actions.\n";
    std::cout << "\"Your mercy is unexpected,\" " << enemyName << " says with genuine surprise.\n";

    moralAlignment += 5;
    npc->setAsEnemy(false);

    if (moralAlignment > 15) {
        std::cout << "\nYour compassion shines like a beacon in the darkness. The villagers whisper that you might be the one to break the curse.\n";
    }
}

bool Player::makeMoralChoice(int choiceType) {
    switch (choiceType) {
    case 1: // Help a villager
        std::cout << "\nYou stop to help a struggling villager with their task.\n";
        std::cout << "They are grateful and spread word of your kindness.\n";
        makeAltruisticChoice();
        return true;

    case 2: // Ignore someone in need
        std::cout << "\nYou ignore the pleas for help, focusing only on your quest.\n";
        std::cout << "The villagers whisper disapprovingly as you pass.\n";
        makeSelfishChoice();
        return true;

    case 3: // Share your supplies
        if (inventory.size() > 1) {
            std::cout << "\nYou share some of your supplies with the hungry refugees.\n";
            std::cout << "Their grateful smiles warm your heart.\n";
            makeAltruisticChoice();
            makeAltruisticChoice();
            for (auto it = inventory.begin(); it != inventory.end(); ++it) {
                if ((*it)->getName() != "Amulet of Eldoria") {
                    std::cout << "You give away your " << (*it)->getName() << ".\n";
                    delete* it;
                    inventory.erase(it);
                    break;
                }
            }
            return true;
        }
        else {
            std::cout << "\nYou would share, but you have nothing to give.\n";
            return false;
        }

    case 4: {
        std::cout << "\nYou demand 'donations' from the frightened villagers.\n";
        std::cout << "They reluctantly comply, fearing your wrath.\n";
        makeSelfishChoice();
        makeSelfishChoice();
        Item* stolenItem = new Item("bread", "Stolen food from the villagers.");
        addItem(stolenItem);
        std::cout << "You acquired some bread.\n";
        return true;
    }

    default:
        return false;
    }
}

bool Player::corruptArtifact(const std::string& artifactName) {
    if (!hasItem(artifactName)) {
        std::cout << "You don't have " << artifactName << " to corrupt.\n";
        return false;
    }

    std::cout << "\nYou channel dark energy into the " << artifactName << "...\n";
    std::cout << "It twists and transforms in your hands, becoming something sinister.\n";

    removeItem(artifactName);

    std::string corruptedName = "Corrupted " + artifactName;
    Item* corruptedItem = new Item(corruptedName,
        "A once-pure artifact, now pulsing with dark energy.");
    addItem(corruptedItem);

    std::cout << "The " << corruptedName << " now grants you additional power!\n";

    makeSelfishChoice();
    makeSelfishChoice();

    setHealth(getHealth() + 20);
    std::cout << "Your maximum health has increased. Health is now "
        << getHealth() << "/" << getMaxHealth() << ".\n";

    return true;
}

void Player::showAlignment() const {
    std::cout << "\nYour moral compass points ";

    if (moralAlignment > 5) {
        std::cout << "strongly toward selflessness and heroism.\n";
        std::cout << "The villagers look to you with hope and admiration.\n";
    }
    else if (moralAlignment > 2) {
        std::cout << "toward good and kindness.\n";
        std::cout << "People generally trust and respect you.\n";
    }
    else if (moralAlignment > -2) {
        std::cout << "neither strongly toward good nor evil.\n";
        std::cout << "People are uncertain of your true intentions.\n";
    }
    else if (moralAlignment > -5) {
        std::cout << "toward selfishness and opportunism.\n";
        std::cout << "People regard you with suspicion and fear.\n";
    }
    else {
        std::cout << "deeply into darkness and cruelty.\n";
        std::cout << "Villagers flee at the sight of you, whispering warnings of your approach.\n";
    }

    if (hasBetrayedNPCs) {
        std::cout << "You have a reputation for betrayal that precedes you.\n";
    }
}

// Utility Methods
bool Player::hasActiveLantern() const {
    for (auto item : inventory) {
        if (item->getName() == "lantern" && dynamic_cast<Item*>(item)->getIsLit()) {
            return lanternTurnsRemaining > 0;
        }
    }
    return false;
}

void Player::takeDamage(int amount) {
    Creature::takeDamage(amount);
    std::cout << "\n*** You took " << amount << " damage! Health: "
        << getHealth() << "/" << getMaxHealth() << " ***\n";

    if (getHealth() <= 25 && getHealth() > 10) {
        std::cout << "You're badly wounded! Find healing quickly!\n";
    }
    else if (getHealth() <= 10) {
        std::cout << "You're critically injured! One more hit could be fatal!\n";
    }
}