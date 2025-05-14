#include "World.h"
#include "Room.h"
#include "Exit.h"
#include "NPC.h"
#include "Item.h"
#include <iostream>
#include <vector>

// World locations
static Room* village;
static Room* forest;
static Room* mine;
static Room* temple;
static Room* tower;

// Sets up all game locations and connections
void World::InitializeWorld() {
    // Create main locations
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

    // Connect locations (unlocked paths)
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

    // Temple items
    temple->addEntity(new Item("golden key", "Ornate and cold to the touch. The priestess might know its purpose.", false, 0, false));
    temple->addEntity(new Item("sapphire", "A smooth blue fragment encased in ice that never melts. It whispers when held.", true, 0, true)); // Amulet fragment 2/3

    // Tower items
    tower->addEntity(new Item("ruby", "A blood-red sliver that burns like embers. The curse's power resonates within it.", true, 0, true)); // Amulet fragment 3/3
    tower->addEntity(new Item("altar",
        "The cursed altar that started it all. A perfect amulet-shaped depression "
        "glows faintly in its center, waiting for the restored artifact.",
        false, 0, false));

    // ===== ADD NPCs =====
    // Village NPC - Blacksmith
    NPC* blacksmith = new NPC("Blacksmith", "A burly man with soot-covered arms", village);
    blacksmith->addDialogue("Welcome to Eldoria, traveler.");
    blacksmith->addDialogue("That mine's been locked since the curse began.");
    blacksmith->addDialogue("Bring me some bread and I'll give you the key.");
    blacksmith->setInteraction("bread", "rusty key");

    // Forest NPC - Hermit
    NPC* hermit = new NPC("Hermit", "An old man with wild hair and knowing eyes", forest);
    hermit->addDialogue("The amulet was shattered into three fragments...");
    hermit->addDialogue("Bring me a potion and I'll tell you more.");
    hermit->addDialogue("The temple holds the second fragment... but beware the ghost!");
    hermit->setInteraction("potion", "scroll");

    // Temple NPC - Ghostly Priestess
    NPC* priestess = new NPC("Ghostly Priestess", "A translucent figure in ancient robes", temple);
    priestess->addDialogue("Only the worthy may ascend to the tower...");
    priestess->addDialogue("Bring me the golden key... and I'll grant passage...");
    priestess->setInteraction("herbs", "sapphire");

    // Debug output
    cout << "=== WORLD INITIALIZED ===\n";
    cout << "Village contains: " << village->getContains().size() << " entities\n";
    cout << "Forest contains: " << forest->getContains().size() << " entities\n";
    cout << "Mine contains: " << mine->getContains().size() << " entities\n";
    cout << "Temple contains: " << temple->getContains().size() << " entities\n";
    cout << "Tower contains: " << tower->getContains().size() << " entities\n";
}

// Returns the player's starting location
Room* World::GetStartingRoom() {
    return village;
}

Room* World::CreateRoomsAndExits() {
    // Actual creation happens in InitializeWorld()
    return village;
}