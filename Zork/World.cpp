#include "World.h"
#include "Room.h"
#include "Exit.h"
#include "GameEnums.h"
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
    new Exit(Direction::NORTH, village, forest, "forest path",
        "A well-trodden path into the forest");
    new Exit(Direction::SOUTH, forest, village, "village path",
        "Path back to the village");

    // Forest ↔ Temple connection
    new Exit(Direction::EAST, forest, temple, "overgrown trail",
        "Barely visible trail through bushes");
    new Exit(Direction::WEST, temple, forest, "forest trail",
        "Trail back to the forest");

    // Locked mine entrance (requires rusty key)
    new Exit(Direction::DOWN, village, mine, "mine entrance",
        "Wooden hatch leading down", true, "rusty key");
    new Exit(Direction::UP, mine, village, "mine exit",
        "Way back up to the village");

    // Temple ↔ Tower (final area)
    new Exit(Direction::UP, temple, tower, "spiral staircase",
        "Ancient stone staircase upward");
    new Exit(Direction::DOWN, tower, temple, "stone steps",
        "Steps leading back down");
}

// Returns the player's starting location
Room* World::GetStartingRoom() {
    return village;
}

Room* World::CreateRoomsAndExits() {
    // Actual creation happens in InitializeWorld()
    return village;
}