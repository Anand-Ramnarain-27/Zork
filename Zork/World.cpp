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
        "The peaceful starting village. Smoke rises from chimneys.");

    forest = new Room("Enchanted Forest",
        "A dense, magical forest with strange lights.");

    mine = new Room("Abandoned Mine",
        "Dark mine shaft with rusty tools scattered about.");

    temple = new Room("Ruined Temple",
        "Once-great temple with faded murals of an amulet.");

    tower = new Room("Sorcerer's Tower",
        "Tall stone tower with an ominous presence.");

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
    village->addEntity(new Item("bread", "A loaf of stale bread", false, 0, false));
    village->addEntity(new Item("map", "A map of Eldoria", false, 0, false));

    // Special container item
    Item* backpack = new Item("backpack", "A sturdy leather backpack", true, 5, false);
    village->addEntity(backpack);
    backpack->addEntity(new Item("note", "A note: 'Find the hermit in the forest'", false, 0, false));

    // Forest items
    forest->addEntity(new Item("lantern", "A brass lantern with oil", false, 0, false));
    forest->addEntity(new Item("potion", "A glowing red potion", false, 0, false));
    forest->addEntity(new Item("herbs", "Medicinal forest herbs", false, 0, false));

    // Mine items
    mine->addEntity(new Item("pickaxe", "A rusty but usable pickaxe", false, 0, false));
    mine->addEntity(new Item("amethyst", "A purple gemstone fragment", true, 0, true)); // Amulet fragment 1/3

    // Temple items
    temple->addEntity(new Item("golden key", "An ornate key with eldritch markings", false, 0, false));
    //temple->addEntity(new Item("sapphire", "A blue gemstone fragment", true, 0, true)); // Amulet fragment 2/3

    // Tower items
    tower->addEntity(new Item("ruby", "A red gemstone fragment", true, 0, true)); // Amulet fragment 3/3
    tower->addEntity(new Item("altar",
        "An ancient altar with three gem-shaped depressions.\n"
        "The carvings seem to match the amulet fragments you've found.",
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