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

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🙌 Credits

* Developed by Anand Ramnarain

---

🧙‍♂️ **Enjoy your adventure in Eldoria!** The kingdom's fate is in your hands.
