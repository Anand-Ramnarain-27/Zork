#include "World.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "NPC.h"
#include <iostream>
#include <vector>

// Static room pointers
static Room* village;
static Room* forest;
static Room* mine;
static Room* temple;
static Room* tower;

void World::InitializeWorld() {
    // Create all rooms
    village = new Room("Village of Eldoria",
        "The peaceful starting village.Wooden cottages with smoking chimneys line the dirt paths. \
        The villagers glance at you nervously, whispering about the curse.To the north, a path leads \
        to the Enchanted Forest.A locked hatch in the town square descends into darkness.");

    forest = new Room("Enchanted Forest",
        "Ancient trees tower above you, their leaves glowing faintly with bioluminescent fungi. \
         Strange whispers dance on the wind. A shadowy figure (the hermit?) watches from between \
         the trees. The path back south leads to the village, while an overgrown trail winds east.");

    mine = new Room("Abandoned Mine",
        "Pitch-black darkness swallows everything. You hear skittering noises...", true);

    temple = new Room("Ruined Temple",
        "Crumbling stone pillars surround a central altar carved with three gem-shaped depressions. \
         Faded murals depict the amulet's destruction. A ghostly priestess drifts near the staircase, \
         which is barred by an ethereal lock. The forest lies west.");

    tower = new Room("Sorcerer's Tower",
        "The air hums with dark energy. A cracked obsidian altar dominates the room, pulsing with \
         malevolent light. The restored amulet could break the curse... if placed here. The staircase \
         descends back to the temple.");

    // ===== CREATE EXITS =====
    // Village exits
    new Exit(Direction::NORTH, village, forest, "forest path", "A well-trodden path leading into the forest");
    new Exit(Direction::DOWN, village, mine, "mine entrance", "A wooden hatch leading down to the mines", true, "rusty key");

    // Forest exits
    new Exit(Direction::SOUTH, forest, village, "village path", "The path back to the village");
    new Exit(Direction::EAST, forest, temple, "overgrown trail", "A barely visible trail through thick bushes");

    // Temple exits
    new Exit(Direction::WEST, temple, forest, "forest trail", "The trail back to the forest");
    new Exit(Direction::UP, temple, tower, "spiral staircase", "An ancient stone staircase winding upward", true, "golden key");

    // Mine exits
    new Exit(Direction::UP, mine, village, "mine exit", "The way back up to the village");

    // Tower exits
    new Exit(Direction::DOWN, tower, temple, "stone steps", "The steps leading back down to the temple");

    // ===== ADD ITEMS =====
    // Village items
    village->addEntity(new Item("bread", "A loaf of stale bread", false, 0, false));
    village->addEntity(new Item("map", "A map of Eldoria", false, 0, false));

    // Special container item
    Item* backpack = new Item("backpack", "A weathered leather pack with a note tucked inside. It can hold 5 items.", true, 5, false);
    village->addEntity(backpack);
    backpack->addEntity(new Item("note", "A note: 'Find the hermit in the forest'", false, 0, false));

    // Forest items
    forest->addEntity(new Item("lantern", "A brass lantern with ever-burning oil. Its light repels shadow creatures.", false, 0, false));
    forest->addEntity(new Item("potion", "A crimson elixir that smells of elderberries. The hermit covets this.", false, 0, false));
    forest->addEntity(new Item("herbs", "Medicinal forest herbs", false, 0, false));

    // Mine items
    mine->addEntity(new Item("pickaxe", "A rusty but usable pickaxe", false, 0, false));
    mine->addEntity(new Item("amethyst", "A jagged purple shard humming with arcane energy. It feels warm to the touch.", true, 0, true)); // Amulet fragment 1/3
    mine->setDark(true);
    mine->addEntity(new Item("dark shrine",
        "A sinister obsidian structure pulsing with malevolent energy. Blood stains its base.",
        false, 0, false));

    // Temple items
    temple->addEntity(new Item("golden key", "Ornate and cold to the touch. The priestess might know its purpose.", false, 0, false));
    temple->addEntity(new Item("sapphire", "A smooth blue fragment encased in ice that never melts. It whispers when held.", true, 0, true)); // Amulet fragment 2/3
    temple->addEntity(new Item("altar",
        "An ancient stone altar with mystical engravings. It has three depressions shaped like gems.",
        false, 0, false));

    // Tower items
    tower->addEntity(new Item("ruby", "A blood-red sliver that burns like embers. The curse's power resonates within it.", true, 0, true)); // Amulet fragment 3/3
    tower->addEntity(new Item("altar",
        "The cursed altar that started it all. A perfect amulet-shaped depression "
        "glows faintly in its center, waiting for the restored artifact.",
        false, 0, false));
    tower->addEntity(new Item("ancient relic",
        "A mysterious artifact vibrating with untapped power. It could be used for good or evil.",
        true, 0, true));

    // ===== ADD NPCs =====
    // Village NPC - Blacksmith
    NPC* blacksmith = new NPC("Blacksmith", "A burly man with soot-covered arms", village);
    blacksmith->addDialogue("Welcome to Eldoria, traveler.");
    blacksmith->addDialogue("That mine's been locked since the curse began.");
    blacksmith->addDialogue("No one dares enter... but I'll trade the key for bread.");
    blacksmith->addDialogue("I'll give you the rusty key if you...");
    blacksmith->addDialogue("1. Trade me bread (honorable)");
    blacksmith->addDialogue("2. Steal it when I'm not looking (risky)");
    blacksmith->addDialogue("3. Threaten me (evil)");
    blacksmith->setInteraction("bread", "rusty key");
    blacksmith->addResponse("steal", "You try to steal the key...");
    blacksmith->addResponse("threaten", "You threaten the blacksmith...");

    // Forest NPC - Hermit
    NPC* hermit = new NPC("Hermit", "An old man with wild hair and knowing eyes", forest);
    hermit->addDialogue("The amulet was shattered into three fragments:");
    hermit->addDialogue("- The amethyst lies buried in the abandoned mine.");
    hermit->addDialogue("- The sapphire is guarded by the temple's ghost.");
    hermit->addDialogue("- The ruby rests atop the sorcerer's tower.");
    hermit->addDialogue("I can help you, but first...");
    hermit->addDialogue("1. Give me the potion (trust)");
    hermit->addDialogue("2. Attack me and take what you want (violence)");
    hermit->addDialogue("3. Leave me alone (miss opportunity)");
    hermit->setInteraction("potion", "scroll");

    // Temple NPC - Ghostly Priestess
    NPC* priestess = new NPC("Ghostly Priestess", "A translucent figure in ancient robes", temple);
    priestess->addDialogue("Only those who offer medicinal herbs may pass.");
    priestess->addDialogue("The sapphire fragment is yours... if you heal my lingering pain.");
    priestess->setInteraction("herbs", "sapphire");

    // Create new NPCs that trigger moral choices
    NPC* elderVillager = new NPC("Elder", "A frail old man with wisdom in his eyes", village);
    elderVillager->addDialogue("Our village has suffered greatly from the curse.");
    elderVillager->addDialogue("Many are hungry and sick. Can you help us?");
    elderVillager->addDialogue("1. Share some of your supplies (Selfless)");
    elderVillager->addDialogue("2. Ignore the suffering and focus on your quest (Selfish)");
    elderVillager->addDialogue("3. Demand payment for your help (Very Selfish)");

    // Add a bandit NPC to create moral choices
    NPC* bandit = new NPC("Bandit", "A rough-looking man with a knife", forest);
    bandit->addDialogue("Stay back! This is my territory now!");
    bandit->addDialogue("I've been driven to stealing by the curse... my family is starving.");
    bandit->addDialogue("1. Attack the bandit (Combat)");
    bandit->addDialogue("2. Forgive and help him (Selfless)");
    bandit->addDialogue("3. Threaten and rob him instead (Very Selfish)");

    // Add a dark spirit that tempts the player
    NPC* darkSpirit = new NPC("Dark Spirit", "A shadowy figure that whispers temptations", tower);
    darkSpirit->addDialogue("I sense great potential in you...");
    darkSpirit->addDialogue("Why save these ungrateful villagers? Take the power for yourself!");
    darkSpirit->addDialogue("I can show you how to corrupt the amulet for your own gain.");
    darkSpirit->addDialogue("1. Reject the spirit's offer (Selfless)");
    darkSpirit->addDialogue("2. Listen to learn more (Neutral)");
    darkSpirit->addDialogue("3. Embrace the darkness (Dark Path)");

    // Create a rival adventurer who can be ally or enemy
    NPC* rival = new NPC("Rival Adventurer", "A determined treasure hunter also seeking the amulet", temple);
    rival->addDialogue("So, you're after the amulet too? We could work together... or against each other.");
    rival->addDialogue("What do you say - alliance or competition?");
    rival->addDialogue("1. Suggest working together (Cooperative)");
    rival->addDialogue("2. Challenge to a race for the amulet (Competitive)");
    rival->addDialogue("3. Attack and eliminate the competition (Dark Path)");
    rival->setInteraction("trust", "map piece");

    // Add a corrupted villager who can be saved or sacrificed
    NPC* corruptedVillager = new NPC("Corrupted Villager", "A villager whose mind has been twisted by the curse", mine);
    corruptedVillager->addDialogue("*Growls and mutters incoherently*");
    corruptedVillager->addDialogue("The darkness... it speaks... must obey...");
    corruptedVillager->addDialogue("1. Try to save them (Difficult, Selfless)");
    corruptedVillager->addDialogue("2. End their suffering mercifully (Neutral)");
    corruptedVillager->addDialogue("3. Sacrifice their corrupted essence for power (Dark Path)");

    // Debug output
    cout << "=== WORLD INITIALIZED ===\n";
    cout << "Village contains: " << village->getContains().size() << " entities\n";
    cout << "Forest contains: " << forest->getContains().size() << " entities\n";
    cout << "Mine contains: " << mine->getContains().size() << " entities\n";
    cout << "Temple contains: " << temple->getContains().size() << " entities\n";
    cout << "Tower contains: " << tower->getContains().size() << " entities\n";
}

Room* World::GetStartingRoom() {
    return village;
}

Room* World::CreateRoomsAndExits() {
    return village;
}