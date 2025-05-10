#pragma once
#include <string>

// Possible movement directions in the game
enum class Direction {
    NORTH,  
    SOUTH, 
    EAST,  
    WEST, 
    UP,     
    DOWN    
};

/**
 * Converts Direction enum to readable string
 * @return String representation ("north", "south", etc.)
 */
inline std::string directionToString(Direction direction) {
    switch (direction) {
    case Direction::NORTH: return "north";
    case Direction::SOUTH: return "south";
    case Direction::EAST:  return "east";
    case Direction::WEST:  return "west";
    case Direction::UP:    return "up";
    case Direction::DOWN:  return "down";
    default:  return "unknown";
    }
}