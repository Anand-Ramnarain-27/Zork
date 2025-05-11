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
     // Village exits
    new Exit(Direction::NORTH, village, forest, "forest path", "A well-trodden path leading into the forest");
    new Exit(Direction::DOWN, village, mine, "mine entrance", "A wooden hatch leading down to the mines", true, "rusty key");

    // Forest exits
    new Exit(Direction::SOUTH, forest, village, "village path", "The path back to the village");
    new Exit(Direction::EAST, forest, temple, "overgrown trail", "A barely visible trail through thick bushes");

    // Temple exits
    new Exit(Direction::WEST, temple, forest, "forest trail", "The trail back to the forest");
    new Exit(Direction::UP, temple, tower, "spiral staircase", "An ancient stone staircase winding upward");

    // Mine exits
    new Exit(Direction::UP, mine, village, "mine exit", "The path back to the village");

    // Tower exits
    new Exit(Direction::DOWN, tower, temple, "stone steps", "The steps leading back down to the temple");


    new Exit(Direction::NORTH, village, forest, "forest path", "A well-trodden path leading into the forest");
    new Exit(Direction::SOUTH, forest, village, "village path", "The path back to the village");

    new Exit(Direction::EAST, forest, temple, "overgrown trail", "A barely visible trail through thick bushes");
    new Exit(Direction::WEST, temple, forest, "forest trail", "The trail back to the forest");
}

// Returns the player's starting location
Room* World::GetStartingRoom() {
    return village;
}

Room* World::CreateRoomsAndExits() {
    // Actual creation happens in InitializeWorld()
    return village;
}