using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class LargeRoom : Room
    {
        public List<Encounter> Encounters { get; private set; } = new();

        private Random random = new();

        public LargeRoom((int, int) position)
            : base(Utils.RoomType.Large, position) { }

        public void GenerateEncounters()
        {
            Console.WriteLine("Generating encounters in the Large Room...");

            // Example: Randomly add 1-3 encounters
            int encounterCount = random.Next(1, 4);

            for (int i = 0; i < encounterCount; i++)
            {
                int encounterType = random.Next(1, 4); // Randomly choose encounter type
                switch (encounterType)
                {
                    case 1: // Enemy Encounter
                        Enemy enemy = new Goblin(); // Example: Create a Goblin enemy
                        Encounters.Add(new EnemyEncounter(enemy));
                        break;

                    case 2: // Trap Encounter
                        int trapDamage = DiceRoller.Roll(1, 6); // Example: 1d6 damage trap
                        Encounters.Add(new Trap(trapDamage));
                        break;

                    case 3: // Treasure Encounter
                        Item treasure = new Potion("Healing Potion", RarityType.Common, Potion.PotionType.Healing);
                        Encounters.Add(new TreasureEncounter(treasure));
                        break;
                }
            }
        }

        public void ResolveEncounters(Player player)
        {
            foreach (var encounter in Encounters)
            {
                encounter.Resolve(player);
                if (!player.IsAlive) break; // Stop if the player dies
            }
        }
    }
}
