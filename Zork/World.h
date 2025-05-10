#pragma once
#include "Room.h"

class World {
public:
    // Initializes all game locations and connections
    static void InitializeWorld();

    // Gets the player's starting location
    static Room* GetStartingRoom();

private:
    static Room* CreateRoomsAndExits();
};