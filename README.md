# Zork

# The Lost Amulet of Eldoria - Text Adventure Game

## 🧭 Overview

**The Lost Amulet of Eldoria** is a text-based adventure game where you play as an adventurer tasked with recovering the fragments of a powerful amulet to save the kingdom from a terrible curse. Explore interconnected locations, solve puzzles, interact with NPCs, and make moral choices that affect the game's outcome.

## ✨ Features

- 🗺️ 5+ interconnected locations (Village, Forest, Mine, Temple, Tower)
- 🎒 Inventory system with container items (e.g. backpack)
- 💬 NPC dialogue system with multiple interaction options
- ⚖️ Moral alignment system affecting story outcomes
- 🌑 Darkness mechanics requiring light sources
- 🔐 Locked exits requiring keys
- 🧭 Multiple endings based on player choices
- 📜 Original storyline with puzzles and quests

## 🎮 How to Play

### 🧾 Basic Commands

| Command          | Description                     | Example           |
|------------------|---------------------------------|-------------------|
| `go [direction]` | Move in specified direction     | `go north`        |
| `n`, `s`, etc.   | Shortcut for directions         | `n` (go north)    |
| `look`           | Examine current location        | `look`            |
| `take [item]`    | Pick up an item                 | `take lantern`    |
| `drop [item]`    | Drop an item                    | `drop key`        |
| `inventory`      | View your inventory             | `inventory`       |
| `use [item]`     | Use an item                     | `use lantern`     |
| `examine [item]` | Examine an item closely         | `examine amulet`  |
| `talk [npc]`     | Talk to an NPC                  | `talk hermit`     |
| `help`           | Show available commands         | `help`            |
| `quit`           | Exit the game                   | `quit`            |

### 🧠 Advanced Commands

| Command              | Description                             | Example             |
|----------------------|-----------------------------------------|---------------------|
| `combine amulet`     | Combine fragments at temple altar       | `combine amulet`    |
| `place amulet`       | Place completed amulet on tower altar   | `place amulet`      |
| `attack [npc]`       | Attack an NPC                           | `attack bandit`     |
| `forgive [npc]`      | Show mercy to an enemy                  | `forgive bandit`    |
| `sacrifice [npc]`    | Dark ritual at shrine                   | `sacrifice villager`|
| `corrupt [item]`     | Corrupt an artifact for power           | `corrupt relic`     |
| `alignment`          | Check your moral standing               | `alignment`         |
| `steal [item]`       | Attempt to steal an item                | `steal key`         |

## 🗺️ Walkthrough (Basic Path to Victory)

1. **Village of Eldoria**
   - Take `backpack` and read the `note`
   - Talk to the `blacksmith` about the mine key
   - Trade `bread` for `rusty key`

2. **Enchanted Forest**
   - Find `lantern` and `potion`
   - Give `potion` to `hermit` for info and a `scroll`

3. **Abandoned Mine**
   - Use `rusty key` to enter
   - Use `lantern` to explore
   - Collect `amethyst fragment` and escape the darkness

4. **Ruined Temple**
   - Give `herbs` to `ghostly priestess`
   - Receive `sapphire fragment` and `golden key`
   - Use `combine amulet` at the altar

5. **Sorcerer's Tower**
   - Use `golden key` to enter
   - Find `ruby fragment`
   - `place amulet` to complete the quest

## 🔚 Multiple Endings

Your choices determine your fate:

- 🦸‍♂️ Heroic Ending – Break the curse (positive alignment)
- ⚖️ Neutral Ending – Destroy the amulet
- 🧛 Dark Ending – Claim the amulet’s power (negative alignment)
- 👑 Tyrant Ending – Embrace total corruption (very negative alignment)
- 💀 Death Ending – Die before completing your quest

## 🛠️ Development

### Class Structure

```text
Entity
├── Creature
│   ├── Player
│   └── NPC
├── Room
├── Exit
└── Item
````

### Technical Features

* Polymorphic design using virtual functions
* Const-correct implementation
* Case-insensitive command parsing
* Inventory containers and limits
* Dialogue trees with branching logic

---

# 🛠️ My Approach to the Zork-like Game Project

## 📘 Introduction

When tasked with creating a simple text-based conversational game inspired by Zork, I approached the project with a structured methodology: research, planning, implementation, testing, and refinement. However, time constraints and initial misunderstandings about the original game's mechanics added unexpected challenges. Despite this, I adhered to the project goals and expanded the game with a unique moral alignment system to enhance player choice and replayability.

## 🔍 Research Phase

My initial research was incomplete—I understood that Zork was a text-based adventure game but didn’t fully grasp its narrative-driven nature. After playing the original game and watching gameplay videos, I realized my oversight: Zork immerses players through descriptive storytelling and interactive dialogue, whereas my early design treated the world and story as separate elements. This realization helped me refine my approach to better align with the spirit of Zork.

## 🗂️ Planning

With a clearer understanding, I outlined the game’s structure:

- **Storyline**: A single main plotline with one ending (later expanded to multiple endings based on moral choices).
- **Gameplay Features**:  
  - Connected rooms (5+ as recommended)  
  - Interactive items (pickup, drop, nesting)  
  - NPCs with dialogue options affecting the player’s moral alignment  
  - A moral system influencing the game’s ending

## 🧱 Implementation

### 🔧 Starting Point

Following an unorthodox but effective method taught by my lecturer, I began by drafting base scripts. These were non-functional placeholders but provided a scaffold for the project. My first priority was ensuring the scripts compiled without errors, even if they lacked functionality. This step confirmed the project’s foundational stability.

### 🏗️ Building the World

1. **Rooms and Navigation**  
   - Created rooms and linked them via exits using C++ classes like `Room` and `Exit`.

2. **Items and Interactions**  
   - Implemented pickup/drop mechanics and containers using polymorphism (`Entity` → `Item`).

3. **NPCs and Dialogue**  
   - Initially added transactional NPCs, but evolved into dialogue systems offering moral choices.

### ⚖️ Challenges and Iteration

The moral alignment system was the most ambitious and challenging addition. I wanted player choices to subtly influence the narrative without disrupting the core storyline.

- **Research**: Studied games like Knights of the Old Republic for moral mechanics.
- **Design**: Built dialogue trees to replace simple item trades; each choice adjusts the player’s alignment.
- **Testing**: Calibrated alignment thresholds to make every choice impactful without punishing experimentation.

## 🧠 Lessons Learned

1. 🎮 Playing the original Zork earlier would have improved the initial design.
2. 🔄 Modular scripting allowed for manageable and extensible development.
3. 🧍 Player agency (via moral choice) added substantial replayability and engagement.

## ✅ Conclusion

This project was both rewarding and educational. While parts of the implementation were straightforward (e.g., room navigation), others (e.g., the moral system) required creative problem-solving. Overcoming these challenges deepened my understanding of C++ and game design principles. Despite its simplicity, the game achieved its goals—and the extra feature (moral alignment) became its standout element. I’m proud of the final product and the skills I gained along the way.

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🙌 Credits

* Developed by Anand Ramnarain

---

🧙‍♂️ **Enjoy your adventure in Eldoria!** The kingdom's fate is in your hands.
