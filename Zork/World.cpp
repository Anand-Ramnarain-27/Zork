#include "World.h"
#include "Room.h"
#include "Exit.h"
#include "Item.h"
#include "NPC.h"
#include <iostream>
#include <vector>

// Static room pointers
static Room* village = nullptr;
static Room* forest = nullptr;
static Room* mine = nullptr;
static Room* temple = nullptr;
static Room* tower = nullptr;

void World::InitializeWorld() {
    // Clean up any previous initialization
    if (village != nullptr) {
        delete village;
        delete forest;
        delete mine;
        delete temple;
        delete tower;
    }

    // ===== CREATE ROOMS =====
    village = new Room("Village of Eldoria",
        "The peaceful starting village. Wooden cottages with smoking chimneys line the dirt paths. "
        "The villagers glance at you nervously, whispering about the curse. To the north, a path leads "
        "to the Enchanted Forest. A locked hatch in the town square descends into darkness.");

    forest = new Room("Enchanted Forest",
        "Ancient trees tower above you, their leaves glowing faintly with bioluminescent fungi. "
        "Strange whispers dance on the wind. A shadowy figure (the hermit?) watches from between "
        "the trees. The path back south leads to the village, while an overgrown trail winds east.");

    mine = new Room("Abandoned Mine",
        "Pitch-black darkness swallows everything. You hear skittering noises...", true);

    temple = new Room("Ruined Temple",
        "Crumbling stone pillars surround a central altar carved with three gem-shaped depressions. "
        "Faded murals depict the amulet's destruction. A ghostly priestess drifts near the staircase, "
        "which is barred by an ethereal lock. The forest lies west.");

    tower = new Room("Sorcerer's Tower",
        "The air hums with dark energy. A cracked obsidian altar dominates the room, pulsing with "
        "malevolent light. The restored amulet could break the curse... if placed here. The staircase "
        "descends back to the temple.");

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

    // Mine items (dark room)
    mine->setDark(true);
    mine->addEntity(new Item("pickaxe", "A rusty but usable pickaxe", false, 0, false));
    mine->addEntity(new Item("amethyst", "A jagged purple shard humming with arcane energy. It feels warm to the touch.",
        false, 0, true)); // Amulet fragment 1/3
    mine->addEntity(new Item("dark shrine",
        "A sinister obsidian structure pulsing with malevolent energy. Blood stains its base.",
        false, 0, false, true)); // Added 'true' to mark as fixed

    // Temple items
    temple->addEntity(new Item("golden key", "Ornate and cold to the touch. The priestess might know its purpose.", false, 0, false));
    temple->addEntity(new Item("sapphire", "A smooth blue fragment encased in ice that never melts. It whispers when held.",
        false, 0, true)); // Amulet fragment 2/3
    temple->addEntity(new Item("forge",
        "An ancient stone forge with mystical engravings. It has three depressions shaped like gems.",
        false, 0, false, true)); // Added 'true' to mark as fixed

    // Tower items
    tower->addEntity(new Item("ruby", "A blood-red sliver that burns like embers. The curse's power resonates within it.",
        false, 0, true)); // Amulet fragment 3/3
    tower->addEntity(new Item("altar",
        "The cursed altar that started it all. A perfect amulet-shaped depression "
        "glows faintly in its center, waiting for the restored artifact.",
        false, 0, false, true)); // Added 'true' to mark as fixed
    tower->addEntity(new Item("ancient relic",
        "A mysterious artifact vibrating with untapped power. It could be used for good or evil.",
        false, 0, true));

    // ===== ADD NPCs =====
    // Village NPC - Blacksmith
    NPC* blacksmith = new NPC("Blacksmith", "A burly man with soot-covered arms", village);
    blacksmith->addDialogue("Welcome to Eldoria, traveler. The curse grows stronger each day.");
    blacksmith->addDialogue("That mine's been locked since the shadows appeared. No one who enters returns unchanged.");
    blacksmith->addDialogue("If you're set on going down there, you'll need my rusty key.");
    blacksmith->addDialogue("I'll give you the rusty key if you help me first.");
    blacksmith->addDialogue("1. Trade me bread - I haven't eaten today (honorable)");
    blacksmith->addDialogue("2. Steal it when I'm not looking (dishonorable)");
    blacksmith->addDialogue("3. Threaten me for it (evil)");
    blacksmith->setInteraction("bread", "rusty key");
    blacksmith->addResponse("steal", "You try to steal the key while the blacksmith's back is turned...");
    blacksmith->addResponse("threaten", "You threaten the blacksmith with violence if he doesn't hand over the key...");

    // Village NPC - Elder
    NPC* elderVillager = new NPC("Elder", "A frail old man with wisdom in his eyes", village);
    elderVillager->addDialogue("*coughs weakly* Our village suffers greatly under this curse.");
    elderVillager->addDialogue("Children go hungry, the sick grow worse, and shadows take more of us each night.");
    elderVillager->addDialogue("You seem capable. Will you aid us in our time of need?");
    elderVillager->addDialogue("1. Share some of your supplies (Increases positive alignment)");
    elderVillager->addDialogue("2. Ignore our suffering and focus on your quest (Neutral)");
    elderVillager->addDialogue("3. Demand payment for your help (Decreases alignment)");
    elderVillager->addResponse("help", "Bless you, traveler. Your kindness brings light to our darkest hour.");
    elderVillager->addResponse("ignore", "I see. Another who cares only for themselves. May you find what you seek, though it brings you no joy.");
    elderVillager->addResponse("payment", "Even in these desperate times, there are those who would profit from suffering. Here, take these few coins - it's all we can spare.");

    // Forest NPC - Hermit
    NPC* hermit = new NPC("Hermit", "An old man with wild hair and knowing eyes", forest);
    hermit->addDialogue("*coughs weakly* A new seeker of the amulet, are you?");
    hermit->addDialogue("The amulet was shattered into three fragments to prevent its power from being misused:");
    hermit->addDialogue("- The amethyst fragment lies deep in the abandoned mine, guarded by darkness.");
    hermit->addDialogue("- The sapphire fragment is kept by the temple's ghostly priestess.");
    hermit->addDialogue("- The ruby fragment rests atop the sorcerer's tower, where the curse began.");
    hermit->addDialogue("I can tell you more, but I need medicine first. That potion you carry would ease my suffering.");
    hermit->addDialogue("1. Give me the potion (build trust)");
    hermit->addDialogue("2. Attack me and take what knowledge I have (choose violence)");
    hermit->addDialogue("3. Leave me to my fate (miss vital information)");
    hermit->setInteraction("potion", "scroll");
    hermit->setHasImportantInfo(true);
    hermit->addResponse("attack", "You raise your weapon against the frail hermit...");
    hermit->addResponse("help", "Thank you for your kindness. The lantern you found will protect you in the darkness.");

    // Forest NPC - Bandit
    NPC* bandit = new NPC("Bandit", "A rough-looking man with a knife", forest);
    bandit->addDialogue("*points knife* Stay back! This is my territory now!");
    bandit->addDialogue("*lowers knife slightly* The curse... it's driven us all to desperation.");
    bandit->addDialogue("My family is starving in the village. I never wanted to become a thief.");
    bandit->addDialogue("1. Attack the bandit (Combat path)");
    bandit->addDialogue("2. Forgive and help him (Greatly increases positive alignment)");
    bandit->addDialogue("3. Threaten and rob him instead (Greatly decreases alignment)");
    bandit->setAsEnemy(true);
    bandit->addResponse("attack", "You draw your weapon as the bandit readies for combat!");
    bandit->addResponse("forgive", "You... would help me? After I threatened you? *tears form in his eyes* I won't forget this mercy.");
    bandit->addResponse("rob", "P-please! Don't take everything! My children will starve!");

    // Mine NPC - Corrupted Villager
    NPC* corruptedVillager = new NPC("Corrupted Villager", "A villager whose mind has been twisted by the curse", mine);
    corruptedVillager->addDialogue("*growls and mutters incoherently while stumbling toward you*");
    corruptedVillager->addDialogue("The d-darkness... it speaks to me... must... obey... *reaches toward you with blackened hands*");
    corruptedVillager->addDialogue("H-help... me... or... end... this...");
    corruptedVillager->addDialogue("1. Try to save them with herbs (Difficult, major alignment increase)");
    corruptedVillager->addDialogue("2. End their suffering mercifully (Neutral alignment)");
    corruptedVillager->addDialogue("3. Sacrifice their corrupted essence for power (Major alignment decrease)");
    corruptedVillager->setAsEnemy(true);
    corruptedVillager->addResponse("save", "You attempt to administer herbs to calm the corrupted villager...");
    corruptedVillager->addResponse("mercy", "You end the villager's suffering quickly and painlessly. It was the only humane choice.");
    corruptedVillager->addResponse("sacrifice", "As you perform the dark ritual, the villager's corrupted essence flows into you, granting forbidden power...");

    // Temple NPC - Ghostly Priestess
    NPC* priestess = new NPC("Ghostly Priestess", "A translucent figure in ancient robes", temple);
    priestess->addDialogue("*her voice echoes eerily* The living do not belong here.");
    priestess->addDialogue("My spirit is bound to this place by pain and regret.");
    priestess->addDialogue("Only those who offer healing herbs may earn my trust and aid.");
    priestess->addDialogue("The sapphire fragment you seek is within my keeping. Bring herbs to ease my eternal suffering.");
    priestess->addDialogue("Once you have all three fragments, return here to combine them at the altar.");
    priestess->setInteraction("herbs", "sapphire");
    priestess->addResponse("help", "You have shown compassion to the dead. Remember this path when darkness tempts you.");
    priestess->addResponse("attack", "Your weapon passes through my spectral form. How foolish to attack what cannot be harmed by mortal means.");

    // Temple NPC - Rival Adventurer
    NPC* rival = new NPC("Rival Adventurer", "A determined treasure hunter also seeking the amulet", temple);
    rival->addDialogue("Well, well... another one seeking the amulet. Competition arrives.");
    rival->addDialogue("We could work together and share the glory, or race to claim it first.");
    rival->addDialogue("I've already found clues to the ruby's location. What do you say?");
    rival->addDialogue("1. Suggest working together (Cooperative path)");
    rival->addDialogue("2. Challenge to a race for the amulet (Competitive path)");
    rival->addDialogue("3. Attack and eliminate the competition (Dark path)");
    rival->setInteraction("trust", "map piece");
    rival->addResponse("cooperate", "Smart choice. Two seekers are better than one against the dangers ahead.");
    rival->addResponse("compete", "A challenge it is! May the best adventurer win - no hard feelings either way.");
    rival->addResponse("attack", "So that's how it's going to be? *draws weapon* I won't go down easily!");
    rival->setPreventReinteraction(true);

    // Tower NPC - Dark Spirit
    NPC* darkSpirit = new NPC("Dark Spirit", "A shadowy figure that whispers temptations", tower);
    darkSpirit->addDialogue("*a voice like smoke in your mind* I sense great potential in you...");
    darkSpirit->addDialogue("Why save these ungrateful villagers? The power of the amulet could be yours alone.");
    darkSpirit->addDialogue("I can show you how to corrupt the amulet fragments. Direct their power for your own desires.");
    darkSpirit->addDialogue("The world has never shown you kindness. Why show it mercy?");
    darkSpirit->addDialogue("1. Reject the spirit's offer (Maintain alignment)");
    darkSpirit->addDialogue("2. Listen to learn more (Slight alignment decrease)");
    darkSpirit->addDialogue("3. Embrace the darkness (Major alignment decrease, gain dark powers)");
    darkSpirit->addResponse("reject", "You will regret spurning such power when the darkness claims you!");
    darkSpirit->addResponse("listen", "Yes... consider the possibilities. The fragments themselves can be corrupted, their power twisted to serve only you.");
    darkSpirit->addResponse("embrace", "Excellent. The corruption begins with your heart and extends to the amulet. Sacrifice at the dark shrine to seal your path.");
    darkSpirit->setPreventReinteraction(true);

    // Debug output
    std::cout << "=== WORLD INITIALIZED ===\n";
    std::cout << "Village contains: " << village->getContains().size() << " entities\n";
    std::cout << "Forest contains: " << forest->getContains().size() << " entities\n";
    std::cout << "Mine contains: " << mine->getContains().size() << " entities\n";
    std::cout << "Temple contains: " << temple->getContains().size() << " entities\n";
    std::cout << "Tower contains: " << tower->getContains().size() << " entities\n";
}

Room* World::GetStartingRoom() {
    return village;
}

Room* World::CreateRoomsAndExits() {
    return village;
}