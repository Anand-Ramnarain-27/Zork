#pragma once
#include "Player.h"
#include "Room.h"
#include <string>
#include <iostream>

class StatusBar {
public:
    static void Display(const Player& player) {
        // Clear previous status bar by printing newlines
        std::cout << "\n\n";

        // Create the status bar string
        std::string status = "| " + player.getLocation()->getName() + " | " +
            "Health: " + std::to_string(player.getHealth()) + "/" +
            std::to_string(player.getMaxHealth()) + " | " +
            "Alignment: " + GetAlignmentString(player.getAlignment()) + " | " +
            "Moves: " + std::to_string(player.getMovesTaken()) + " |";

        // Calculate padding to make the bar full width
        const int terminalWidth = 80; // Default terminal width
        int padding = terminalWidth - status.length() - 2;

        // Print the status bar
        std::cout << "+" << std::string(terminalWidth - 2, '-') << "+\n";
        std::cout << "|" << status << std::string(std::max(0, padding), ' ') << "|\n";
        std::cout << "+" << std::string(terminalWidth - 2, '-') << "+\n\n";
    }

private:
    static std::string GetAlignmentString(int alignment) {
        if (alignment < -5) return "Evil";
        if (alignment < -2) return "Selfish";
        if (alignment <= 2) return "Neutral";
        if (alignment <= 5) return "Good";
        return "Saintly";
    }
};